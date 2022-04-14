#pragma once

#include <set>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

#include "component/ordering.h"
#include "component/parameter.h"
#include "component/primitive.h"
#include "component/property.h"
#include "guid/guid.h"
#include "material/material.h"
#include "position/position.h"

namespace pack::component {

/**
 * Subcomponent represents both the joint combining two components in a traditional CAD/robotics platform as well as the
 * concrete form of a templated component through bound parameters. This design is probably going to be incorrect; it
 * will likely result in confusion to the user and other programmers.
 */
struct Subcomponent final {
  using Container = std::vector<Subcomponent>;

  guid::Guid id{};

  // Subcomponents can either refer to primitives or to child components. If it refers to a primitive, the primitive
  // field will be non-null and the material field will be populated. If it refers to a child component, the primitive
  // field will be null and the id below will refer to a valid component.
  enum class Type : uint8_t {
    UNTYPED,
    PRIMITIVE,
    COMPONENT,
  };
  // Type of subcomponent.
  Type type{};

  const Primitive* primitive{nullptr};
  material::Material material{};

  // Id of the child component.
  guid::Guid child_id{};

  // Position and orientation wrt parent's reference frame. These fields are independent of the type of subcomponent.
  position::Position position{};
  position::Orientation orientation{};

  // Bindings of expressions / values to the subcomponent's parameters.
  ParameterBinding::Set bindings{};

  bool operator==(const Subcomponent& rhs) const {
    using std::to_string;
    if (type != rhs.type || position != rhs.position || orientation != rhs.orientation || bindings != rhs.bindings) {
      return false;
    }
    switch (type) {
      case Type::UNTYPED:
        return true;
      case Type::PRIMITIVE:
        return primitive == rhs.primitive && material == rhs.material;
      case Type::COMPONENT:
        return child_id == rhs.child_id;
      default:
        throw std::domain_error("Unknown Subcomponent type '" +
                                to_string(static_cast<std::underlying_type_t<Type>>(type)) + "'");
    }
  }
};

std::string to_string(const Subcomponent& subcomponent);
std::string to_string(const Subcomponent::Type type);
std::string to_string(const Subcomponent::Container& subcomponents);

struct Component final {
  using Set = std::unordered_set<Component, HashByIdField<Component, guid::GuidHash>, CompareByIdField<Component>>;

  // The id of a Component is globally unique.
  guid::Guid id{};
  std::string name{};

  Subcomponent::Container children{};

  Parameter::Set parameters{};
  Property::Set properties{};

  bool operator==(const Component& rhs) const {
    return id == rhs.id &&                  //
           name == rhs.name &&              //
           children == rhs.children &&      //
           parameters == rhs.parameters &&  //
           properties == rhs.properties;
  }
};

std::string to_string(const Component& component);

}  // namespace pack::component
