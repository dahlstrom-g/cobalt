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

#ifndef CSS_PARSER_GRAMMAR_H_
#define CSS_PARSER_GRAMMAR_H_

// A wrapper around grammar_generated.h, the latter should never be included
// directly.
//
// This file confines violations of a C++ style guide caused by Bison to a
// single place. Bison does not generate header guards nor it allows to specify
// a namespace for generated code. This causes three problems:
//
//   - files generated by Bison are not complete and cannot be included in
//     the same way as headers that comply with style guide;
//
//   - files generated by Bison should be included inside our namespace
//     to avoid a polution of a global namespace;
//
//   - as a corollary to the previous problem, header files that contain
//     namespaces cannot be included from grammar (.y) as this will cause double
//     nesting of included classes.

#include "cobalt/css_parser/trivial_string_piece.h"
#include "cobalt/cssom/compound_selector.h"
#include "cobalt/cssom/css_style_declaration.h"
#include "cobalt/cssom/css_style_rule.h"
#include "cobalt/cssom/css_style_sheet.h"
#include "cobalt/cssom/length_value.h"
#include "cobalt/cssom/number_value.h"
#include "cobalt/cssom/selector.h"
#include "cobalt/cssom/transform_function.h"
#include "cobalt/cssom/transform_list_value.h"

namespace cobalt {
namespace css_parser {

class ParserImpl;
struct PropertyDeclaration;

// A header generated by Bison must be included inside our namespace
// to avoid global namespace pollution.
#include "cobalt/css_parser/grammar_generated.h"

}  // namespace css_parser
}  // namespace cobalt

#endif  // CSS_PARSER_GRAMMAR_H_
