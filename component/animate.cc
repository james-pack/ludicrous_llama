#include "component/animate.h"

#include <chrono>

#include "component/gear.pb.h"
#include "component/position.pb.h"
#include "entt/entity/registry.hpp"

namespace pack::component {

using Animate = std::function<void(double t, Gear& component, Position& position, Orientation& orientation)>;

void Animator::animate() const {
  // Build a duration type that will count seconds but at double precision.
  using DoubleSeconds = std::chrono::duration<double, std::chrono::seconds::period>;

  if (!is_paused_) {
    auto animations = registry_->view<Animate, Gear, Position, Orientation>();

    // We want the animations all to render as if it were the exact same time, so lock in the time once.
    double seconds = DoubleSeconds(std::chrono::steady_clock::now().time_since_epoch()).count();

    animations.each([seconds](Animate animate, Gear& component, Position& position,
                              Orientation& orientation) {  //
      animate(seconds, component, position, orientation);
    });
  }
}

}  // namespace pack::component
