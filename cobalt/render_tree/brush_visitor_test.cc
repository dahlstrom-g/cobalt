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

#include "cobalt/render_tree/brush_visitor.h"

#include "cobalt/render_tree/brush.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace cobalt {
namespace render_tree {

class MockBrushVisitor : public BrushVisitor {
 public:
  MOCK_METHOD1(Visit, void(SolidColorBrush* solid_color_brush));
  MOCK_METHOD1(Visit, void(LinearGradientBrush* linear_gradient_brush));
};

TEST(BrushVisitorTest, VisitsSolidColorBrush) {
  SolidColorBrush solid_color_brush;
  MockBrushVisitor mock_visitor;
  EXPECT_CALL(mock_visitor, Visit(&solid_color_brush));
  solid_color_brush.Accept(&mock_visitor);
}

TEST(BrushVisitorTest, VisitsLinearGradientBrush) {
  LinearGradientBrush linear_gradient_brush;
  MockBrushVisitor mock_visitor;
  EXPECT_CALL(mock_visitor, Visit(&linear_gradient_brush));
  linear_gradient_brush.Accept(&mock_visitor);
}

}  // namespace render_tree
}  // namespace cobalt
