// Copyright 2017 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "cobalt/renderer/rasterizer/egl/hardware_rasterizer.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "base/debug/trace_event.h"
#include "base/memory/scoped_vector.h"
#include "base/threading/thread_checker.h"
#include "cobalt/renderer/backend/egl/framebuffer_render_target.h"
#include "cobalt/renderer/backend/egl/graphics_context.h"
#include "cobalt/renderer/backend/egl/graphics_system.h"
#include "cobalt/renderer/backend/egl/texture.h"
#include "cobalt/renderer/backend/egl/utils.h"
#include "cobalt/renderer/rasterizer/egl/draw_object_manager.h"
#include "cobalt/renderer/rasterizer/egl/graphics_state.h"
#include "cobalt/renderer/rasterizer/egl/offscreen_target_manager.h"
#include "cobalt/renderer/rasterizer/egl/render_tree_node_visitor.h"
#include "cobalt/renderer/rasterizer/egl/shader_program_manager.h"
#include "cobalt/renderer/rasterizer/skia/cobalt_skia_type_conversions.h"
#include "cobalt/renderer/rasterizer/skia/hardware_rasterizer.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/gpu/GrContext.h"

namespace cobalt {
namespace renderer {
namespace rasterizer {
namespace egl {

class HardwareRasterizer::Impl {
 public:
  explicit Impl(backend::GraphicsContext* graphics_context,
                int skia_atlas_width, int skia_atlas_height,
                int skia_cache_size_in_bytes,
                int scratch_surface_cache_size_in_bytes,
                int surface_cache_size_in_bytes,
                bool purge_skia_font_caches_on_destruction);
  ~Impl();

  void Submit(const scoped_refptr<render_tree::Node>& render_tree,
              const scoped_refptr<backend::RenderTarget>& render_target,
              const Options& options);

  void SubmitToFallbackRasterizer(
      const scoped_refptr<render_tree::Node>& render_tree,
      const math::Matrix3F& transform,
      const OffscreenTargetManager::TargetInfo& target);

  render_tree::ResourceProvider* GetResourceProvider() {
    return fallback_rasterizer_->GetResourceProvider();
  }

 private:
  GrContext* GetFallbackContext() {
    return fallback_rasterizer_->GetGrContext();
  }

  void ResetFallbackContextDuringFrame();
  void FlushFallbackOffscreenDraws();

  void RasterizeTree(const scoped_refptr<render_tree::Node>& render_tree,
                     backend::RenderTargetEGL* render_target,
                     const math::Rect& content_rect);

  scoped_ptr<skia::HardwareRasterizer> fallback_rasterizer_;
  scoped_ptr<GraphicsState> graphics_state_;
  scoped_ptr<ShaderProgramManager> shader_program_manager_;
  scoped_ptr<OffscreenTargetManager> offscreen_target_manager_;

