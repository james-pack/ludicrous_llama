#pragma once

#include <set>
#include <string>

#include "component/expression.h"
#include "component/ordering.h"
#include "component/proto/component.pb.h"

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

inline void to_proto(const Property& property, proto::Property* proto) {
  proto->set_name(property.name);
  to_proto(property.value, proto->mutable_value());
}

inline void from_proto(const proto::Property& proto, Property* property) {
  property->name = proto.name();
  from_proto(proto.value(), &property->value);
}

}  // namespace pack::component
