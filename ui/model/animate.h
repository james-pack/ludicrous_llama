#pragma once

#include <functional>

#include "position/position.h"

namespace pack::ui::model {

class Gear;

using Animate = std::function<void(double seconds, Gear& component, position::Position& position,
                                   position::Orientation& orientation)>;

}  // namespace pack::ui::model
