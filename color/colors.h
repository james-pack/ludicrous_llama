#pragma once

#include <limits>

#include "color/color.pb.h"

namespace pack::color {

class Colors final {
 public:
  static Rgba as_floats(const Rgba& color);
  static Rgba as_ints(const Rgba& color);
  static Rgba as_uint32s(const Rgba& color);

  static void to_array(const Rgba& color, float* color_array);
  static void from_array(const float* color_array, Rgba* color);

  static Rgba create(float red, float green, float blue, float alpha = 1.f) {
    Rgba result{};
    result.mutable_float_values()->set_red(red);
    result.mutable_float_values()->set_green(green);
    result.mutable_float_values()->set_blue(blue);
    result.mutable_float_values()->set_alpha(alpha);
    return result;
  }

  static Rgba create(int red, int green, int blue, int alpha = 255) {
    Rgba result{};
    result.mutable_int_values()->set_red(red);
    result.mutable_int_values()->set_green(green);
    result.mutable_int_values()->set_blue(blue);
    result.mutable_int_values()->set_alpha(alpha);
    return result;
  }

  static Rgba create(uint32_t red, uint32_t green, uint32_t blue,
                     uint32_t alpha = std::numeric_limits<uint32_t>::max()) {
    Rgba result{};
    result.mutable_uint32_values()->set_red(red);
    result.mutable_uint32_values()->set_green(green);
    result.mutable_uint32_values()->set_blue(blue);
    result.mutable_uint32_values()->set_alpha(alpha);
    return result;
  }
};

}  // namespace pack::color
