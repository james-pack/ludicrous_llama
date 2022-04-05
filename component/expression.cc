#include "component/expression.h"

#include <stdexcept>
#include <string>

#include "component/value.h"

namespace pack::component {

std::string to_string(const Expression& expr) {
  using std::to_string;

  size_t index{expr.index()};
  switch (index) {
    case 0:
      return "'" + as_expression(expr) + "' (expression)";
    case 1:
      return pack::component::to_string(as_literal(expr)) + " (literal)";
    default:
      throw std::domain_error("Index '" + to_string(index) + "' not implemented in to_string(Expression)");
  }
}

}  // namespace pack::component
