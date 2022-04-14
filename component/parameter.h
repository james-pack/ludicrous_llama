#pragma once

#include <set>
#include <string>

#include "component/expression.h"
#include "component/ordering.h"
#include "component/value.h"

namespace pack::component {

struct Precision final {
  // There are many ways of determining precision. For now, we focus on the simplest form, number of decimal places, but
  // should expand this to handle different concepts as they arise.
  int32_t num_decimal_places{-1};

  bool operator==(const Precision& rhs) const { return num_decimal_places == rhs.num_decimal_places; }
};

std::string to_string(const Precision& precision);

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

std::string to_string(const ValueDomain& domain);

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

std::string to_string(const Parameter& parameter);
std::string to_string(const Parameter::Set& parameters);

struct ParameterBinding final {
  using Set = std::set<ParameterBinding, OrderByNameField<ParameterBinding>>;

  // Name of the parameter being bound.
  std::string name{};

  // Value it is bound to.
  Expression value{};

  // Cached lookup of parameter.
  const Parameter* cached_parameter{nullptr};

  bool operator==(const ParameterBinding& rhs) const {
    return name == rhs.name &&  //
           value == rhs.value;
  }
};

std::string to_string(const ParameterBinding& binding);
std::string to_string(const ParameterBinding::Set& bindings);

}  // namespace pack::component
