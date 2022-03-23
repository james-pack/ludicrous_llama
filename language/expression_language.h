#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <strstream>

#include "tao/pegtl/contrib/parse_tree.hpp"
#include "tao/pegtl/contrib/parse_tree_to_dot.hpp"

namespace pack::language {

// Forward declaration of parsed types that are part of the API.
struct integer_value;
struct float_value;

std::string to_string(const tao::pegtl::parse_tree::node& node) {
  std::strstream ss{};
  tao::pegtl::parse_tree::print_dot(ss, node);
  return ss.str();
}

class ExpressionLanguage final {
 public:
  static std::unique_ptr<tao::pegtl::parse_tree::node> parse(std::string_view expression);
};

}  // namespace pack::language
