#pragma once

#include <set>
#include <string>
#include <unordered_set>

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
  using Set =
      std::unordered_set<Subcomponent, HashByIdField<Subcomponent, guid::GuidHash>, CompareByIdField<Subcomponent>>;

  // Id of the child component.
  guid::Guid id{};

  // Position and orientation wrt parent's reference frame.
  position::Position position{};
  position::Orientation orientation{};

  ParameterBinding::Set bindings{};

  bool operator==(const Subcomponent& rhs) const {
    return id == rhs.id &&                    //
           position == rhs.position &&        //
           orientation == rhs.orientation &&  //
           bindings == rhs.bindings;
  }
};

std::string to_string(const Subcomponent& subcomponent);
std::string to_string(const Subcomponent::Set& subcomponents);

struct Component final {
  using Set = std::unordered_set<Component, HashByIdField<Component, guid::GuidHash>, CompareByIdField<Component>>;

  // The id of a Component is globally unique.
  guid::Guid id{};
  std::string name{};

  const Primitive* primitive{nullptr};
  ParameterBinding::Set bindings{};
  material::Material material{};

  Subcomponent::Set children{};

  Parameter::Set parameters{};
  Property::Set properties{};

  bool operator==(const Component& rhs) const {
    return id == rhs.id &&                  //
           name == rhs.name &&              //
           primitive == rhs.primitive &&    //
           bindings == rhs.bindings &&      //
           children == rhs.children &&      //
           parameters == rhs.parameters &&  //
           properties == rhs.properties;
  }
};

std::string to_string(const Component& component);

}  // namespace pack::component
