#pragma once

#include <set>
#include <string>

#include "component/expression.h"
#include "component/ordering.h"
#include "component/proto/component.pb.h"
#include "component/value.h"
#include "serialization/serialize.h"

namespace pack::component {

struct Precision final {
  // There are many ways of determining precision. For now, we focus on the simplest form, number of decimal places, but
  // should expand this to handle different concepts as they arise.
  int32_t num_decimal_places{-1};

  bool operator==(const Precision& rhs) const { return num_decimal_places == rhs.num_decimal_places; }
};

struct ValueDomain final {
  Type type{};

  // Use when the domain is best modelled as a min value, max value and a precision. Note that none of the three are
  // required and reasonable defaults should be used when interpreting this message. The type, values and precision
  // should agree.
  Value min_value{};
  Value max_value{};
  Precision precision{};

  bool operator==(const ValueDomain& rhs) const {
    return type == rhs.type &&            //
           min_value == rhs.min_value &&  //
           max_value == rhs.max_value &&  //
           precision == rhs.precision;
  }
};

// A Parameter is a value provided to a primitive, component, process, etc. that allows for variable behavior. It is
// similar to a parameter to a function.
struct Parameter final {
  using Set = std::set<Parameter, OrderByNameField<Parameter>>;

  std::string name{};

  ValueDomain domain{};
  Value default_value{};

  bool operator==(const Parameter& rhs) const {
    return name == rhs.name &&      //
           domain == rhs.domain &&  //
           default_value == rhs.default_value;
  }
};

struct ParameterBinding final {
  using Set = std::set<ParameterBinding, OrderByNameField<ParameterBinding>>;

  // Name of the parameter being bound.
  std::string name{};

  // Value it is bound to.
  Expression value{};

  bool operator==(const ParameterBinding& rhs) const {
    return name == rhs.name &&  //
           value == rhs.value;
  }
};

}  // namespace pack::component

namespace pack {

template <>
inline void to_proto(const component::Precision& value, component::proto::Precision* proto) {
  proto->set_num_decimal_places(value.num_decimal_places);
}

template <>
inline void from_proto(const component::proto::Precision& proto, component::Precision* value) {
  value->num_decimal_places = proto.num_decimal_places();
}

template <>
inline void to_proto(const component::ValueDomain& domain, component::proto::ValueDomain* proto) {
  proto->mutable_type()->set_type(static_cast<component::proto::Type::Types>(domain.type));
  to_proto(domain.min_value, proto->mutable_min_value());
  to_proto(domain.max_value, proto->mutable_max_value());
  to_proto(domain.precision, proto->mutable_precision());
}

template <>
inline void from_proto(const component::proto::ValueDomain& proto, component::ValueDomain* domain) {
  domain->type = static_cast<component::Type>(proto.type().type());
  from_proto(proto.min_value(), &domain->min_value);
  from_proto(proto.max_value(), &domain->max_value);
  from_proto(proto.precision(), &domain->precision);
}

template <>
inline void to_proto(const component::Parameter& parameter, component::proto::Parameter* proto) {
  proto->set_name(parameter.name);
  to_proto(parameter.domain, proto->mutable_domain());
  to_proto(parameter.default_value, proto->mutable_default_value());
}

template <>
inline void from_proto(const component::proto::Parameter& proto, component::Parameter* parameter) {
  parameter->name = proto.name();
  from_proto(proto.domain(), &parameter->domain);
  from_proto(proto.default_value(), &parameter->default_value);
}

template <>
inline void to_proto(const component::ParameterBinding& binding, component::proto::ParameterBinding* proto) {
  proto->set_name(binding.name);
  to_proto(binding.value, proto->mutable_value());
}

template <>
inline void from_proto(const component::proto::ParameterBinding& proto, component::ParameterBinding* binding) {
  binding->name = proto.name();
  from_proto(proto.value(), &binding->value);
}

}  // namespace pack
