#pragma once

#include <string>

#include "serialization/position.pb.h"

namespace pack::ui::model {

struct Orientation final {
  // Orientation in 3D space as Euler angles in the order of [rot_x, rot_y, rot_z].
  float orientation[3]{};

  static Orientation from_proto(const serialization::Orientation& proto);
  static void from_proto(const serialization::Orientation& proto, Orientation* orientation);
  static serialization::Orientation to_proto(const Orientation& orientation);
  static void to_proto(const Orientation& orientation, serialization::Orientation* proto);
};

std::string to_string(const Orientation& orientation);

struct Position final {
  // Position in 4D space (homogeneous object coordinates) as an array in the order [x, y, z, w].
  float position[4]{};

  static Position from_proto(const serialization::Position& proto);
  static void from_proto(const serialization::Position& proto, Position* position);
  static serialization::Position to_proto(const Position& position);
  static void to_proto(const Position& position, serialization::Position* proto);
};

std::string to_string(const Position& position);

}  // namespace pack::ui::model
