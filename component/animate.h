#pragma once

#include "component/gear.pb.h"
#include "component/position.pb.h"
#include "entt/entity/registry.hpp"

namespace pack::component {

using Animate = std::function<void(double seconds, Gear& component, Position& position, Orientation& orientation)>;

class Animator final {
 private:
  entt::registry* registry_;
  bool is_paused_{false};

 public:
  Animator(entt::registry& registry) : registry_(&registry) {}

  void animate() const;

  bool is_paused() const { return is_paused_; }

  void pause() { is_paused_ = true; }

  void unpause() { is_paused_ = false; }

  void toggle_pause() {
    if (is_paused_) {
      is_paused_ = false;
    } else {
      is_paused_ = true;
    }
  }
};

}  // namespace pack::component
