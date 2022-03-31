#pragma once

#include <string>

#include "material/colors.h"
#include "material/proto/color.pb.h"
#include "serialization/serialize.h"

namespace pack::material {

struct Color final {
  // The color is packed as four 32-bit signed integers. The exact encoding is currently platform-dependent. This type
  // matches the memory layout that OpenGL wants for its colors in its glMaterialiv() -- int32[4]. That function, and
  // other material creation functions, only take colors by reference; this message layout is designed to own the
  // memory for the color in order to make it easier to create materials.
  float values[4]{};

  bool operator==(const Color& rhs) const {
    return values[0] == rhs.values[0] &&  //
           values[1] == rhs.values[1] &&  //
           values[2] == rhs.values[2] &&  //
           values[3] == rhs.values[3];
  }
};

std::string to_string(const Color& color);

}  // namespace pack::material

namespace pack {

template <>
inline void to_proto(const material::Color& color, material::proto::Rgba* proto) {
  material::Colors::from_array(color.values, proto);
}

template <>
inline void from_proto(const material::proto::Rgba& proto, material::Color* color) {
  material::Colors::to_array(proto, color->values);
}

}  // namespace pack
