#pragma once

#include <string>

#include "material/proto/material.pb.h"
#include "ui/model/color.h"

namespace pack::ui::model {

struct Material final {
  std::string id{};
  Color ambient{};
  Color diffuse{};
  Color specular{};

  // For compatibility with OpenGL, shininess is on [0, 128].
  float shininess{0.f};

  static void from_proto(const material::proto::Material& proto, Material* material);
  static Material from_proto(const material::proto::Material& proto);
  static void to_proto(const Material& material, material::proto::Material* proto);
  static material::proto::Material to_proto(const Material& material);
};

}  // namespace pack::ui::model
