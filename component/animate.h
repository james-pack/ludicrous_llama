#pragma once

#include "component/gear.pb.h"
#include "component/position.pb.h"

namespace pack::component {

using Animate = std::function<void(double seconds, Gear& component, Position& position, Orientation& orientation)>;

}  // namespace pack::component
