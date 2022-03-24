#pragma once

#include <limits>

#include "serialization/color.pb.h"

namespace pack::color {

class Colors final {
 public:
  static serialization::Rgba as_floats(const serialization::Rgba& color);
  static serialization::Rgba as_ints(const serialization::Rgba& color);
  static serialization::Rgba as_uint32s(const serialization::Rgba& color);

  static void to_array(const serialization::Rgba& color, float* color_array);
  static void from_array(const float* color_array, serialization::Rgba* color);

  static serialization::Rgba create(float red, float green, float blue, float alpha = 1.f) {
    serialization::Rgba result{};
    result.mutable_float_values()->set_red(red);
    result.mutable_float_values()->set_green(green);
    result.mutable_float_values()->set_blue(blue);
    result.mutable_float_values()->set_alpha(alpha);
    return result;
  }

  static serialization::Rgba create(int red, int green, int blue, int alpha = 255) {
    serialization::Rgba result{};
    result.mutable_int_values()->set_red(red);
    result.mutable_int_values()->set_green(green);
    result.mutable_int_values()->set_blue(blue);
    result.mutable_int_values()->set_alpha(alpha);
    return result;
  }

  static serialization::Rgba create(uint32_t red, uint32_t green, uint32_t blue,
                                    uint32_t alpha = std::numeric_limits<uint32_t>::max()) {
    serialization::Rgba result{};
    result.mutable_uint32_values()->set_red(red);
    result.mutable_uint32_values()->set_green(green);
    result.mutable_uint32_values()->set_blue(blue);
    result.mutable_uint32_values()->set_alpha(alpha);
    return result;
  }
};

}  // namespace pack::color
