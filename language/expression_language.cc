#include "language/expression_language.h"

#include <cmath>
#include <functional>
#include <map>
#include <sstream>
#include <string>
#include <string_view>

#include "language/parse_tree.h"
#include "tao/pegtl.hpp"

// Include the analyze function that checks
// a grammar for possible infinite cycles.
#include "tao/pegtl/contrib/analyze.hpp"

namespace pack::language {

using namespace tao::pegtl;

struct Operators {
  Operators() {
    // By default we initialise with all binary operators from the C language that can be
    // used on integers, all with their usual priority.

    insert("**", OperatorPriority(4), [](const double l, const double r) { return std::pow(l, r); });
    insert("*", OperatorPriority(5), [](const double l, const double r) { return l * r; });
    insert("/", OperatorPriority(5), [](const double l, const double r) { return l / r; });
    // insert( "%", OperatorPriority(5), []( const double l, const double r ) { return l % r; } );
    insert("+", OperatorPriority(6), [](const double l, const double r) { return l + r; });
    insert("-", OperatorPriority(6), [](const double l, const double r) { return l - r; });
    // insert( "<<", OperatorPriority(7), []( const double l, const double r ) { return l << r; } );
    // insert( ">>", OperatorPriority(7), []( const double l, const double r ) { return l >> r; } );
    insert("<", OperatorPriority(8), [](const double l, const double r) { return l < r; });
    insert(">", OperatorPriority(8), [](const double l, const double r) { return l > r; });
    insert("<=", OperatorPriority(8), [](const double l, const double r) { return l <= r; });
    insert(">=", OperatorPriority(8), [](const double l, const double r) { return l >= r; });
    insert("==", OperatorPriority(9), [](const double l, const double r) { return l == r; });
    insert("!=", OperatorPriority(9), [](const double l, const double r) { return l != r; });
    // insert( "&", OperatorPriority(10), []( const double l, const double r ) { return l & r; } );
    // insert( "^", OperatorPriority(11), []( const double l, const double r ) { return l ^ r; } );
    // insert( "|", OperatorPriority(12), []( const double l, const double r ) { return l | r; } );
    // insert( "&&", OperatorPriority(13), []( const double l, const double r ) { return ( ( l != 0 ) && ( r != 0 ) )
    // ? 1 : 0; }
    // ); insert( "||", OperatorPriority(14), []( const double l, const double r ) { return ( ( l != 0 ) || ( r != 0 )
    // ) ? 1 : 0; } );
  }

  // Arbitrary user-defined operators can be added at runtime.

  void insert(const std::string& name, const OperatorPriority p, const std::function<double(double, double)>& f) {
    assert(!name.empty());
    m_ops.try_emplace(name, Operator{p, f});
  }

  [[nodiscard]] const std::map<std::string, Operator>& ops() const noexcept { return m_ops; }

 private:
  std::map<std::string, Operator> m_ops;
};

// Comments are introduced by a '#' and proceed to the end-of-line/file.

struct comment : seq<one<'#'>, until<eolf>> {};

// The calculator ignores all spaces and comments; space is a pegtl rule
// that matches the usual ascii characters ' ', '\t', '\n' etc. In other
// words, everything that is space or a comment is ignored.

struct ignored : sor<space, comment> {};

// Since the binary operators are taken from a runtime data structure
// (rather than hard-coding them into the grammar), we need a custom
// rule that attempts to match the input against the current map of
// operators.
struct infix {
  using rule_t = ascii::any::rule_t;

  template <apply_mode, rewind_mode, template <typename...> class Action, template <typename...> class Control,
            typename ParseInput, typename... States>
  static bool match(ParseInput& in, const Operators& b, ParseStack& s, States&&... /*unused*/) {
    // Look for the longest match of the input against the operators in the operator map.

    return match(in, b, s, std::string());
  }

 private:
  template <typename ParseInput>
  static bool match(ParseInput& in, const Operators& b, ParseStack& s, std::string t) {
    if (in.size(t.size() + 1) > t.size()) {
      t += in.peek_char(t.size());
      const auto i = b.ops().lower_bound(t);
      if (i != b.ops().end()) {
        if (match(in, b, s, t)) {
          return true;
        }
        if (i->first == t) {
          // While we are at it, this rule also performs the task of what would
          // usually be an associated action: To push the matched operator onto
          // the operator stack.
          s.push(i->second);
          in.bump(t.size());
          return true;
        }
      }
    }
    return false;
  }
};

// A number is a non-empty sequence of digits preceded by an optional sign, followed by an optional decimal point and
// further digits. This rule should match a stripped down float/double.
struct number : seq<opt<one<'+', '-'>>, plus<digit>, opt<one<'.'>, plus<digit>>> {};

struct expression;

// A bracketed expression is introduced by a '(' and, in this grammar, must
// proceed with an expression and a ')'.
struct bracket : seq<one<'('>, expression, one<')'>> {};

// An atomic expression, i.e. one without operators, is either a number or
// a bracketed expression.
struct atomic : sor<number, bracket> {};

// An expression is a non-empty list of atomic expressions where each pair
// of atomic expressions is separated by an infix operator and we allow
// the rule ignored as padding (before and after every single expression).
struct expression : list<atomic, infix, ignored> {};

// The top-level grammar allows one expression and then expects eof.
struct grammar : seq<expression, eof> {};

// After the grammar we proceed with the additional actions that are
// required to let our language actually do something.

// The base-case of the class template for the actions, does nothing.
template <typename Rule>
struct Action {};

// This action will be called when the number rule matches; it converts the
// matched portion of the input to a long and pushes it onto the operand
// stack.
template <>
struct Action<number> {
  template <typename ActionInput>
  static void apply(const ActionInput& in, const Operators& /*unused*/, ParseStack& s) {
    std::stringstream ss(in.string());
    double v;
    ss >> v;
    s.push(v);
  }
};

// The actions for the brackets call functions that create, and collect
// a temporary additional stack for evaluating the bracketed expression.
template <>
struct Action<one<'('>> {
  static void apply0(const Operators& /*unused*/, ParseStack& s) { s.open(); }
};

template <>
struct Action<one<')'>> {
  static void apply0(const Operators& /*unused*/, ParseStack& s) { s.close(); }
};

bool ExpressionLanguage::parse(std::string_view str, ParseStack& stacks) {
  static const Operators operators{};
  memory_input in(str.data(), str.size(), str);
  return tao::pegtl::parse<grammar, Action>(in, operators, stacks);
}

}  // namespace pack::language
