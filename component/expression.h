#pragma once

#include <stdexcept>
#include <string>
#include <variant>

#include "component/value.h"

namespace pack::component {

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

inline bool operator==(const Expression& lhs, const Expression& rhs) {
  using std::to_string;

  if (lhs.index() != rhs.index()) {
    return false;
  }

  switch (lhs.index()) {
    case 0:
      return as_expression(lhs) == as_expression(rhs);
    case 1:
      return as_literal(lhs) == as_literal(rhs);
    default:
      throw std::invalid_argument("Unknown expression storage index '" + to_string(lhs.index()) + "'");
  }
}

std::string to_string(const Expression& expr);

}  // namespace pack::component
