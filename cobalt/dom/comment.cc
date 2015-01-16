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

#include "cobalt/dom/comment.h"

namespace cobalt {
namespace dom {

// static
scoped_refptr<Comment> Comment::Create(const base::StringPiece& comment) {
  return make_scoped_refptr(new Comment(comment));
}

Comment::Comment(const base::StringPiece& comment)
    : comment_(comment.begin(), comment.end()) {}

bool Comment::CheckAcceptAsChild(const scoped_refptr<Node>& child) const {
  // Can't attach children nodes to a comment node.
  return false;
}

void Comment::Accept(NodeVisitor* visitor) { visitor->Visit(this); }

void Comment::Accept(ConstNodeVisitor* visitor) const { visitor->Visit(this); }

}  // namespace dom
}  // namespace cobalt
