#include "component/primitive.h"

#include <set>
#include <string_view>
#include <vector>

#include "component/parameter.h"
#include "component/value.h"

namespace pack::component {

namespace {

[[maybe_unused]] Parameter build_parameter(std::string_view name, Type type) {
  Parameter result{};
  result.name = name;
  result.domain.type = Type::FLOAT;
  return result;
}

[[maybe_unused]] Parameter build_parameter(std::string_view name, float min_value) {
  Parameter result{};
  result.name = name;
  result.domain.type = Type::FLOAT;
  as_float(result.domain.min_value) = min_value;
  return result;
}

[[maybe_unused]] Parameter build_parameter(std::string_view name, float min_value, float max_value) {
  Parameter result{};
  result.name = name;
  result.domain.type = Type::FLOAT;
  as_float(result.domain.min_value) = min_value;
  as_float(result.domain.max_value) = max_value;
  return result;
}

[[maybe_unused]] Parameter build_parameter(std::string_view name, int64_t min_value) {
  Parameter result{};
  result.name = name;
  result.domain.type = Type::INTEGER;
  as_integer(result.domain.min_value) = min_value;
  return result;
}

[[maybe_unused]] Parameter build_parameter(std::string_view name, int64_t min_value, int64_t max_value) {
  Parameter result{};
  result.name = name;
  result.domain.type = Type::INTEGER;
  as_integer(result.domain.min_value) = min_value;
  as_integer(result.domain.max_value) = max_value;
  return result;
}

Primitive::Set generate_primitives() {
  Primitive::Set primitives{};
  primitives.emplace("gear", std::vector<Parameter>{
                                 build_parameter("inner_radius", Type::FLOAT),
                                 build_parameter("outer_radius", Type::FLOAT),
                                 build_parameter("width", Type::FLOAT),
                                 build_parameter("teeth", Type::INTEGER),
                                 build_parameter("tooth_depth", Type::FLOAT),
                                 build_parameter("angle_coefficient", Type::FLOAT),
                                 build_parameter("phase", Type::FLOAT),
                             });
  primitives.emplace("box", std::vector<Parameter>{
                                build_parameter("width", Type::FLOAT),   //
                                build_parameter("height", Type::FLOAT),  //
                                build_parameter("depth", Type::FLOAT),   //
                            });
  return primitives;
}

}  // namespace

const Primitive::Set Primitive::primitives_{generate_primitives()};

}  // namespace pack::component
