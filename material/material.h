#pragma once

#include <string>

#include "material/color.h"
#include "material/proto/material.pb.h"
#include "serialization/serialize.h"

namespace pack::material {

struct Material final {
  std::string id{};
  Color ambient{};
  Color diffuse{};
  Color specular{};

  // For compatibility with OpenGL, shininess is on [0, 128].
  float shininess{0.f};

  bool operator==(const Material& rhs) const {
    return id == rhs.id &&              //
           ambient == rhs.ambient &&    //
           diffuse == rhs.diffuse &&    //
           specular == rhs.specular &&  //
           shininess == rhs.shininess;
  }
};

std::string to_string(const Material& material);

}  // namespace pack::material

namespace pack {

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
