#pragma once

#include <set>
#include <string>
#include <variant>

#include "component/ordering.h"

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

// The index of a type in the variant type list corresponds to the C++ type of the enum value in Type.
using Value = std::variant<std::monostate, float, int64_t, std::string, std::string>;
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

using Expression = std::variant<std::string, Value>;
inline std::string& as_expression(Expression& expr) {
  if (expr.index() != 0) {
    expr.emplace<0>();
  }
  return std::get<0>(expr);
}
inline const std::string& as_expression(const Expression& expr) { return std::get<0>(expr); }

inline Value& as_literal(Expression& expr) {
  if (expr.index() != 1) {
    expr.emplace<1>();
  }
  return std::get<1>(expr);
}
inline const Value& as_literal(const Expression& expr) { return std::get<1>(expr); }

struct Precision final {
  // There are many ways of determining precision. For now, we focus on the simplest form, number of decimal places, but
  // should expand this to handle different concepts as they arise.
  int32_t num_decimal_places{-1};
};

struct ValueDomain final {
  Type type{};

  // Use when the domain is best modelled as a min value, max value and a precision. Note that none of the three are
  // required and reasonable defaults should be used when interpreting this message. The type, values and precision
  // should agree.
  Value min_value{};
  Value max_value{};
  Precision precision{};
};

// A Parameter is a value provided to a primitive, component, process, etc. that allows for variable behavior. It is
// similar to a parameter to a function.
struct Parameter final {
  using Set = std::set<Parameter, OrderByNameField<Parameter>>;

  std::string name{};

  ValueDomain domain{};
  Value default_value{};
};

struct ParameterBinding final {
  using Set = std::set<ParameterBinding, OrderByNameField<ParameterBinding>>;

  // Name of the parameter being bound.
  std::string name{};

  // Value it is bound to.
  Expression value{};
};

struct Property final {
  using Set = std::set<Property, OrderByNameField<Property>>;

  std::string name{};
  Expression value{};
};

}  // namespace pack::component
