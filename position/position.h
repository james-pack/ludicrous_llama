#pragma once

#include <string>

#include "position/proto/position.pb.h"
#include "serialization/serialize.h"

namespace pack::position {

struct Orientation final {
  // Orientation in 3D space as Euler angles in the order of [rot_x, rot_y, rot_z].
  float orientation[3]{};

  bool operator==(const Orientation& rhs) const {
    return orientation[0] == rhs.orientation[0] &&  //
           orientation[1] == rhs.orientation[1] &&  //
           orientation[2] == rhs.orientation[2];
  }
};

std::string to_string(const Orientation& orientation);

struct Position final {
  // Position in 4D space (homogeneous object coordinates) as an array in the order [x, y, z, w].
  float position[4]{};

  bool operator==(const Position& rhs) const {
    return position[0] == rhs.position[0] &&  //
           position[1] == rhs.position[1] &&  //
           position[2] == rhs.position[2] &&  //
           position[3] == rhs.position[3];
  }
};

std::string to_string(const Position& position);

}  // namespace pack::position

namespace pack {

template <>
inline void to_proto(const position::Orientation& orientation, position::proto::Orientation* proto) {
  proto->set_rot_x(orientation.orientation[0]);
  proto->set_rot_y(orientation.orientation[1]);
  proto->set_rot_z(orientation.orientation[2]);
}

template <>
inline void from_proto(const position::proto::Orientation& proto, position::Orientation* orientation) {
  orientation->orientation[0] = proto.rot_x();
  orientation->orientation[1] = proto.rot_y();
  orientation->orientation[2] = proto.rot_z();
}

template <>
inline void to_proto(const position::Position& position, position::proto::Position* proto) {
  proto->set_x(position.position[0]);
  proto->set_y(position.position[1]);
  proto->set_z(position.position[2]);
  proto->set_w(position.position[3]);
}

template <>
inline void from_proto(const position::proto::Position& proto, position::Position* position) {
  position->position[0] = proto.x();
  position->position[1] = proto.y();
  position->position[2] = proto.z();
  position->position[3] = proto.w();
}

}  // namespace pack
