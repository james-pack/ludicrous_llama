#include "component/component.h"
#include "component/components.h"
#include "component/expression.h"
#include "component/parameter.h"
#include "component/property.h"
#include "component/proto/component.pb.h"
#include "component/value.h"
#include "material/proto/proto_serialization.h"
#include "position/proto/proto_serialization.h"
#include "serialization/serialize.h"

namespace pack {

template <>
inline void to_proto(const component::Value& value, component::proto::Value* proto) {
  using std::to_string;
  proto->mutable_type()->set_type(static_cast<component::proto::Type::Types>(value.index()));
  switch (static_cast<component::Type>(value.index())) {
    case component::Type::UNTYPED:
      break;
    case component::Type::FLOAT:
      proto->set_float_value(component::as_float(value));
      break;
    case component::Type::INTEGER:
      proto->set_integer_value(component::as_integer(value));
      break;
    case component::Type::STRING:
      proto->set_string_value(component::as_string(value));
      break;
    case component::Type::ID:
      proto->set_id(component::as_id(value));
      break;
    default:
      throw std::invalid_argument("Unknown Type '" + to_string(value.index()) + "'");
  }
}

template <>
inline void from_proto(const component::proto::Value& proto, component::Value* value) {
  using std::to_string;
  component::Type result_type = static_cast<component::Type>(proto.type().type());
  switch (result_type) {
    case component::Type::UNTYPED:
      value->emplace<component::as_index(component::Type::UNTYPED)>();
      break;
    case component::Type::FLOAT:
      value->emplace<component::as_index(component::Type::FLOAT)>(proto.float_value());
      break;
    case component::Type::INTEGER:
      value->emplace<component::as_index(component::Type::INTEGER)>(proto.integer_value());
      break;
    case component::Type::STRING:
      value->emplace<component::as_index(component::Type::STRING)>(proto.string_value());
      break;
    case component::Type::ID:
      value->emplace<component::as_index(component::Type::ID)>(proto.id());
      break;
    default:
      throw std::invalid_argument("Unknown Type '" + to_string(component::as_index(result_type)) + "'");
  }
}

template <>
inline void to_proto(const component::Precision& value, component::proto::Precision* proto) {
  proto->set_num_decimal_places(value.num_decimal_places);
}

template <>
inline void from_proto(const component::proto::Precision& proto, component::Precision* value) {
  value->num_decimal_places = proto.num_decimal_places();
}

template <>
inline void to_proto(const component::ValueDomain& domain, component::proto::ValueDomain* proto) {
  proto->mutable_type()->set_type(static_cast<component::proto::Type::Types>(domain.type));
  to_proto(domain.min_value, proto->mutable_min_value());
  to_proto(domain.max_value, proto->mutable_max_value());
  to_proto(domain.precision, proto->mutable_precision());
}

template <>
inline void from_proto(const component::proto::ValueDomain& proto, component::ValueDomain* domain) {
  domain->type = static_cast<component::Type>(proto.type().type());
  from_proto(proto.min_value(), &domain->min_value);
  from_proto(proto.max_value(), &domain->max_value);
  from_proto(proto.precision(), &domain->precision);
}

template <>
inline void to_proto(const component::Expression& expr, component::proto::Expression* proto) {
  using std::to_string;
  switch (expr.index()) {
    case 0:
      proto->set_expression(component::as_expression(expr));
      break;
    case 1:
      to_proto(component::as_literal(expr), proto->mutable_literal());
      break;
    default:
      throw std::invalid_argument("Unknown expression storage index '" + to_string(expr.index()) + "'");
  }
}

template <>
inline void from_proto(const component::proto::Expression& proto, component::Expression* expr) {
  if (proto.has_expression()) {
    expr->emplace<0>(proto.expression());
  } else if (proto.has_literal()) {
    expr->emplace<1>(from_proto<component::Value, component::proto::Value>(proto.literal()));
  } else {
    throw std::invalid_argument("Unknown expression proto structure");
  }
}

template <>
inline void to_proto(const component::Property& property, component::proto::Property* proto) {
  proto->set_name(property.name);
  to_proto(property.value, proto->mutable_value());
}

template <>
inline void from_proto(const component::proto::Property& proto, component::Property* property) {
  property->name = proto.name();
  from_proto(proto.value(), &property->value);
}

template <>
inline void to_proto(const component::Parameter& parameter, component::proto::Parameter* proto) {
  proto->set_name(parameter.name);
  to_proto(parameter.domain, proto->mutable_domain());
  to_proto(parameter.default_value, proto->mutable_default_value());
}

template <>
inline void from_proto(const component::proto::Parameter& proto, component::Parameter* parameter) {
  parameter->name = proto.name();
  from_proto(proto.domain(), &parameter->domain);
  from_proto(proto.default_value(), &parameter->default_value);
}

template <>
inline void to_proto(const component::ParameterBinding& binding, component::proto::ParameterBinding* proto) {
  proto->set_name(binding.name);
  to_proto(binding.value, proto->mutable_value());
}

template <>
inline void from_proto(const component::proto::ParameterBinding& proto, component::ParameterBinding* binding) {
  binding->name = proto.name();
  from_proto(proto.value(), &binding->value);
}

template <>
inline void to_proto(const component::Subcomponent& subcomponent, component::proto::Subcomponent* proto) {
  proto->set_child_id(subcomponent.id.as_string());

  to_proto(subcomponent.position, proto->mutable_position());
  to_proto(subcomponent.orientation, proto->mutable_orientation());

  for (const auto& binding : subcomponent.bindings) {
    to_proto(binding, proto->add_bindings());
  }
}

template <>
inline void from_proto(const component::proto::Subcomponent& proto, component::Subcomponent* subcomponent) {
  subcomponent->id = guid::Guid(proto.child_id());

  from_proto(proto.position(), &subcomponent->position);
  from_proto(proto.orientation(), &subcomponent->orientation);

  for (const auto& binding : proto.bindings()) {
    subcomponent->bindings.insert(from_proto<component::ParameterBinding, component::proto::ParameterBinding>(binding));
  }
}

template <>
inline void to_proto(const component::Component& component, component::proto::Component* proto) {
  proto->set_id(component.id.as_string());
  proto->set_name(component.name);

  if (component.primitive) {
    proto->set_primitive_name(std::string(component.primitive->name()));
  } else {
    proto->clear_primitive_name();
  }

  for (const auto& binding : component.bindings) {
    to_proto(binding, proto->add_bindings());
  }

  to_proto(component.material, proto->mutable_material());

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
inline void from_proto(const component::proto::Component& proto, component::Component* component) {
  component->id = guid::Guid(proto.id());
  component->name = proto.name();
  if (!proto.primitive_name().empty()) {
    component->primitive = component::Primitive::by_name(proto.primitive_name());
  } else {
    component->primitive = nullptr;
  }

  for (const auto& binding : proto.bindings()) {
    component->bindings.insert(from_proto<component::ParameterBinding, component::proto::ParameterBinding>(binding));
  }

  from_proto(proto.material(), &component->material);

  for (const auto& child : proto.children()) {
    component->children.insert(from_proto<component::Subcomponent, component::proto::Subcomponent>(child));
  }

  for (const auto& parameter : proto.parameters()) {
    component->parameters.insert(from_proto<component::Parameter, component::proto::Parameter>(parameter));
  }

  for (const auto& property : proto.properties()) {
    component->properties.insert(from_proto<component::Property, component::proto::Property>(property));
  }
}

template <>
inline void to_proto(const component::Components& components, component::proto::Components* proto) {
  for (const auto& component : components) {
    to_proto(component, proto->add_components());
  }
}

template <>
inline void from_proto(const component::proto::Components& proto, component::Components* components) {
  for (const auto& component : proto.components()) {
    components->insert(from_proto<component::Component, component::proto::Component>(component));
  }
}

}  // namespace pack
