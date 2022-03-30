#include "component/components.h"

#include <string>
#include <string_view>

#include "component/component.h"
#include "component/parameter.h"
#include "component/primitive.h"
#include "guid/guid.h"

namespace pack::component {

guid::Guid create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                            Component* component) {
  component->name = std::move(name);
  component->primitive = Primitive::by_name(primitive_name);

  component->bindings.insert(ParameterBinding{std::move(parameter1), Expression{value1}});
  return component->id;
}

guid::Guid create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                            std::string parameter2, float value2, Component* component) {
  component->name = std::move(name);
  component->primitive = Primitive::by_name(primitive_name);

  component->bindings.insert(ParameterBinding{std::move(parameter1), Expression{value1}});
  component->bindings.insert(ParameterBinding{std::move(parameter2), Expression{value2}});
  return component->id;
}

guid::Guid create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                            std::string parameter2, float value2, std::string parameter3, float value3,
                            Component* component) {
  component->name = std::move(name);
  component->primitive = Primitive::by_name(primitive_name);

  component->bindings.insert(ParameterBinding{std::move(parameter1), Expression{value1}});
  component->bindings.insert(ParameterBinding{std::move(parameter2), Expression{value2}});
  component->bindings.insert(ParameterBinding{std::move(parameter3), Expression{value3}});
  return component->id;
}

guid::Guid create_component(std::string name, std::string primitive_name, std::string parameter1,
                            std::string expression1, Component* component) {
  component->name = std::move(name);
  component->primitive = Primitive::by_name(primitive_name);

  component->bindings.insert(ParameterBinding{std::move(parameter1), Expression{std::move(expression1)}});
  return component->id;
}

guid::Guid create_component(std::string name, std::string primitive_name, std::string parameter1,
                            std::string expression1, std::string parameter2, std::string expression2,
                            Component* component) {
  component->name = std::move(name);
  component->primitive = Primitive::by_name(primitive_name);

  component->bindings.insert(ParameterBinding{std::move(parameter1), Expression{std::move(expression1)}});
  component->bindings.insert(ParameterBinding{std::move(parameter2), Expression{std::move(expression2)}});
  return component->id;
}

guid::Guid create_component(std::string name, std::string primitive_name, std::string parameter1,
                            std::string expression1, std::string parameter2, std::string expression2,
                            std::string parameter3, std::string expression3, Component* component) {
  component->name = std::move(name);
  component->primitive = Primitive::by_name(primitive_name);

  component->bindings.insert(ParameterBinding{std::move(parameter1), Expression{std::move(expression1)}});
  component->bindings.insert(ParameterBinding{std::move(parameter2), Expression{std::move(expression2)}});
  component->bindings.insert(ParameterBinding{std::move(parameter3), Expression{std::move(expression3)}});
  return component->id;
}

Component create_component(std::string name, std::string primitive_name, std::string parameter1, float value1) {
  Component result{};
  create_component(std::move(name), std::move(primitive_name), std::move(parameter1), value1, &result);
  return result;
}

Component create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                           std::string parameter2, float value2) {
  Component result{};
  create_component(std::move(name), std::move(primitive_name), std::move(parameter1), value1, std::move(parameter2),
                   value2, &result);
  return result;
}

Component create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                           std::string parameter2, float value2, std::string parameter3, float value3) {
  Component result{};
  create_component(std::move(name), std::move(primitive_name), std::move(parameter1), value1, std::move(parameter2),
                   value2, std::move(parameter3), value3, &result);
  return result;
}

Component create_component(std::string name, std::string primitive_name, std::string parameter1,
                           std::string expression1) {
  Component result{};
  create_component(std::move(name), std::move(primitive_name), std::move(parameter1), std::move(expression1), &result);
  return result;
}

Component create_component(std::string name, std::string primitive_name, std::string parameter1,
                           std::string expression1, std::string parameter2, std::string expression2) {
  Component result{};
  create_component(std::move(name), std::move(primitive_name), std::move(parameter1), std::move(expression1),
                   std::move(parameter2), std::move(expression2), &result);
  return result;
}

Component create_component(std::string name, std::string primitive_name, std::string parameter1,
                           std::string expression1, std::string parameter2, std::string expression2,
                           std::string parameter3, std::string expression3) {
  Component result{};
  create_component(std::move(name), std::move(primitive_name), std::move(parameter1), std::move(expression1),
                   std::move(parameter2), std::move(expression2), std::move(parameter3), std::move(expression3),
                   &result);
  return result;
}

const Component* find_by_id(const Components& components, std::string_view id) {
#if defined(__cpp_lib_generic_unordered_lookup)
  const auto iter = components.find(id);
#else
  const Component for_comparison{guid::Guid{id}};
  const auto iter = components.find(for_comparison);
#endif
  if (iter != components.end()) {
    return &(*iter);
  }
  return nullptr;
}

Component* find_by_id(Components& components, std::string_view id) {
#if defined(__cpp_lib_generic_unordered_lookup)
  const auto iter = components.find(id);
#else
  const Component for_comparison{guid::Guid{id}};
  const auto iter = components.find(for_comparison);
#endif
  if (iter != components.end()) {
    return const_cast<Component*>(&(*iter));
  }
  return nullptr;
}

}  // namespace pack::component
