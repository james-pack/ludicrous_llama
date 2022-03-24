#pragma once

#include <string>

#include "serialization/material.pb.h"
#include "ui/model/color.h"

namespace pack::ui::model {

struct Material final {
  std::string id{};
  Color ambient{};
  Color diffuse{};
  Color specular{};

  // For compatibility with OpenGL, shininess is on [0, 128].
  float shininess{0.f};

  static void from_proto(const serialization::Material& proto, Material* material);
  static Material from_proto(const serialization::Material& proto);
  static void to_proto(const Material& material, serialization::Material* proto);
  static serialization::Material to_proto(const Material& material);
};

}  // namespace pack::ui::model
