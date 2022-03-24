#include "component/components.h"

#include <string>
#include <string_view>

#include "component/proto/component.pb.h"
#include "guid/guid.h"

namespace pack::component {

using namespace pack::component::proto;

std::string create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                             Component* component) {
  std::string id{guid::Guid()};
  component->set_id(id);
  component->set_name(std::move(name));
  ProcessedPrimitive* processed = component->add_primitives();
  Primitive* primitive = processed->mutable_primitive();
  primitive->set_name(std::move(primitive_name));
  ParameterBinding* binding = primitive->add_parameters();
  binding->set_parameter_name(std::move(parameter1));
  binding->mutable_value()->mutable_literal()->set_float_value(value1);
  return id;
}

std::string create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                             std::string parameter2, float value2, Component* component) {
  std::string id{guid::Guid()};
  component->set_id(id);
  component->set_name(std::move(name));
  ProcessedPrimitive* processed = component->add_primitives();
  Primitive* primitive = processed->mutable_primitive();
  primitive->set_name(std::move(primitive_name));
  {
    ParameterBinding* binding = primitive->add_parameters();
    binding->set_parameter_name(std::move(parameter1));
    binding->mutable_value()->mutable_literal()->set_float_value(value1);
  }
  {
    ParameterBinding* binding = primitive->add_parameters();
    binding->set_parameter_name(std::move(parameter2));
    binding->mutable_value()->mutable_literal()->set_float_value(value2);
  }
  return id;
}

std::string create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                             std::string parameter2, float value2, std::string parameter3, float value3,
                             Component* component) {
  std::string id{guid::Guid()};
  component->set_id(id);
  component->set_name(std::move(name));
  ProcessedPrimitive* processed = component->add_primitives();
  Primitive* primitive = processed->mutable_primitive();
  primitive->set_name(std::move(primitive_name));
  {
    ParameterBinding* binding = primitive->add_parameters();
    binding->set_parameter_name(std::move(parameter1));
    binding->mutable_value()->mutable_literal()->set_float_value(value1);
  }
  {
    ParameterBinding* binding = primitive->add_parameters();
    binding->set_parameter_name(std::move(parameter2));
    binding->mutable_value()->mutable_literal()->set_float_value(value2);
  }
  {
    ParameterBinding* binding = primitive->add_parameters();
    binding->set_parameter_name(std::move(parameter3));
    binding->mutable_value()->mutable_literal()->set_float_value(value3);
  }
  return id;
}

std::string create_component(std::string name, std::string primitive_name, std::string parameter1,
                             std::string expression1, Component* component) {
  std::string id{guid::Guid()};
  component->set_id(id);
  component->set_name(std::move(name));
  ProcessedPrimitive* processed = component->add_primitives();
  Primitive* primitive = processed->mutable_primitive();
  primitive->set_name(std::move(primitive_name));
  ParameterBinding* binding = primitive->add_parameters();
  binding->set_parameter_name(std::move(parameter1));
  binding->mutable_value()->set_expression(std::move(expression1));
  return id;
}

std::string create_component(std::string name, std::string primitive_name, std::string parameter1,
                             std::string expression1, std::string parameter2, std::string expression2,
                             Component* component) {
  std::string id{guid::Guid()};
  component->set_id(id);
  component->set_name(std::move(name));
  ProcessedPrimitive* processed = component->add_primitives();
  Primitive* primitive = processed->mutable_primitive();
  primitive->set_name(std::move(primitive_name));
  {
    ParameterBinding* binding = primitive->add_parameters();
    binding->set_parameter_name(std::move(parameter1));
    binding->mutable_value()->set_expression(std::move(expression1));
  }
  {
    ParameterBinding* binding = primitive->add_parameters();
    binding->set_parameter_name(std::move(parameter2));
    binding->mutable_value()->set_expression(std::move(expression2));
  }
  return id;
}

std::string create_component(std::string name, std::string primitive_name, std::string parameter1,
                             std::string expression1, std::string parameter2, std::string expression2,
                             std::string parameter3, std::string expression3, Component* component) {
  std::string id{guid::Guid()};
  component->set_id(id);
  component->set_name(std::move(name));
  ProcessedPrimitive* processed = component->add_primitives();
  Primitive* primitive = processed->mutable_primitive();
  primitive->set_name(std::move(primitive_name));
  {
    ParameterBinding* binding = primitive->add_parameters();
    binding->set_parameter_name(std::move(parameter1));
    binding->mutable_value()->set_expression(std::move(expression1));
  }
  {
    ParameterBinding* binding = primitive->add_parameters();
    binding->set_parameter_name(std::move(parameter2));
    binding->mutable_value()->set_expression(std::move(expression2));
  }
  {
    ParameterBinding* binding = primitive->add_parameters();
    binding->set_parameter_name(std::move(parameter3));
    binding->mutable_value()->set_expression(std::move(expression3));
  }
  return id;
}

Component create_component(std::string name, std::string primitive_name, std::string parameter1, float value1) {
  Component result;
  create_component(std::move(name), std::move(primitive_name), std::move(parameter1), value1, &result);
  return result;
}

Component create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                           std::string parameter2, float value2) {
  Component result;
  create_component(std::move(name), std::move(primitive_name), std::move(parameter1), value1, std::move(parameter2),
                   value2, &result);
  return result;
}

Component create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                           std::string parameter2, float value2, std::string parameter3, float value3) {
  Component result;
  create_component(std::move(name), std::move(primitive_name), std::move(parameter1), value1, std::move(parameter2),
                   value2, std::move(parameter3), value3, &result);
  return result;
}

Component create_component(std::string name, std::string primitive_name, std::string parameter1,
                           std::string expression1) {
  Component result;
  create_component(std::move(name), std::move(primitive_name), std::move(parameter1), std::move(expression1), &result);
  return result;
}

Component create_component(std::string name, std::string primitive_name, std::string parameter1,
                           std::string expression1, std::string parameter2, std::string expression2) {
  Component result;
  create_component(std::move(name), std::move(primitive_name), std::move(parameter1), std::move(expression1),
                   std::move(parameter2), std::move(expression2), &result);
  return result;
}

Component create_component(std::string name, std::string primitive_name, std::string parameter1,
                           std::string expression1, std::string parameter2, std::string expression2,
                           std::string parameter3, std::string expression3) {
  Component result;
  create_component(std::move(name), std::move(primitive_name), std::move(parameter1), std::move(expression1),
                   std::move(parameter2), std::move(expression2), std::move(parameter3), std::move(expression3),
                   &result);
  return result;
}

const Component* find_by_id(const Components& components, std::string_view id) {
  for (const auto& component : components.components()) {
    if (component.id() == id) {
      return &component;
    }
  }
  return nullptr;
}

Component* find_by_id(Components& components, std::string_view id) {
  for (auto& component : *components.mutable_components()) {
    if (component.id() == id) {
      return &component;
    }
  }
  return nullptr;
}

}  // namespace pack::component
