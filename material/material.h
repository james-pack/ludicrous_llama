#pragma once

#include <string>

#include "material/color.h"

namespace pack::material {

struct Material final {
  std::string id{};
  Color ambient{};
  Color diffuse{};
  Color specular{};

  // For compatibility with OpenGL, shininess is on [0, 128].
  float shininess{0.f};

  bool operator==(const Material& rhs) const {
    return id == rhs.id &&              //
           ambient == rhs.ambient &&    //
           diffuse == rhs.diffuse &&    //
           specular == rhs.specular &&  //
           shininess == rhs.shininess;
  }
};

std::string to_string(const Material& material);

}  // namespace pack::material
