/**
 * Helper functions for working with components, including creating, finding, editing, etc.
 *
 * This set of functions is deliberately non-exhaustive. It includes many functions that are useful in tests and other
 * select scenarios. Before adding more functions to this module, consider the added complexity that would introduce; it
 * may be more appropriate to use the API of the protos themselves.
 */
#pragma once

#include <string>
#include <string_view>

#include "component/component.pb.h"

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
std::string create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                             Component* component);
std::string create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                             std::string parameter2, float value2, Component* component);
std::string create_component(std::string name, std::string primitive_name, std::string parameter1, float value1,
                             std::string parameter2, float value2, std::string parameter3, float value3,
                             Component* component);

std::string create_component(std::string name, std::string primitive_name, std::string parameter1,
                             std::string expression1, Component* component);
std::string create_component(std::string name, std::string primitive_name, std::string parameter1,
                             std::string expression1, std::string parameter2, std::string expression2,
                             Component* component);
std::string create_component(std::string name, std::string primitive_name, std::string parameter1,
                             std::string expression1, std::string parameter2, std::string expression2,
                             std::string parameter3, std::string expression3, Component* component);

// Simple linear search to find the id in the Components. These methods do not expect the components to be sorted or
// have any special organization.
const Component* find_by_id(const Components& components, std::string_view id);
Component* find_by_id(Components& components, std::string_view id);

}  // namespace pack::component
