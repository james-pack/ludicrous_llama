#pragma once

#include <string>

#include "component/proto/position.pb.h"

namespace pack::ui::model {

struct Orientation final {
  // Orientation in 3D space as Euler angles in the order of [rot_x, rot_y, rot_z].
  float orientation[3]{};

  static Orientation from_proto(const component::proto::Orientation& proto);
  static void from_proto(const component::proto::Orientation& proto, Orientation* orientation);
  static component::proto::Orientation to_proto(const Orientation& orientation);
  static void to_proto(const Orientation& orientation, component::proto::Orientation* proto);
};

std::string to_string(const Orientation& orientation);

struct Position final {
  // Position in 4D space (homogeneous object coordinates) as an array in the order [x, y, z, w].
  float position[4]{};

  static Position from_proto(const component::proto::Position& proto);
  static void from_proto(const component::proto::Position& proto, Position* position);
  static component::proto::Position to_proto(const Position& position);
  static void to_proto(const Position& position, component::proto::Position* proto);
};

std::string to_string(const Position& position);

}  // namespace pack::ui::model
