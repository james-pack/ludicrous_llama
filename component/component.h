#pragma once

#include <set>
#include <string>
#include <unordered_set>

#include "component/ordering.h"
#include "component/parameter.h"
#include "component/primitive.h"
#include "component/property.h"
#include "component/proto/component.pb.h"
#include "component/serialize.h"
#include "guid/guid.h"
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

template <>
void to_proto(const Subcomponent& subcomponent, proto::Subcomponent* proto) {
  proto->set_child_id(subcomponent.id.as_string());

  position::to_proto(subcomponent.position, proto->mutable_position());
  position::to_proto(subcomponent.orientation, proto->mutable_orientation());

  for (const auto& binding : subcomponent.bindings) {
    to_proto(binding, proto->add_bindings());
  }
}

template <>
inline void from_proto(const proto::Subcomponent& proto, Subcomponent* subcomponent) {
  subcomponent->id = guid::Guid(proto.child_id());

  position::from_proto(proto.position(), &subcomponent->position);
  position::from_proto(proto.orientation(), &subcomponent->orientation);

  for (const auto& binding : proto.bindings()) {
    subcomponent->bindings.insert(from_proto<ParameterBinding, proto::ParameterBinding>(binding));
  }
}

struct Component final {
  using Set = std::unordered_set<Component, HashByIdField<Component, guid::GuidHash>, CompareByIdField<Component>>;

  // The id of a Component is globally unique.
  guid::Guid id{};
  std::string name{};

  const Primitive* primitive{nullptr};
  ParameterBinding::Set bindings{};

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

template <>
void to_proto(const Component& component, proto::Component* proto) {
  proto->set_id(component.id.as_string());
  proto->set_name(component.name);

  if (component.primitive) {
    proto->set_primitive_name(component.primitive->name());
  } else {
    proto->clear_primitive_name();
  }

  for (const auto& binding : component.bindings) {
    to_proto(binding, proto->add_bindings());
  }

  for (const auto& child : component.children) {
    to_proto(child, proto->add_children());
  }

  for (const auto& parameter : component.parameters) {
    to_proto(parameter, proto->add_parameters());
  }

  for (const auto& property : component.properties) {
    to_proto(property, proto->add_properties());
  }
}

template <>
inline void from_proto(const proto::Component& proto, Component* component) {
  component->id = guid::Guid(proto.id());
  component->name = proto.name();
  if (!proto.primitive_name().empty()) {
    component->primitive = Primitive::by_name(proto.primitive_name());
  } else {
    component->primitive = nullptr;
  }

  for (const auto& binding : proto.bindings()) {
    component->bindings.insert(from_proto<ParameterBinding, proto::ParameterBinding>(binding));
  }

  for (const auto& child : proto.children()) {
    component->children.insert(from_proto<Subcomponent, proto::Subcomponent>(child));
  }

  for (const auto& parameter : proto.parameters()) {
    component->parameters.insert(from_proto<Parameter, proto::Parameter>(parameter));
  }

  for (const auto& property : proto.properties()) {
    component->properties.insert(from_proto<Property, proto::Property>(property));
  }
}

}  // namespace pack::component
