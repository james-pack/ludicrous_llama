#pragma once

#include <set>
#include <string>

#include "component/expression.h"
#include "component/ordering.h"

namespace pack::component {

struct Property final {
  using Set = std::set<Property, OrderByNameField<Property>>;

  std::string name{};
  Expression value{};

  bool operator==(const Property& rhs) const {
    return name == rhs.name &&  //
           value == rhs.value;
  }
};

std::string to_string(const Property& property);
std::string to_string(const Property::Set& properties);

}  // namespace pack::component
