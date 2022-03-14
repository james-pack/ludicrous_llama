#pragma once

#include <functional>

#include "ui/model/position.h"

namespace pack::ui::model {

class Gear;

using Animate = std::function<void(double seconds, Gear& component, Position& position, Orientation& orientation)>;

}  // namespace pack::ui::model
