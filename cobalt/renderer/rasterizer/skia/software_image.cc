/*
 * Copyright 2014 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cobalt/renderer/rasterizer/skia/software_image.h"

#include "base/memory/aligned_memory.h"
#include "cobalt/renderer/rasterizer/skia/cobalt_skia_type_conversions.h"

namespace cobalt {
namespace renderer {
namespace rasterizer {
namespace skia {

SkiaSoftwareImageData::SkiaSoftwareImageData(
    const math::Size& size, render_tree::PixelFormat pixel_format,
    render_tree::AlphaFormat alpha_format)
    : descriptor_(size, pixel_format, alpha_format,
                  size.width() * render_tree::BytesPerPixel(pixel_format)),
      pixel_data_(new uint8_t[size.height() * descriptor_.pitch_in_bytes]) {}

const render_tree::ImageDataDescriptor& SkiaSoftwareImageData::GetDescriptor()
    const {
  return descriptor_;
}

uint8_t* SkiaSoftwareImageData::GetMemory() { return pixel_data_.get(); }

scoped_array<uint8_t> SkiaSoftwareImageData::PassPixelData() {
  return pixel_data_.Pass();
}

SkiaSoftwareImage::SkiaSoftwareImage(
    scoped_ptr<SkiaSoftwareImageData> source_data) {
  owned_pixel_data_ = source_data->PassPixelData();
  Initialize(owned_pixel_data_.get(), source_data->GetDescriptor());
}

SkiaSoftwareImage::SkiaSoftwareImage(
    uint8_t* source_data, const render_tree::ImageDataDescriptor& descriptor) {
  Initialize(source_data, descriptor);
}

namespace {
// Converts UV8 texture data to an ARGB SkBitmap where the original U channel is
// found in the B channel, the original V channel is found in the A channel, and
// the R and G channels are zeroed out.
void ConvertUV8ToARGBSkBitmap(
    uint8_t* source_data, const render_tree::ImageDataDescriptor& descriptor,
    SkBitmap* bitmap) {
  bitmap->allocN32Pixels(descriptor.size.width(), descriptor.size.height());
  int row_pixels_dest = bitmap->rowBytes() / bitmap->bytesPerPixel();

  bitmap->lockPixels();
  uint8_t* current_src = source_data;
  SkColor* current_dest = static_cast<SkColor*>(bitmap->getPixels());
  for (int row = 0; row < descriptor.size.height(); ++row) {
    for (int column = 0; column < descriptor.size.width(); ++column) {
      current_dest[column] = SkColorSetARGBMacro(
          current_src[column * 2 + 1], 0, 0, current_src[column * 2 + 0]);
    }
    current_dest += row_pixels_dest;
    current_src += descriptor.pitch_in_bytes;
  }
  bitmap->unlockPixels();
}
}  // namespace

void SkiaSoftwareImage::Initialize(
    uint8_t* source_data, const render_tree::ImageDataDescriptor& descriptor) {
  SkAlphaType skia_alpha_format =
      RenderTreeAlphaFormatToSkia(descriptor.alpha_format);
  DCHECK_EQ(kPremul_SkAlphaType, skia_alpha_format);

  size_ = descriptor.size;

  if (descriptor.pixel_format == render_tree::kPixelFormatUV8) {
    // Convert UV8 to ARGB because Skia does not support any two-channel
    // formats.  This of course is not efficient, but efficiency in the software
    // renderer is not as important as completeness and correctness.
    ConvertUV8ToARGBSkBitmap(source_data, descriptor, &bitmap_);
  } else {
// Check that the incoming pixel data is indeed in premultiplied alpha
// format.
#if !defined(NDEBUG)
    SkiaImage::DCheckForPremultipliedAlpha(
        descriptor.size, descriptor.pitch_in_bytes, descriptor.pixel_format,
        source_data);
#endif
    // Convert our incoming pixel data from unpremultiplied alpha to
    // premultiplied alpha format, which is what Skia expects.
    SkImageInfo premul_image_info = SkImageInfo::Make(
        descriptor.size.width(), descriptor.size.height(),
        RenderTreeSurfaceFormatToSkia(descriptor.pixel_format),
        skia_alpha_format);
    bitmap_.installPixels(premul_image_info, source_data,
                          descriptor.pitch_in_bytes);
  }
}

SkiaSoftwareRawImageMemory::SkiaSoftwareRawImageMemory(size_t size_in_bytes,
                                                       size_t alignment)
    : size_in_bytes_(size_in_bytes),
      pixel_data_(
          static_cast<uint8_t*>(base::AlignedAlloc(size_in_bytes, alignment))) {
}

size_t SkiaSoftwareRawImageMemory::GetSizeInBytes() const {
  return size_in_bytes_;
}

uint8_t* SkiaSoftwareRawImageMemory::GetMemory() { return pixel_data_.get(); }

scoped_ptr_malloc<uint8_t, base::ScopedPtrAlignedFree>
SkiaSoftwareRawImageMemory::PassPixelData() {
  return pixel_data_.Pass();
}

SkiaSoftwareMultiPlaneImage::SkiaSoftwareMultiPlaneImage(
    scoped_ptr<SkiaSoftwareRawImageMemory> raw_image_memory,
    const render_tree::MultiPlaneImageDataDescriptor& descriptor)
    : size_(descriptor.GetPlaneDescriptor(0).size),
      format_(descriptor.image_format()),
      owned_pixel_data_(raw_image_memory->PassPixelData()) {
  for (int i = 0; i < descriptor.num_planes(); ++i) {
    planes_[i] = new SkiaSoftwareImage(
        owned_pixel_data_.get() + descriptor.GetPlaneOffset(i),
        descriptor.GetPlaneDescriptor(i));
  }
}

}  // namespace skia
}  // namespace rasterizer
}  // namespace renderer
}  // namespace cobalt
