#include "ui/model/material.h"

#include "color/proto/material.pb.h"
#include "ui/model/color.h"

namespace pack::ui::model {

void Material::from_proto(const color::proto::Material& proto, Material* material) {
  material->id = proto.id();
  Color::from_proto(proto.ambient(), &material->ambient);
  Color::from_proto(proto.diffuse(), &material->diffuse);
  Color::from_proto(proto.specular(), &material->specular);
  material->shininess = proto.shininess();
}

Material Material::from_proto(const color::proto::Material& proto) {
  Material material{};
  from_proto(proto, &material);
  return material;
}

void Material::to_proto(const Material& material, color::proto::Material* proto) {
  proto->set_id(material.id);
  Color::to_proto(material.ambient, proto->mutable_ambient());
  Color::to_proto(material.diffuse, proto->mutable_diffuse());
  Color::to_proto(material.specular, proto->mutable_specular());
  proto->set_shininess(material.shininess);
}

color::proto::Material Material::to_proto(const Material& material) {
  color::proto::Material proto{};
  to_proto(material, &proto);
  return proto;
}

}  // namespace pack::ui::model
