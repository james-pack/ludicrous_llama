#pragma once

#include <string>

#include "lighting/proto/light.pb.h"
#include "position/position.h"
#include "ui/model/color.h"

namespace pack::ui::model {

struct Light final {
  std::string id{};
  std::string name{};
  int light_num{-1};  // Used to identify this light to OpenGL when using glLight() and similar calls.
  Color ambient{{1.f, 1.f, 1.f, 1.f}};
  Color diffuse{{1.f, 1.f, 1.f, 1.f}};
  Color specular{{1.f, 1.f, 1.f, 1.f}};
  bool enabled{};

  static void from_proto(const lighting::proto::Light& proto, Light* light, position::Position* position,
                         position::Orientation* orientation);
  static void to_proto(const Light& light, const position::Position& position, const position::Orientation& orientation,
                       lighting::proto::Light* proto);
  static lighting::proto::Light to_proto(const Light& light, const position::Position& position,
                                         const position::Orientation& orientation);
};

std::string to_string(const Light& light);

}  // namespace pack::ui::model
