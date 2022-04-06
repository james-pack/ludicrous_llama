/**
 * Helper functions for working with components, including creating, finding, editing, etc.
 *
 * This set of functions is deliberately non-exhaustive. It includes many functions that are useful in tests and other
 * select scenarios. Before adding more functions to this module, consider the added complexity that would introduce; it
 * may be more appropriate to use the API of the component types themselves.
 */
#pragma once

#include <string>
#include <string_view>

#include "component/component.h"

namespace pack::component {

// Helper functions for creating components with float values for the primitive parameters.
Component create_component(std::string name, std::string primitive_name, std::string parameter1, float value1);
Component create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                           std::string parameter2, float value2);
Component create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                           std::string parameter2, float value2, std::string parameter3, float value3);

// Helper functions for creating components with expressions as values for the primitive parameters.
Component create_component(std::string name, std::string primitive_name, std::string parameter1,
                           std::string expression1);
Component create_component(std::string name, std::string primitive_name, std::string parameter1,
                           std::string expression1, std::string parameter2, std::string expression2);
Component create_component(std::string name, std::string primitive_name, std::string parameter1,
                           std::string expression1, std::string parameter2, std::string expression2,
                           std::string parameter3, std::string expression3);

// Helper functions for creating components, starting with a component that has already been instantiated.
guid::Guid create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                            Component* component);
guid::Guid create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                            std::string parameter2, float value2, Component* component);
guid::Guid create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                            std::string parameter2, float value2, std::string parameter3, float value3,
                            Component* component);

guid::Guid create_component(std::string name, std::string primitive_name, std::string parameter1,
                            std::string expression1, Component* component);
guid::Guid create_component(std::string name, std::string primitive_name, std::string parameter1,
                            std::string expression1, std::string parameter2, std::string expression2,
                            Component* component);
guid::Guid create_component(std::string name, std::string primitive_name, std::string parameter1,
                            std::string expression1, std::string parameter2, std::string expression2,
                            std::string parameter3, std::string expression3, Component* component);

// For now, we don't need anything fancy to describe a bunch of Components.
using Components = Component::Set;

// Simple search to find the id in the Components.
const Component* find_by_id(const Components& components, std::string_view id);
Component* find_by_id(Components& components, std::string_view id);

std::string to_string(const Components& components);

}  // namespace pack::component
