#include "material/color.h"
#include "material/material.h"
#include "material/proto/color.pb.h"
#include "material/proto/colors.h"
#include "material/proto/material.pb.h"
#include "serialization/serialize.h"

namespace pack {

template <>
inline void to_proto(const material::Color& color, material::proto::Rgba* proto) {
  material::proto::Colors::from_array(color.values, proto);
}

template <>
inline void from_proto(const material::proto::Rgba& proto, material::Color* color) {
  material::proto::Colors::to_array(proto, color->values);
}

template <>
inline void from_proto(const material::proto::Material& proto, material::Material* material) {
  material->id = proto.id();
  from_proto(proto.ambient(), &material->ambient);
  from_proto(proto.diffuse(), &material->diffuse);
  from_proto(proto.specular(), &material->specular);
  material->shininess = proto.shininess();
}

template <>
inline void to_proto(const material::Material& material, material::proto::Material* proto) {
  proto->set_id(material.id);
  to_proto(material.ambient, proto->mutable_ambient());
  to_proto(material.diffuse, proto->mutable_diffuse());
  to_proto(material.specular, proto->mutable_specular());
  proto->set_shininess(material.shininess);
}

}  // namespace pack
