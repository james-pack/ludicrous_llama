#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "tao/pegtl/contrib/parse_tree.hpp"

namespace pack::language {

// Forward declaration of parsed types that are part of the API. These declarations should include all literal types and
// all operators as well as any function names, component names, and identifiers.
struct integer_literal;
struct float_literal;

struct function_call;
struct function_name;

struct component_id;
struct component_reference;

struct power;
struct multiply;
struct divide;
struct add;
struct subtract;

std::string to_string(const tao::pegtl::parse_tree::node& node);

class ExpressionLanguage final {
 public:
  static std::unique_ptr<tao::pegtl::parse_tree::node> parse(std::string_view expression);
};

}  // namespace pack::language
