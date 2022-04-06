#pragma once

#include <string>

namespace pack::material {

struct Color final {
  // The color is packed as four 32-bit float values. The exact encoding is currently platform-dependent. This type
  // matches the memory layout that OpenGL wants for its colors in glMaterialfv(). That function, and
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
