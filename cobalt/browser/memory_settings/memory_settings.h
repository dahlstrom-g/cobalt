/*
 * Copyright 2017 Google Inc. All Rights Reserved.
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

#ifndef COBALT_BROWSER_MEMORY_SETTINGS_MEMORY_SETTINGS_H_
#define COBALT_BROWSER_MEMORY_SETTINGS_MEMORY_SETTINGS_H_

#include "cobalt/math/size.h"
#include "starboard/types.h"

namespace cobalt {
namespace browser {
namespace memory_settings {

// Gets the ImageCacheSize in bytes from the given dimensions. If
// COBALT_IMAGE_CACHE_SIZE_IN_BYTES is defined, then this is the value
// that is returned, otherwise the value is generated via a call to
// CalculateImageCacheSize().
size_t GetImageCacheSize(const math::Size& dimensions);

// Get's the width and height of the skia atlas texture. Optionally applies
// overrides if they have been defined.
math::Size GetSkiaAtlasTextureSize(const math::Size& ui_resolution);

// Calculates the ImageCacheSize in bytes.
// The return ranges from [kMinImageCacheSize, kMaxImageCacheSize].
size_t CalculateImageCacheSize(const math::Size& dimensions);

// Calculates the SkiaAtlasTextureSize.
// When the ui resolution is 1920x1080, then the returned atlas texture size
// will be 8192x4096. The texture will scale up and down, by powers of two,
// in relation to the input ui_resolution. The returned value will be clamped
// such that
// kMinSkiaTextureAtlasWidth <= output.width() <= kMaxSkiaTextureAtlasWidth
// and
// kMinSkiaTextureAtlasHeight <= output.height() <= kMaxSkiaTextureAtlasHeight
// will be true.
math::Size CalculateSkiaAtlasTextureSize(const math::Size& ui_resolution);

/////////////////////////////// Implementation ///////////////////////////////
enum MemorySizes {
  kMinImageCacheSize = 20 * 1024 * 1024,  // 20mb.
  kMaxImageCacheSize = 64 * 1024 * 1024,  // 64mb

  kMinSkiaTextureAtlasWidth = 2048,
  kMinSkiaTextureAtlasHeight = 2048,
};

}  // namespace memory_settings
}  // namespace browser
}  // namespace cobalt

#endif  // COBALT_BROWSER_MEMORY_SETTINGS_MEMORY_SETTINGS_H_
