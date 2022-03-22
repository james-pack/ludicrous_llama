#pragma once

#include <functional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "glog/logging.h"

namespace pack::language {

// Currently, we only support operations that generate floats. There is no type system. These markers help us identify
// all of the places that will need to change to support other types (booleans, integers, enumerated sets, etc.) later.
using ExpressionValueType = float;
using BinaryOperatorEvaluator = std::function<ExpressionValueType(ExpressionValueType, ExpressionValueType)>;

// TODO(james): Write these.
class IdentifierBindings;
class SymbolTable;

// This enum is used for the order in which the operators are
// evaluated, i.e. the priority of the operators; a higher
// number indicates a lower priority.
enum class OperatorPriority : uint8_t {};

// For each binary operator known to the calculator we need an
// instance of the following data structure with the priority,
// and a function that performs the calculation. All operators
// are left-associative.
struct Operator {
  OperatorPriority p;
  BinaryOperatorEvaluator f;
};

class ParseNode {
 public:
  virtual ~ParseNode() = default;

  virtual void identifiers(std::vector<std::string_view>* result) const;
  virtual void bind(const IdentifierBindings& bindings);

  virtual ExpressionValueType eval(const SymbolTable& symbols) = 0;

  virtual void as_string(std::string* str) const = 0;
  std::string as_string() const {
    std::string result{};
    as_string(&result);
    return result;
  }
};

// Class that takes care of an operand and an operator stack for
// shift-reduce style handling of operator priority; in a
// reduce-step it calls on the functions contained in the op
// instances to perform the calculation.
class SubexpressionStack final {
 private:
  std::vector<Operator> operators_;
  std::vector<ExpressionValueType> operands_;

  void reduce() {
    CHECK(!operators_.empty()) << "No operators to reduce";
    CHECK_GT(operands_.size(), 1) << "Need at least two operands to reduce.";

    const auto r = operands_.back();
    operands_.pop_back();
    const auto l = operands_.back();
    operands_.pop_back();
    const auto o = operators_.back();
    operators_.pop_back();
    operands_.push_back(o.f(l, r));
  }

 public:
  void push(const Operator& b) {
    while ((!operators_.empty()) && (operators_.back().p <= b.p)) {
      reduce();
    }
    operators_.push_back(b);
  }

  void push(const ExpressionValueType l) { operands_.push_back(l); }

  ExpressionValueType finish() {
    while (!operators_.empty()) {
      reduce();
    }
    CHECK_EQ(operands_.size(), 1) << "Should be exactly one operand after reducing in finish.";
    CHECK_EQ(operators_.size(), 0) << "Should be exactly no operators remaining after reducing in finish.";
    const auto result = operands_.back();
    operands_.clear();
    return result;
  }
};

// Additional layer, a "stack of stacks", to clearly show how bracketed
// sub-expressions can be easily processed by giving them a stack of
// their own. Once a bracketed sub-expression has finished evaluation on
// its stack, the result is pushed onto the next higher stack, and the
// sub-expression's temporary stack is discarded. The top-level calculation
// is handled just like a bracketed sub-expression, on the first stack pushed
// by the constructor.
class ParseStack final {
 public:
  ParseStack() { open(); }

  void open() { subexpressions_.emplace_back(); }

  template <typename T>
  void push(const T& expr) {
    CHECK(!subexpressions_.empty()) << "Must have at least one subexpression to push to.";
    subexpressions_.back().push(expr);
  }

  void close() {
    CHECK_GT(subexpressions_.size(), 1) << "Must have at least two subexpressions in close() -- one to finish, and one "
                                           "to place the result of that finished subexpression.";
    const auto value = subexpressions_.back().finish();
    subexpressions_.pop_back();
    subexpressions_.back().push(value);
  }

  ExpressionValueType finish() {
    CHECK_EQ(subexpressions_.size(), 1) << "Should be exactly one subexpression in finish.";
    return subexpressions_.back().finish();
  }

 private:
  std::vector<SubexpressionStack> subexpressions_{};
};

class IdentifierNode final : public ParseNode {
 private:
  std::string_view text_;
  const IdentifierBindings* bindings_{nullptr};

 public:
  IdentifierNode(std::string_view text) : text_(text) {}

  void identifiers(std::vector<std::string_view>* result) const override;
  void bind(const IdentifierBindings& bindings) override;

  ExpressionValueType eval(const SymbolTable& symbols) override;

  void as_string(std::string* str) const override;
};

class LiteralNode final : public ParseNode {
 private:
  ExpressionValueType value_{};
  std::string_view text_;

 public:
  LiteralNode(std::string_view text) : text_({text}) {
    std::istringstream ss(std::string{text_});
    ss >> value_;
  }

  void identifiers(std::vector<std::string_view>* result) const override;
  void bind(const IdentifierBindings& bindings) override;

  ExpressionValueType eval(const SymbolTable& symbols) override;

  void as_string(std::string* str) const override;
};

class BinaryOperatorNode final : public ParseNode {
 private:
  std::vector<ParseNode*> children_{2, nullptr};
  BinaryOperatorEvaluator operation_;
  std::string_view text_;

 public:
  BinaryOperatorNode(std::string_view text, BinaryOperatorEvaluator operation) : operation_(operation), text_(text) {}

  void identifiers(std::vector<std::string_view>* result) const override;
  void bind(const IdentifierBindings& bindings) override;

  ExpressionValueType eval(const SymbolTable& symbols) override;

  void set_left_child(ParseNode* lhs) { children_[0] = lhs; }
  void set_right_child(ParseNode* rhs) { children_[1] = rhs; }

  void as_string(std::string* str) const override;
};

}  // namespace pack::language
