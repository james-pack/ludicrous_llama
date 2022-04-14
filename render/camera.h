#pragma once

#include "position/position.h"

namespace pack::render {

struct Camera final {
  position::Position position{};
  position::Orientation orientation{};
};

}  // namespace pack::render
