#pragma once

#include <functional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "component/ordering.h"
#include "component/parameter.h"
#include "material/material.h"

namespace pack::component {

/**
 * DrawFuncs are simple thunks that can be evaluated on every redraw cycle. They can redraw a mesh from scratch, or they
 * can leverage an existing draw list or VBO or similar cached structure.
 */
using DrawFunc = std::function<void()>;

/**
 * Primitives are generators for DrawFuncs. They bind the name of the primitive to the creation of a VBO, draw list, or
 * immediate mode rendering scheme. The set of primitives is constructed the first time this class is referenced and
 * does not change.
 */
class Primitive final {
 public:
  using Set = std::set<Primitive, OrderByName<Primitive>>;

  using GeneratorFunc = std::function<DrawFunc(const ParameterBinding::Set&, const material::Material&)>;

 private:
  static const Set primitives_;

  const std::string_view name_;
  const Parameter::Set parameters_;
  const GeneratorFunc generator_;

 public:
  // TODO(james): Rework to make constexpr. These are known fully at compile-time and should be defined that way.
  Primitive(std::string_view name, Parameter::Set&& parameters, GeneratorFunc generator)
    : name_(name), parameters_(std::move(parameters)), generator_(std::move(generator)) {}

  std::string_view name() const { return name_; }
  const Parameter::Set& parameters() const { return parameters_; }

  /**
   * Method to check if a DrawFunc can be generated from the set of bindings.
   */
  bool can_generate(const ParameterBinding::Set& bindings) const {
    for (const auto& parameter : parameters_) {
      if (bindings.find(parameter.name) == bindings.cend()) {
        return false;
      }
    }
    return true;
  }

  DrawFunc generate(const ParameterBinding::Set& bindings, const material::Material& material) const {
    return generator_(bindings, material);
  }

  static const Set& primitives() { return primitives_; }
  static const Primitive* by_name(std::string_view name) {
    auto iter = primitives_.find(name);
    if (iter != primitives_.end()) {
      return &(*iter);
    }
    return nullptr;
  }
};

}  // namespace pack::component
