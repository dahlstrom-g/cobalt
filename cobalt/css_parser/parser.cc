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

#include "cobalt/css_parser/parser.h"

#include <cstdio>
#include <sstream>

#include "base/bind.h"
#include "base/string_util.h"
#include "cobalt/css_parser/grammar.h"
#include "cobalt/css_parser/property_declaration.h"
#include "cobalt/css_parser/ref_counted_util.h"
#include "cobalt/css_parser/scanner.h"
#include "cobalt/css_parser/string_pool.h"
#include "cobalt/css_parser/trivial_string_piece.h"
#include "cobalt/cssom/class_selector.h"
#include "cobalt/cssom/css_rule_list.h"
#include "cobalt/cssom/css_style_rule.h"
#include "cobalt/cssom/css_style_sheet.h"
#include "cobalt/cssom/font_weight_value.h"
#include "cobalt/cssom/id_selector.h"
#include "cobalt/cssom/keyword_names.h"
#include "cobalt/cssom/keyword_value.h"
#include "cobalt/cssom/length_value.h"
#include "cobalt/cssom/number_value.h"
#include "cobalt/cssom/property_names.h"
#include "cobalt/cssom/rgba_color_value.h"
#include "cobalt/cssom/scale_function.h"
#include "cobalt/cssom/string_value.h"
#include "cobalt/cssom/translate_function.h"
#include "cobalt/cssom/type_selector.h"

namespace cobalt {
namespace css_parser {

namespace {

uint32_t ParseHexToken(const TrivialStringPiece& string_piece) {
  char* value_end(const_cast<char*>(string_piece.end));
  uint32_t integer = std::strtoul(string_piece.begin, &value_end, 16);
  DCHECK_EQ(value_end, string_piece.end);
  return integer;
}

template <typename Value>
Value ClampToRange(Value min_value, Value max_value, Value value) {
  return std::min(max_value, std::max(min_value, value));
}

}  // namespace

// This class not only hides details of implementation of the parser but also
// provides a low-level API used by semantic actions in grammar.y.
class ParserImpl {
 public:
  ParserImpl(const std::string& input,
             const base::SourceLocation& input_location,
             const Parser::OnMessageCallback& on_warning_callback,
             const Parser::OnMessageCallback& on_error_callback);

  scoped_refptr<cssom::CSSStyleSheet> ParseStyleSheet();
  scoped_refptr<cssom::PropertyValue> ParsePropertyValue(
      const std::string& property_name);

  Scanner& scanner() { return scanner_; }

  void set_last_syntax_error_location(
      const YYLTYPE& last_syntax_error_location) {
    last_syntax_error_location_ = last_syntax_error_location;
  }

  void LogWarning(const YYLTYPE& source_location, const std::string& message);
  void LogError(const YYLTYPE& source_location, const std::string& message);

  cssom::CSSStyleSheet& style_sheet() const { return *style_sheet_; }
  void set_property_value(
      const scoped_refptr<cssom::PropertyValue>& property_value) {
    property_value_ = property_value;
  }

 private:
  bool Parse();

  std::string FormatMessage(const std::string& message_type,
                            const YYLTYPE& source_location,
                            const std::string& message);

  const std::string input_;
  const base::SourceLocation input_location_;
  const Parser::OnMessageCallback on_warning_callback_;
  const Parser::OnMessageCallback on_error_callback_;

  StringPool string_pool_;
  Scanner scanner_;
  YYLTYPE last_syntax_error_location_;

  // Parsing results, only one of them may be non-NULL.
  scoped_refptr<cssom::CSSStyleSheet> style_sheet_;
  scoped_refptr<cssom::PropertyValue> property_value_;