  backend::GraphicsContextEGL* graphics_context_;
  base::ThreadChecker thread_checker_;
};

HardwareRasterizer::Impl::Impl(backend::GraphicsContext* graphics_context,
                               int skia_atlas_width, int skia_atlas_height,
                               int skia_cache_size_in_bytes,
                               int scratch_surface_cache_size_in_bytes,
                               int surface_cache_size_in_bytes,
                               bool purge_skia_font_caches_on_destruction)
    : fallback_rasterizer_(new skia::HardwareRasterizer(
          graphics_context, skia_atlas_width, skia_atlas_height,
          skia_cache_size_in_bytes, scratch_surface_cache_size_in_bytes,
          0 /* fallback rasterizer should not use a surface cache */,
          purge_skia_font_caches_on_destruction)),
      graphics_context_(
          base::polymorphic_downcast<backend::GraphicsContextEGL*>(
              graphics_context)) {
  DLOG(INFO) << "surface_cache_size_in_bytes: " << surface_cache_size_in_bytes;

  backend::GraphicsContextEGL::ScopedMakeCurrent scoped_make_current(
      graphics_context_);
  graphics_state_.reset(new GraphicsState());
  shader_program_manager_.reset(new ShaderProgramManager());
  offscreen_target_manager_.reset(new OffscreenTargetManager(
      graphics_context_, GetFallbackContext(), surface_cache_size_in_bytes));
}

HardwareRasterizer::Impl::~Impl() {
  backend::GraphicsContextEGL::ScopedMakeCurrent scoped_make_current(
      graphics_context_);

  GL_CALL(glFinish());
  offscreen_target_manager_.reset();
  shader_program_manager_.reset();
  graphics_state_.reset();
}

void HardwareRasterizer::Impl::Submit(
    const scoped_refptr<render_tree::Node>& render_tree,
    const scoped_refptr<backend::RenderTarget>& render_target,
    const Options& options) {
  DCHECK(thread_checker_.CalledOnValidThread());

  backend::RenderTargetEGL* render_target_egl =
      base::polymorphic_downcast<backend::RenderTargetEGL*>(
          render_target.get());
  backend::GraphicsContextEGL::ScopedMakeCurrent scoped_make_current(
      graphics_context_, render_target_egl);

  fallback_rasterizer_->AdvanceFrame();

  // Update only the dirty pixels if the render target contents are preserved
  // between frames.
  math::Rect content_rect(render_target->GetSize());
  if (options.dirty && render_target_egl->ContentWasPreservedAfterSwap()) {
    content_rect = *options.dirty;
  }

  offscreen_target_manager_->Update(render_target->GetSize());

  RasterizeTree(render_tree, render_target_egl, content_rect);

  graphics_context_->SwapBuffers(render_target_egl);
}

void HardwareRasterizer::Impl::SubmitToFallbackRasterizer(
    const scoped_refptr<render_tree::Node>& render_tree,
    const math::Matrix3F& transform,
    const OffscreenTargetManager::TargetInfo& target) {
  DCHECK(thread_checker_.CalledOnValidThread());
  TRACE_EVENT0("cobalt::renderer", "SubmitToFallbackRasterizer");

  // Use skia to rasterize to the allocated offscreen target.
  target.skia_canvas->save();

  target.skia_canvas->clipRect(SkRect::MakeXYWH(
      target.region.x(), target.region.y(),
      target.region.width(), target.region.height()));
  target.skia_canvas->translate(target.region.x(), target.region.y());
  target.skia_canvas->concat(skia::CobaltMatrixToSkia(transform));

  if (target.is_scratch_surface) {
    // The scratch surface is always dirty, so it must be cleared before use.
    target.skia_canvas->clear(SK_ColorTRANSPARENT);
  }

  fallback_rasterizer_->SubmitOffscreen(render_tree, target.skia_canvas);

  if (target.is_scratch_surface) {
    // Flush the skia draw calls so the contents can be used immediately.
    target.skia_canvas->flush();
  }

  target.skia_canvas->restore();
}

void HardwareRasterizer::Impl::FlushFallbackOffscreenDraws() {
  TRACE_EVENT0("cobalt::renderer", "Skia Flush");
  offscreen_target_manager_->Flush();
}

void HardwareRasterizer::Impl::ResetFallbackContextDuringFrame() {
  // Perform a minimal reset of the fallback context. Only need to invalidate
  // states that this rasterizer pollutes.
  uint32_t untouched_states = kMSAAEnable_GrGLBackendState |
      kStencil_GrGLBackendState | kPixelStore_GrGLBackendState |
      kFixedFunction_GrGLBackendState | kPathRendering_GrGLBackendState |
      kMisc_GrGLBackendState;

  GetFallbackContext()->resetContext(~untouched_states & kAll_GrBackendState);
}

void HardwareRasterizer::Impl::RasterizeTree(
    const scoped_refptr<render_tree::Node>& render_tree,
    backend::RenderTargetEGL* render_target,
    const math::Rect& content_rect) {
  DrawObjectManager draw_object_manager(
      base::Bind(&HardwareRasterizer::Impl::ResetFallbackContextDuringFrame,
                 base::Unretained(this)),
      base::Bind(&HardwareRasterizer::Impl::FlushFallbackOffscreenDraws,
                 base::Unretained(this)));
  RenderTreeNodeVisitor visitor(
      graphics_state_.get(), &draw_object_manager,
      offscreen_target_manager_.get(),
      base::Bind(&HardwareRasterizer::Impl::SubmitToFallbackRasterizer,
                 base::Unretained(this)),
      render_target, content_rect);

  // Traverse the render tree to populate the draw object manager.
  {
    TRACE_EVENT0("cobalt::renderer", "VisitRenderTree");
    render_tree->Accept(&visitor);
  }

  graphics_state_->BeginFrame();

  // Rasterize to offscreen targets using skia.
  {
    TRACE_EVENT0("cobalt::renderer", "OffscreenRasterize");

    // Ensure the skia context is fully reset.
    GetFallbackContext()->resetContext();
    draw_object_manager.ExecuteOffscreenRasterize(graphics_state_.get(),
        shader_program_manager_.get());
  }

  // Clear the dirty region of the render target.
  graphics_state_->BindFramebuffer(render_target);
  graphics_state_->Viewport(0, 0,
                            render_target->GetSize().width(),
                            render_target->GetSize().height());
  graphics_state_->Scissor(content_rect.x(), content_rect.y(),
                           content_rect.width(), content_rect.height());
  graphics_state_->Clear();

  {
    TRACE_EVENT0("cobalt::renderer", "OnscreenRasterize");
    draw_object_manager.ExecuteOnscreenRasterize(graphics_state_.get(),
        shader_program_manager_.get());
  }

  graphics_context_->ResetCurrentSurface();
  graphics_state_->EndFrame();
}

HardwareRasterizer::HardwareRasterizer(
    backend::GraphicsContext* graphics_context, int skia_atlas_width,
    int skia_atlas_height, int skia_cache_size_in_bytes,
    int scratch_surface_cache_size_in_bytes, int surface_cache_size_in_bytes,
    bool purge_skia_font_caches_on_destruction)
    : impl_(new Impl(
          graphics_context, skia_atlas_width, skia_atlas_height,
          skia_cache_size_in_bytes, scratch_surface_cache_size_in_bytes,
          surface_cache_size_in_bytes, purge_skia_font_caches_on_destruction)) {
}

void HardwareRasterizer::Submit(
    const scoped_refptr<render_tree::Node>& render_tree,
    const scoped_refptr<backend::RenderTarget>& render_target,
    const Options& options) {
  TRACE_EVENT0("cobalt::renderer", "HardwareRasterizer::Submit");
  impl_->Submit(render_tree, render_target, options);
}

render_tree::ResourceProvider* HardwareRasterizer::GetResourceProvider() {
  return impl_->GetResourceProvider();
}

}  // namespace egl
}  // namespace rasterizer
}  // namespace renderer
}  // namespace cobalt
