#include "language/expression_language.h"

#include <memory>
#include <string_view>

#include "tao/pegtl.hpp"
#include "tao/pegtl/contrib/analyze.hpp"
#include "tao/pegtl/contrib/parse_tree.hpp"

namespace pack::language {

using namespace tao::pegtl;

struct plus_minus : opt<one<'+', '-'>> {};
struct dot : one<'.'> {};

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

struct integer_value : seq<plus_minus, sor<integer_hexadecimal, integer_decimal>> {};
struct float_value : seq<plus_minus, sor<float_hexadecimal, float_decimal, inf, nan>> {};
struct grammar : sor<float_value, integer_value> {};

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
  static void transform(std::unique_ptr<Node>& n, States&&... st) {
    if (n->children.size() == 1) {
      n = std::move(n->children.back());
    } else {
      n->remove_content();
      auto& c = n->children;
      auto r = std::move(c.back());
      c.pop_back();
      auto o = std::move(c.back());
      c.pop_back();
      o->children.emplace_back(std::move(n));
      o->children.emplace_back(std::move(r));
      n = std::move(o);
      transform(n->children.front(), st...);
    }
  }
};

// select which rules in the grammar will produce parse tree nodes:

template <typename Rule>
using selector = parse_tree::selector<Rule,  //
                                      parse_tree::store_content::on<integer_value, float_value>,
				      parse_tree::fold_one::on<grammar>>;

std::unique_ptr<parse_tree::node> ExpressionLanguage::parse(std::string_view str) {
  memory_input in(str.data(), str.size(), str);
  return tao::pegtl::parse_tree::parse<grammar, selector, nothing, control>(in);
}

}  // namespace pack::language
