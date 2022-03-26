#pragma once

#include <set>
#include <string>
#include <unordered_set>

#include "component/ordering.h"
#include "component/primitive.h"
#include "component/types.h"
#include "guid/guid.h"

namespace pack::component {

struct Subcomponent final {
  using Set =
      std::unordered_set<Subcomponent, HashByIdField<Subcomponent, guid::GuidHash>, CompareByIdField<Subcomponent>>;

  // Id of the child component.
  guid::Guid id{};

  // TODO(james): Add position and orientation.

  ParameterBinding::Set bindings{};
};

struct Component final {
  using Set = std::unordered_set<Component, HashByIdField<Component, guid::GuidHash>, CompareByIdField<Component>>;

  // The id of a Component is globally unique.
  const guid::Guid id{};
  std::string name{};
  const Primitive* primitive{nullptr};

  // TODO(james): Can this be removed? Either document some use cases or remove.
  ParameterBinding::Set bindings{};

  Subcomponent::Set children{};

  Parameter::Set parameters{};
  Property::Set properties{};
};

}  // namespace pack::component
