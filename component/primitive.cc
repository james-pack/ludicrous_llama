#include "component/primitive.h"

#include <functional>
#include <string_view>

#include "component/box.h"
#include "component/gear.h"
#include "component/parameter.h"
#include "component/value.h"

namespace pack::component {

namespace {

Primitive::GeneratorFunc construct_draw_list_generator(std::function<GLint(const ParameterBinding::Set&)> builder) {
  return [builder](const ParameterBinding::Set& bindings) {
    GLint draw_list_id = builder(bindings);
    return [draw_list_id]() { glCallList(draw_list_id); };
  };
}

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

// TODO(james): This function should be constexpr. All of the details are known at compile-time.
Primitive::Set generate_primitives() {
  Primitive::Set primitives{};
  primitives.emplace("gear",
                     Parameter::Set{
                         build_parameter("inner_radius", Type::FLOAT),
                         build_parameter("outer_radius", Type::FLOAT),
                         build_parameter("width", Type::FLOAT),
                         build_parameter("teeth", Type::INTEGER),
                         build_parameter("tooth_depth", Type::FLOAT),
                         build_parameter("angle_coefficient", Type::FLOAT),
                         build_parameter("phase", Type::FLOAT),
                     },
                     construct_draw_list_generator(build_gear));
  primitives.emplace("box",
                     Parameter::Set{
                         build_parameter("width", Type::FLOAT),   //
                         build_parameter("height", Type::FLOAT),  //
                         build_parameter("depth", Type::FLOAT),   //
                     },
                     construct_draw_list_generator(build_box));
  return primitives;
}

}  // namespace

const Primitive::Set Primitive::primitives_{generate_primitives()};

}  // namespace pack::component
