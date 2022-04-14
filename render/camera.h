#pragma once

#include <string>
#include <vector>

#include "position/position.h"

namespace pack::render {

struct Camera final {
  using Container = std::vector<Camera>;

  std::string name{};
  position::Position position{};
  position::Orientation orientation{};

  bool operator==(const Camera& rhs) const {
    return name == rhs.name && position == rhs.position && orientation == rhs.orientation;
  }
};

std::string to_string(const Camera& camera);

}  // namespace pack::render
