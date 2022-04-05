#include "component/parameter.h"

#include <string>

#include "component/expression.h"

namespace pack::component {

std::string to_string(const ParameterBinding& binding) {
  using std::to_string;
  return binding.name + ": " + pack::component::to_string(binding.value);
}

std::string to_string(const ParameterBinding::Set& bindings) {
  using std::to_string;
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
