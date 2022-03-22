#include "language/parse_tree.h"

#include <string>
#include <string_view>
#include <vector>

namespace pack::language {

void ParseNode::identifiers(std::vector<std::string_view>*) const {
  // Nothing to do by default. Subclasses can override.
}

void ParseNode::bind(const IdentifierBindings& bindings) {
  // Do nothing by default. Subclasses can override.
}

void IdentifierNode::identifiers(std::vector<std::string_view>* result) const { result->push_back(text_); }

void IdentifierNode::bind(const IdentifierBindings& bindings) { bindings_ = &bindings; }

float IdentifierNode::eval(const SymbolTable& symbols) {
  //  const auto [component, property] = bindings_->lookup(text_);
  //  return symbols.find(component).get(property).eval(symbols);
  return 0.f;
}

void IdentifierNode::as_string(std::string* str) const { str->append(text_); }

float LiteralNode::eval(const SymbolTable& symbols) { return value_; }

void LiteralNode::as_string(std::string* str) const { str->append(text_); }

void BinaryOperatorNode::identifiers(std::vector<std::string_view>* result) const {
  for (const auto* child : children_) {
    child->identifiers(result);
  }
}

void BinaryOperatorNode::bind(const IdentifierBindings& bindings) {
  for (auto* child : children_) {
    child->bind(bindings);
  }
}

float BinaryOperatorNode::eval(const SymbolTable& symbols) {
  float lhs = children_[0]->eval(symbols);
  float rhs = children_[1]->eval(symbols);
  return operation_(lhs, rhs);
}

void BinaryOperatorNode::as_string(std::string* str) const {
  children_[0]->as_string(str);
  str->append(1, ' ').append(text_).append(1, ' ');
  children_[1]->as_string(str);
}

}  // namespace pack::language
