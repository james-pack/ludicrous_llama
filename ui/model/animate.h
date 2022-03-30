#pragma once

#include <functional>

#include "component/position.h"

namespace pack::ui::model {

class Gear;

using Animate = std::function<void(double seconds, Gear& component, component::Position& position,
                                   component::Orientation& orientation)>;

}  // namespace pack::ui::model
