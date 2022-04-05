#include "component/parameter.h"

#include <string>

namespace pack::component {

std::string to_string(const Precision& precision) {
  using std::to_string;
  return "num_decimal_places: " + to_string(precision.num_decimal_places);
}

std::string to_string(const ValueDomain& domain) {
  std::string result{};

  result += "type: " + to_string(domain.type);
  result += ", ";

  result += "min_value: " + to_string(domain.min_value);
  result += ", ";

  result += "max_value: " + to_string(domain.max_value);
  result += ", ";

  result += "precision: " + to_string(domain.precision);

  return result;
}

std::string to_string(const Parameter& parameter) {
  std::string result{};
  result += "name: " + parameter.name;
  result += ", ";

  result += "domain: " + to_string(parameter.domain);
  result += ", ";

  result += "default_value: " + to_string(parameter.default_value);

  return result;
}

std::string to_string(const Parameter::Set& parameters) {
  std::string result{"{"};
  bool need_comma{false};
  for (const auto& parameter : parameters) {
    if (need_comma) {
      result += ", ";
    }
    result += to_string(parameter);
    need_comma = true;
  }
  result += "}";
  return result;
}

std::string to_string(const ParameterBinding& binding) { return binding.name + ": " + to_string(binding.value); }

std::string to_string(const ParameterBinding::Set& bindings) {
  std::string result{"{"};
  bool need_comma{false};
  for (const auto& binding : bindings) {
    if (need_comma) {
      result += ", ";
    }
    result += to_string(binding);
    need_comma = true;
  }
  result += "}";
  return result;
}

}  // namespace pack::component
