#pragma once

#include <stdexcept>
#include <string>
#include <variant>

#include "component/proto/component.pb.h"
#include "glog/logging.h"
#include "serialization/serialize.h"

namespace pack::component {

enum class Type : uint8_t {
  UNTYPED = 0,  // Exists to force a type to be explicitly chosen, since this is the default value.
  FLOAT = 1,
  INTEGER = 2,
  STRING = 3,
  ID = 4,
  // Additional types to consider adding: SET, SET_MEMBER, POINT, DIRECTION, and LINE.
};
inline constexpr auto as_index(Type type) { return static_cast<std::underlying_type_t<Type>>(type); }
std::string to_string(Type type);

// The index of a type in the variant type list corresponds to the C++ type of the enum value in Type.
using Value = std::variant<std::monostate, float, int64_t, std::string, std::string>;
inline Type get_type(const Value& value) { return static_cast<Type>(value.index()); }

inline float& as_float(Value& value) {
  if (value.index() != as_index(Type::FLOAT)) {
    value.emplace<as_index(Type::FLOAT)>();
  }
  return std::get<as_index(Type::FLOAT)>(value);
}
inline const float& as_float(const Value& value) { return std::get<as_index(Type::FLOAT)>(value); }

inline int64_t& as_integer(Value& value) {
  if (value.index() != as_index(Type::INTEGER)) {
    value.emplace<as_index(Type::INTEGER)>();
  }
  return std::get<as_index(Type::INTEGER)>(value);
}
inline const int64_t& as_integer(const Value& value) { return std::get<as_index(Type::INTEGER)>(value); }

inline std::string& as_string(Value& value) {
  if (value.index() != as_index(Type::STRING)) {
    value.emplace<as_index(Type::STRING)>();
  }
  return std::get<as_index(Type::STRING)>(value);
}
inline const std::string& as_string(const Value& value) { return std::get<as_index(Type::STRING)>(value); }

inline std::string& as_id(Value& value) {
  if (value.index() != as_index(Type::ID)) {
    value.emplace<as_index(Type::ID)>();
  }
  return std::get<as_index(Type::ID)>(value);
}
inline const std::string& as_id(const Value& value) { return std::get<as_index(Type::ID)>(value); }

inline bool operator==(const Value& lhs, const Value& rhs) {
  using std::to_string;

  Type lhs_type = get_type(lhs);
  Type rhs_type = get_type(rhs);

  // TODO(james): Modify to handle type promotion.
  if (lhs_type != rhs_type) {
    return false;
  }

  switch (lhs_type) {
    case Type::UNTYPED:
      // rhs has the same type per check above.
      return true;
    case Type::FLOAT:
      return as_float(lhs) == as_float(rhs);
    case Type::INTEGER:
      return as_integer(lhs) == as_integer(rhs);
    case Type::STRING:
      return as_string(lhs) == as_string(rhs);
    case Type::ID:
      return as_id(lhs) == as_id(rhs);
    default:
      throw std::invalid_argument("Unknown Type '" + to_string(as_index(lhs_type)) + "'");
  }
}

std::string to_string(const Value& value);

}  // namespace pack::component

namespace pack {

template <>
inline void to_proto(const component::Value& value, component::proto::Value* proto) {
  using std::to_string;
  proto->mutable_type()->set_type(static_cast<component::proto::Type::Types>(value.index()));
  switch (static_cast<component::Type>(value.index())) {
    case component::Type::UNTYPED:
      break;
    case component::Type::FLOAT:
      proto->set_float_value(component::as_float(value));
      break;
    case component::Type::INTEGER:
      proto->set_integer_value(component::as_integer(value));
      break;
    case component::Type::STRING:
      proto->set_string_value(component::as_string(value));
      break;
    case component::Type::ID:
      proto->set_id(component::as_id(value));
      break;
    default:
      throw std::invalid_argument("Unknown Type '" + to_string(value.index()) + "'");
  }
}

template <>
inline void from_proto(const component::proto::Value& proto, component::Value* value) {
  using std::to_string;
  component::Type result_type = static_cast<component::Type>(proto.type().type());
  switch (result_type) {
    case component::Type::UNTYPED:
      value->emplace<component::as_index(component::Type::UNTYPED)>();
      break;
    case component::Type::FLOAT:
      value->emplace<component::as_index(component::Type::FLOAT)>(proto.float_value());
      break;
    case component::Type::INTEGER:
      value->emplace<component::as_index(component::Type::INTEGER)>(proto.integer_value());
      break;
    case component::Type::STRING:
      value->emplace<component::as_index(component::Type::STRING)>(proto.string_value());
      break;
    case component::Type::ID:
      value->emplace<component::as_index(component::Type::ID)>(proto.id());
      break;
    default:
      throw std::invalid_argument("Unknown Type '" + to_string(component::as_index(result_type)) + "'");
  }
}

}  // namespace pack
