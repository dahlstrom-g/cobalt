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

#include "cobalt/cssom/selector_visitor.h"

#include "cobalt/cssom/class_selector.h"
#include "cobalt/cssom/compound_selector.h"
#include "cobalt/cssom/empty_pseudo_class.h"
#include "cobalt/cssom/id_selector.h"
#include "cobalt/cssom/type_selector.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace cobalt {
namespace cssom {

class MockSelectorVisitor : public SelectorVisitor {
 public:
  MOCK_METHOD1(VisitClassSelector, void(ClassSelector* class_selector));
  MOCK_METHOD1(VisitEmptyPseudoClass,
               void(EmptyPseudoClass* empty_pseudo_class));
  MOCK_METHOD1(VisitIdSelector, void(IdSelector* id_selector));
  MOCK_METHOD1(VisitTypeSelector, void(TypeSelector* type_selector));

  MOCK_METHOD1(VisitCompoundSelector,
               void(CompoundSelector* compound_selector));
};

TEST(SelectorVisitorTest, VisitsClassSelector) {
  ClassSelector class_selector("my-class");
  MockSelectorVisitor mock_visitor;
  EXPECT_CALL(mock_visitor, VisitClassSelector(&class_selector));
  class_selector.Accept(&mock_visitor);
}

TEST(SelectorVisitorTest, VisitsEmptyPseudoClass) {
  EmptyPseudoClass empty_pseudo_class;
  MockSelectorVisitor mock_visitor;
  EXPECT_CALL(mock_visitor, VisitEmptyPseudoClass(&empty_pseudo_class));
  empty_pseudo_class.Accept(&mock_visitor);
}

TEST(SelectorVisitorTest, VisitsIdSelector) {
  IdSelector id_selector("id");
  MockSelectorVisitor mock_visitor;
  EXPECT_CALL(mock_visitor, VisitIdSelector(&id_selector));
  id_selector.Accept(&mock_visitor);
}

TEST(SelectorVisitorTest, VisitsTypeSelector) {
  TypeSelector type_selector("div");
  MockSelectorVisitor mock_visitor;
  EXPECT_CALL(mock_visitor, VisitTypeSelector(&type_selector));
  type_selector.Accept(&mock_visitor);
}

TEST(SelectorVisitorTest, VisitCompoundSelector) {
  CompoundSelector compound_selector;
  MockSelectorVisitor mock_visitor;
  EXPECT_CALL(mock_visitor, VisitCompoundSelector(&compound_selector));
  compound_selector.Accept(&mock_visitor);
}

}  // namespace cssom
}  // namespace cobalt
