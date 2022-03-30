#pragma once

#include <limits>

#include "material/proto/color.pb.h"

namespace pack::material {

class Colors final {
 public:
  static proto::Rgba as_floats(const proto::Rgba& color);
  static proto::Rgba as_ints(const proto::Rgba& color);
  static proto::Rgba as_uint32s(const proto::Rgba& color);

  static void to_array(const proto::Rgba& color, float* color_array);
  static void from_array(const float* color_array, proto::Rgba* color);

  static proto::Rgba create(float red, float green, float blue, float alpha = 1.f) {
    proto::Rgba result{};
    result.mutable_float_values()->set_red(red);
    result.mutable_float_values()->set_green(green);
    result.mutable_float_values()->set_blue(blue);
    result.mutable_float_values()->set_alpha(alpha);
    return result;
  }

  static proto::Rgba create(int red, int green, int blue, int alpha = 255) {
    proto::Rgba result{};
    result.mutable_int_values()->set_red(red);
    result.mutable_int_values()->set_green(green);
    result.mutable_int_values()->set_blue(blue);
    result.mutable_int_values()->set_alpha(alpha);
    return result;
  }

  static proto::Rgba create(uint32_t red, uint32_t green, uint32_t blue,
                            uint32_t alpha = std::numeric_limits<uint32_t>::max()) {
    proto::Rgba result{};
    result.mutable_uint32_values()->set_red(red);
    result.mutable_uint32_values()->set_green(green);
    result.mutable_uint32_values()->set_blue(blue);
    result.mutable_uint32_values()->set_alpha(alpha);
    return result;
  }
};

}  // namespace pack::material
