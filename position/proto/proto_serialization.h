#include "position/position.h"
#include "position/proto/position.pb.h"
#include "serialization/serialize.h"

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
