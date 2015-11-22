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

#ifndef CSSOM_ANIMATION_SET_H_
#define CSSOM_ANIMATION_SET_H_

#include <map>
#include <string>

#include "cobalt/cssom/animation.h"
#include "cobalt/cssom/css_style_declaration_data.h"

namespace cobalt {
namespace cssom {

// Manages a set of CSS Animation instantiations applied to a specific
// element (either an HTML Element or a Pseudo Element).  The
// AnimationSet::Update() method should be called to update the AnimationSet's
// internal animations based on the new computed style.  As AnimationSet
// internally creates and destroys a set of animations, it generates
// EventHandler::OnAnimationStarted() and EventHandler::OnAnimationRemoved()
// events.
class AnimationSet {
 public:
  class EventHandler {
   public:
    virtual void OnAnimationStarted(const Animation& animation) = 0;
    virtual void OnAnimationRemoved(const Animation& animation) = 0;
  };

  // Create an Animation with the specified EventHandler, whose methods will
  // be called when Animation::Update() is called.
  explicit AnimationSet(EventHandler* event_handler);

  // Update the internal state of animations based on our previous state and the
  // passed in computed style.  Animations may be started or ended when this
  // method is called.
  void Update(const base::TimeDelta& current_time,
              const CSSStyleDeclarationData& style,
              const cssom::CSSKeyframesRule::NameMap& keyframes_map);

 private:
  // Our internal collection of animations, mapping 'animation-name' to
  // a cssom::Animation object.
  typedef std::map<std::string, Animation> InternalAnimationMap;

  EventHandler* event_handler_;
  InternalAnimationMap animations_;

  DISALLOW_COPY_AND_ASSIGN(AnimationSet);
};

}  // namespace cssom
}  // namespace cobalt

#endif  // CSSOM_ANIMATION_SET_H_
