#pragma once

#include "component/position.pb.h"

namespace pack::component {

class Positions final {
 public:
  static Position as_packed(const Position& position);
  static Position as_floats(const Position& position);
};

}  // namespace pack::component
