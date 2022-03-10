#include "component/positions.h"

namespace pack::component {

Position Positions::as_packed(const Position& position) {
  if (position.has_packed()) {
    return position;
  }

  Position result{position};
  std::string* bytes = result.mutable_packed()->mutable_bytes();
  bytes->resize(4 * sizeof(float), 0);
  float values[4];
  switch (position.representation_case()) {
    case Position::RepresentationCase::kFloatValues: {
      values[0] = position.float_values().x();
      values[1] = position.float_values().y();
      values[2] = position.float_values().z();
      values[3] = position.float_values().w();
      break;
    }
    default:
      values[0] = 0.f;
      values[1] = 0.f;
      values[2] = 0.f;
      values[3] = 0.f;
  }
  std::memcpy(bytes->data(), values, 3 * sizeof(float));

  return result;
}

Position Positions::as_floats(const Position& position) {
  if (position.has_float_values()) {
    return position;
  }

  Position result{position};
  switch (position.representation_case()) {
    case Position::RepresentationCase::kPacked: {
      const std::string& bytes = position.packed().bytes();
      if (bytes.length() != 4 * sizeof(float)) {
        throw std::logic_error("Position in invalid format");
      }
      result.mutable_float_values()->set_x(*(reinterpret_cast<const float*>(bytes.data()) + 0));
      result.mutable_float_values()->set_y(*(reinterpret_cast<const float*>(bytes.data()) + 1));
      result.mutable_float_values()->set_z(*(reinterpret_cast<const float*>(bytes.data()) + 2));
      result.mutable_float_values()->set_w(*(reinterpret_cast<const float*>(bytes.data()) + 3));
      break;
    }
    default:
      result.mutable_float_values()->set_x(0.f);
      result.mutable_float_values()->set_y(0.f);
      result.mutable_float_values()->set_z(0.f);
      result.mutable_float_values()->set_w(0.f);
  }

  return result;
}

}  // namespace pack::component
