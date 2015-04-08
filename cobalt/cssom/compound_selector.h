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

#ifndef CSSOM_COMPOUND_SELECTOR_H_
#define CSSOM_COMPOUND_SELECTOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "cobalt/cssom/selector.h"

namespace cobalt {
namespace cssom {

// A compound selector is a chain of simple selectors that are not separated by
// a combinator.
//   http://www.w3.org/TR/selectors4/#compound
class CompoundSelector : public Selector {
 public:
  CompoundSelector() {}
  ~CompoundSelector() OVERRIDE {}

  void Accept(SelectorVisitor* visitor) OVERRIDE;

  void push_back(Selector* selector) { selectors_.push_back(selector); }

  const Selectors& selectors() { return selectors_; }

 private:
  Selectors selectors_;
  DISALLOW_COPY_AND_ASSIGN(CompoundSelector);
};

}  // namespace cssom
}  // namespace cobalt

#endif  // CSSOM_COMPOUND_SELECTOR_H_
