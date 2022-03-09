#pragma once

#include "color/color.pb.h"

namespace pack::color {

class Colors final {
 public:
  static Rgba as_packed(const Rgba& color);
  static Rgba as_floats(const Rgba& color);
  static Rgba as_ints(const Rgba& color);
  static Rgba as_uint32s(const Rgba& color);
};

}  // namespace pack::color
