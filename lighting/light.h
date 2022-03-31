#pragma once

#include <string>

#include "lighting/proto/light.pb.h"
#include "material/color.h"
#include "position/position.h"

namespace pack::lighting {

struct Light final {
  std::string id{};
  std::string name{};
  int light_num{-1};  // Used to identify this light to OpenGL when using glLight() and similar calls.
  material::Color ambient{{1.f, 1.f, 1.f, 1.f}};
  material::Color diffuse{{1.f, 1.f, 1.f, 1.f}};
  material::Color specular{{1.f, 1.f, 1.f, 1.f}};
  bool enabled{};

  static void from_proto(const proto::Light& proto, Light* light, position::Position* position,
                         position::Orientation* orientation);
  static void to_proto(const Light& light, const position::Position& position, const position::Orientation& orientation,
                       proto::Light* proto);
  static proto::Light to_proto(const Light& light, const position::Position& position,
                               const position::Orientation& orientation);
};

std::string to_string(const Light& light);

}  // namespace pack::lighting
