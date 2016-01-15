/*
 * Copyright 2015 Google Inc. All Rights Reserved.
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

#include "cobalt/dom/dom_settings.h"

#include "cobalt/dom/document.h"

namespace cobalt {
namespace dom {

DOMSettings::DOMSettings(loader::FetcherFactory* fetcher_factory,
                         network::NetworkModule* network_module,
                         const scoped_refptr<Window>& window,
                         ArrayBuffer::Allocator* array_buffer_allocator,
                         MediaSource::Registry* media_source_registry,
                         script::JavaScriptEngine* engine,
                         script::GlobalObjectProxy* global_object_proxy)
    : fetcher_factory_(fetcher_factory),
      network_module_(network_module),
      window_(window),
      array_buffer_allocator_(array_buffer_allocator),
      media_source_registry_(media_source_registry),
      javascript_engine_(engine),
      global_object_proxy_(global_object_proxy) {}

DOMSettings::~DOMSettings() {}

GURL DOMSettings::base_url() const {
  return window()->document()->url_as_gurl();
}

}  // namespace dom
}  // namespace cobalt
