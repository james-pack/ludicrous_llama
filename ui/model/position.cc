#include "ui/model/position.h"

#include <string>

#include "serialization/position.pb.h"

namespace pack::ui::model {

void Orientation::from_proto(const serialization::Orientation& proto, Orientation* orientation) {
  orientation->orientation[0] = proto.rot_x();
  orientation->orientation[1] = proto.rot_y();
  orientation->orientation[2] = proto.rot_z();
}

Orientation Orientation::from_proto(const serialization::Orientation& proto) {
  Orientation result{};
  from_proto(proto, &result);
  return result;
}

void Orientation::to_proto(const Orientation& orientation, serialization::Orientation* proto) {
  proto->set_rot_x(orientation.orientation[0]);
  proto->set_rot_y(orientation.orientation[1]);
  proto->set_rot_z(orientation.orientation[2]);
}

serialization::Orientation Orientation::to_proto(const Orientation& orientation) {
  serialization::Orientation proto{};
  to_proto(orientation, &proto);
  return proto;
}

std::string to_string(const Orientation& orientation) {
  using std::to_string;
  return "[" + to_string(orientation.orientation[0]) + ", " + to_string(orientation.orientation[1]) + ", " +
         to_string(orientation.orientation[2]) + "]";
}

void Position::from_proto(const serialization::Position& proto, Position* position) {
  position->position[0] = proto.x();
  position->position[1] = proto.y();
  position->position[2] = proto.z();
  position->position[3] = proto.w();
}

Position Position::from_proto(const serialization::Position& proto) {
  Position result{};
  from_proto(proto, &result);
  return result;
}

void Position::to_proto(const Position& position, serialization::Position* proto) {
  proto->set_x(position.position[0]);
  proto->set_y(position.position[1]);
  proto->set_z(position.position[2]);
  proto->set_w(position.position[3]);
}

serialization::Position Position::to_proto(const Position& position) {
  serialization::Position proto{};
  to_proto(position, &proto);
  return proto;
}

std::string to_string(const Position& position) {
  using std::to_string;
  return "[" + to_string(position.position[0]) + ", " + to_string(position.position[1]) + ", " +
         to_string(position.position[2]) + ", " + to_string(position.position[3]) + "]";
}

}  // namespace pack::ui::model
