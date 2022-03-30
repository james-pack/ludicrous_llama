#pragma once

#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "component/ordering.h"
#include "component/parameter.h"

namespace pack::component {

class Primitive final {
 public:
  using Set = std::set<Primitive, OrderByName<Primitive>>;

 private:
  static const Set primitives_;

  const std::string name_;
  const std::vector<Parameter> parameters_;

 public:
  Primitive(std::string_view name, const std::vector<Parameter>& parameters) : name_(name), parameters_(parameters) {}

  const std::string& name() const { return name_; }
  const std::vector<Parameter>& parameters() const { return parameters_; }

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
