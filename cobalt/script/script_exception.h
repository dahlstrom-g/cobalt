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
#ifndef SCRIPT_SCRIPT_EXCEPTION_H_
#define SCRIPT_SCRIPT_EXCEPTION_H_

#include "cobalt/script/wrappable.h"

namespace cobalt {
namespace script {

class ScriptException : public Wrappable {
 public:
  virtual std::string message() const = 0;
  virtual std::string name() const = 0;
};

}  // namespace script
}  // namespace cobalt


#endif  // SCRIPT_SCRIPT_EXCEPTION_H_
