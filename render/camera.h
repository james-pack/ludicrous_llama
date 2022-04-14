#pragma once

#include <string>
#include <vector>

#include "position/position.h"

namespace pack::render {

struct Camera final {
  using Container = std::vector<Camera>;

  position::Position position{};
  position::Orientation orientation{};

  bool operator==(const Camera& rhs) const { return position == rhs.position && orientation == rhs.orientation; }
};

std::string to_string(const Camera& camera);

}  // namespace pack::render
