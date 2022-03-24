#pragma once

#include <string>

#include "serialization/light.pb.h"
#include "ui/model/color.h"
#include "ui/model/position.h"

namespace pack::ui::model {

struct Light final {
  std::string id{};
  std::string name{};
  int light_num{-1};  // Used to identify this light to OpenGL when using glLight() and similar calls.
  Color ambient{{1.f, 1.f, 1.f, 1.f}};
  Color diffuse{{1.f, 1.f, 1.f, 1.f}};
  Color specular{{1.f, 1.f, 1.f, 1.f}};
  bool enabled{};

  static void from_proto(const serialization::Light& proto, Light* light, Position* position, Orientation* orientation);
  static void to_proto(const Light& light, const Position& position, const Orientation& orientation,
                       serialization::Light* proto);
  static serialization::Light to_proto(const Light& light, const Position& position, const Orientation& orientation);
};

std::string to_string(const Light& light);

}  // namespace pack::ui::model
