#pragma once

#include <string>

#include "serialization/color.pb.h"

namespace pack::ui::model {

struct Color final {
  // The color is packed as four 32-bit signed integers. The exact encoding is currently platform-dependent. This type
  // matches the memory layout that OpenGL wants for its colors in its glMaterialiv() -- int32[4]. That function, and
  // other material creation functions, only take colors by reference; this message layout is designed to own the
  // memory for the color in order to make it easier to create materials.
  float values[4]{};

  static Color from_proto(const serialization::Rgba& proto);
  static void from_proto(const serialization::Rgba& proto, Color* color);
  static serialization::Rgba to_proto(const Color& color);
  static void to_proto(const Color& color, serialization::Rgba* proto);
};

std::string to_string(const Color& color);

}  // namespace pack::ui::model