  friend int yyparse(ParserImpl* parser_impl);
};

ParserImpl::ParserImpl(const std::string& input,
                       const base::SourceLocation& input_location,
                       const Parser::OnMessageCallback& on_warning_callback,
                       const Parser::OnMessageCallback& on_error_callback)
    : input_(input),
      input_location_(input_location),
      scanner_(input_.c_str(), &string_pool_),
      on_warning_callback_(on_warning_callback),
      on_error_callback_(on_error_callback) {}

scoped_refptr<cssom::CSSStyleSheet> ParserImpl::ParseStyleSheet() {
  style_sheet_ = new cssom::CSSStyleSheet();

  scanner_.PrependToken(kStyleSheetEntryPointToken);
  Parse();

  return style_sheet_;
}

scoped_refptr<cssom::PropertyValue> ParserImpl::ParsePropertyValue(
    const std::string& property_name) {
  Token property_name_token;
  bool known_property_name =
      scanner_.DetectPropertyNameToken(property_name, &property_name_token);
  DCHECK(known_property_name);

  scanner_.PrependToken(kPropertyValueEntryPointToken);
  scanner_.PrependToken(property_name_token);
  scanner_.PrependToken(':');
  if (!Parse()) {
    return NULL;
  }

  return property_value_;
}

void ParserImpl::LogWarning(const YYLTYPE& source_location,
                            const std::string& message) {
  on_warning_callback_.Run(FormatMessage("warning", source_location, message));
}

void ParserImpl::LogError(const YYLTYPE& source_location,
                          const std::string& message) {
  on_error_callback_.Run(FormatMessage("error", source_location, message));
}

bool ParserImpl::Parse() {
  // For more information on error codes
  // see http://www.gnu.org/software/bison/manual/html_node/Parser-Function.html
  int error_code(yyparse(this));
  switch (error_code) {
    case 0:
      // Parsed successfully or was able to recover from errors.
      return true;
    case 1:
      // Failed to recover from errors.
      LogError(last_syntax_error_location_, "unrecoverable syntax error");
      return false;
    default:
      NOTREACHED();
      return false;
  }
}

std::string ParserImpl::FormatMessage(const std::string& message_type,
                                      const YYLTYPE& source_location,
                                      const std::string& message) {
  // Adjust source location for CSS embedded in HTML.
  int line_number = source_location.first_line;
  int column_number = source_location.first_column;
  base::AdjustForStartLocation(input_location_.line_number,
                               input_location_.column_number, &line_number,
                               &column_number);

  std::stringstream message_stream;
  message_stream << input_location_.file_path << ":" << line_number << ":"
                 << column_number << ": " << message_type << ": " << message;
  return message_stream.str();
}

// This function is only used to record a location of unrecoverable
// syntax error. Most of error reporting is implemented in semantic actions
// in the grammar.
inline void yyerror(YYLTYPE* source_location, ParserImpl* parser_impl,
                    const char* /*message*/) {
  parser_impl->set_last_syntax_error_location(*source_location);
}

// TODO(***REMOVED***): Revisit after upgrading to Bison 3.
#if defined(COBALT_WIN)
#pragma warning(push)
#pragma warning(disable : 4242)  // possible loss of data
#pragma warning(disable : 4244)  // possible loss of data
#pragma warning(disable : 4365)  // signed/unsigned mismatch
#pragma warning(disable : 4702)  // unreachable code
#endif

// A header generated by Bison must be included inside our namespace
// to avoid global namespace pollution.
#include "cobalt/css_parser/grammar_impl_generated.h"

#if defined(COBALT_WIN)
#pragma warning(pop)
#endif

namespace {

void LogWarning(const std::string& message) { LOG(WARNING) << message; }

void LogError(const std::string& message) { LOG(ERROR) << message; }

}  // namespace

scoped_ptr<Parser> Parser::Create() {
  return make_scoped_ptr(
      new Parser(base::Bind(&LogWarning), base::Bind(&LogError)));
}

Parser::Parser(const OnMessageCallback& on_warning_callback,
               const OnMessageCallback& on_error_callback)
    : on_warning_callback_(on_warning_callback),
      on_error_callback_(on_error_callback) {}

Parser::~Parser() {}

scoped_refptr<cssom::CSSStyleSheet> Parser::ParseStyleSheet(
    const std::string& input, const base::SourceLocation& input_location) {
  ParserImpl parser_impl(input, input_location, on_warning_callback_,
                         on_error_callback_);
  return parser_impl.ParseStyleSheet();
}

scoped_refptr<cssom::CSSStyleDeclaration> Parser::ParseDeclarationList(
    const std::string& /*input*/,
    const base::SourceLocation& /*input_location*/) {
  // TODO(***REMOVED***): Implement this.
  NOTREACHED();
  return NULL;
}

scoped_refptr<cssom::PropertyValue> Parser::ParsePropertyValue(
    const std::string& property_name, const std::string& property_value,
    const base::SourceLocation& property_location) {
  ParserImpl parser_impl(property_value, property_location,
                         on_warning_callback_, on_error_callback_);
  return parser_impl.ParsePropertyValue(property_name);
}

}  // namespace css_parser
}  // namespace cobalt
