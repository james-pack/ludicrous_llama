#include "language/expression_language.h"

#include <memory>
#include <string>
#include <string_view>
#include <strstream>

#include "tao/pegtl.hpp"
#include "tao/pegtl/contrib/analyze.hpp"
#include "tao/pegtl/contrib/parse_tree.hpp"
#include "tao/pegtl/contrib/parse_tree_to_dot.hpp"

namespace pack::language {

using namespace tao::pegtl;

std::string to_string(const parse_tree::node& node) {
  std::ostrstream stream{};
  parse_tree::print_dot(stream, node);
  stream << std::ends;
  return stream.str();
}

struct plus_minus : opt<one<'+', '-'>> {};
struct dot : one<'.'> {};

// inf and nan may not be useful in this context. They are included for completeness but could be removed if they cause
// other parse issues.
struct inf : seq<istring<'i', 'n', 'f'>, opt<istring<'i', 'n', 'i', 't', 'y'>>> {};
struct nan : seq<istring<'n', 'a', 'n'>, opt<one<'('>, plus<alnum>, one<')'>>> {};

template <typename D>
struct float_number : seq<plus<D>, dot, star<D>> {};

template <typename D>
struct integer_number : plus<D> {};

struct decimal_exponent_suffix : one<'e', 'E'> {};
struct hexadecimal_exponent_suffix : one<'p', 'P'> {};
struct exponent : seq<plus_minus, plus<digit>> {};

struct float_decimal : seq<float_number<digit>, opt<decimal_exponent_suffix, exponent>> {};
struct float_hexadecimal
    : seq<one<'0'>, one<'x', 'X'>, float_number<xdigit>, opt<hexadecimal_exponent_suffix, exponent>> {};

struct integer_decimal : seq<integer_number<digit>, opt<decimal_exponent_suffix, exponent>> {};
struct integer_hexadecimal
    : seq<one<'0'>, one<'x', 'X'>, integer_number<xdigit>, opt<hexadecimal_exponent_suffix, exponent>> {};

struct integer_literal : seq<plus_minus, sor<integer_hexadecimal, integer_decimal>> {};
struct float_literal : seq<plus_minus, sor<float_hexadecimal, float_decimal, inf, nan>> {};

struct power : two<'*'> {};
struct add : one<'+'> {};
struct subtract : one<'-'> {};
struct multiply : one<'*'> {};
struct divide : one<'/'> {};
// This rule determines the operator priority.
struct binary_operator : sor<power, multiply, divide, add, subtract> {};

struct open_bracket : seq<one<'('>, star<space>> {};
struct close_bracket : seq<star<space>, one<')'>> {};

struct function_name : identifier {};

struct expression;

struct function_arguments : list_must<expression, one<','>, space> {};
struct function_call : seq<function_name, star<space>, open_bracket, opt<function_arguments>, close_bracket> {};

struct double_dot : two<'.'> {};
struct single_dot : one<'.'> {};

struct component_name : identifier {};
struct dots_or_name : sor<double_dot, single_dot, component_name> {};
struct component_path : list_must<dots_or_name, one<'/'>> {};
struct component_id : seq<one<'#'>, component_name> {};
struct component_id_path : seq<component_id, one<'/'>, component_path> {};
struct component_reference : sor<component_id_path, component_path> {};

struct bracketed : seq<open_bracket, expression, close_bracket> {};

struct value : sor<float_literal, integer_literal, function_call, component_reference, bracketed> {};
struct expression : list_must<value, binary_operator, space> {};

struct grammar : seq<star<space>, expression, star<space>> {};

// Using must_if<> we define a control class which is used for
// the parsing run instead of the default control class.
//
// This improves the errors reported to the user.
//
// The following turns local errors into global errors, i.e.
// if one of the rules for which a custom error message is
// defined fails, it throws a parse_error exception (aka global
// failure) instead of returning false (aka local failure).
template <typename>
inline constexpr const char* error_message = nullptr;

struct error {
  template <typename Rule>
  static constexpr auto message = error_message<Rule>;
};

template <typename Rule>
using control = must_if<error>::control<Rule>;

// Note: This rearrange class and associated comments were copied from PEGTL example code in
// src/example/pegtl/parse_tree.cpp

// Since we are going to generate a parse tree, we define a
// selector that decides which rules will be included in our
// parse tree, which rules will be omitted from the parse tree,
// and which of the nodes will store the matched content.
// Additionally, some nodes will fold when they have exactly
// one child node. (see fold_one below)

// after a node is stored successfully, you can add an optional transformer like this:
struct rearrange : parse_tree::apply<rearrange>  // allows bulk selection, see selector<...>
{
  // recursively rearrange nodes. the basic principle is:
  //
  // from:          PROD/EXPR
  //                /   |   \          (LHS... may be one or more children, followed by OP,)
  //             LHS... OP   RHS       (which is one operator, and RHS, which is a single child)
  //
  // to:               OP
  //                  /  \             (OP now has two children, the original PROD/EXPR and RHS)
  //         PROD/EXPR    RHS          (Note that PROD/EXPR has two fewer children now)
  //             |
  //            LHS...
  //
  // if only one child is left for LHS..., replace the PROD/EXPR with the child directly.
  // otherwise, perform the above transformation, then apply it recursively until LHS...
  // becomes a single child, which then replaces the parent node and the recursion ends.
  template <typename Node, typename... States>
  static void transform(std::unique_ptr<Node>& node, States&&... states) {
    if (node->children.size() == 1) {
      node = std::move(node->children.back());
    } else {
      node->remove_content();
      auto& children = node->children;
      auto rhs = std::move(children.back());
      children.pop_back();
      auto op = std::move(children.back());
      children.pop_back();
      op->children.emplace_back(std::move(node));
      op->children.emplace_back(std::move(rhs));
      node = std::move(op);
      transform(node->children.front(), states...);
    }
  }
};

template <typename Rule>
using selector = parse_tree::selector<
    Rule,  //
    parse_tree::store_content::on<integer_literal, float_literal, function_name, component_reference, component_name,
                                  double_dot, power, multiply, divide, add, subtract>,  //
    parse_tree::remove_content::on<function_call, component_id>,                        //
    rearrange::on<expression>>;

std::unique_ptr<parse_tree::node> ExpressionLanguage::parse(std::string_view str) {
  memory_input in(str.data(), str.size(), str);
  return tao::pegtl::parse_tree::parse<grammar, selector>(in);
}

}  // namespace pack::language
