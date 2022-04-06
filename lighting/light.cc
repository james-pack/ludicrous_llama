#include "lighting/light.h"

#include <string>

#include "lighting/proto/light.pb.h"
#include "material/color.h"
#include "material/proto/proto_serialization.h"
#include "position/position.h"
#include "position/proto/proto_serialization.h"
#include "serialization/serialize.h"

namespace pack::lighting {

void Light::from_proto(const proto::Light& proto, Light* light, position::Position* position,
                       position::Orientation* orientation) {
  using std::to_string;

  light->light_num = proto.light_num();
  light->id = "Light " + to_string(light->light_num);
  light->name = light->id;
  light->enabled = proto.enabled();

  pack::from_proto(proto.position(), position);
  pack::from_proto(proto.orientation(), orientation);
  pack::from_proto(proto.ambient(), &light->ambient);
  pack::from_proto(proto.diffuse(), &light->diffuse);
  pack::from_proto(proto.specular(), &light->specular);
}

void Light::to_proto(const Light& light, const position::Position& position, const position::Orientation& orientation,
                     proto::Light* proto) {
  proto->set_light_num(light.light_num);
  pack::to_proto(position, proto->mutable_position());
  pack::to_proto(orientation, proto->mutable_orientation());
  pack::to_proto(light.ambient, proto->mutable_ambient());
  pack::to_proto(light.diffuse, proto->mutable_diffuse());
  pack::to_proto(light.specular, proto->mutable_specular());
  proto->set_enabled(light.enabled);
}

proto::Light Light::to_proto(const Light& light, const position::Position& position,
                             const position::Orientation& orientation) {
  proto::Light proto{};
  to_proto(light, position, orientation, &proto);
  return proto;
}

std::string to_string(const Light& light) {
  using std::to_string;
  return "{id: " + light.id + ", enabled: " + to_string(light.enabled) + ", name: " + light.name +
         ", light_num: " + to_string(light.light_num) + ", ambient: " + to_string(light.ambient) +
         ", diffuse: " + to_string(light.diffuse) + ", specular: " + to_string(light.specular) + "}";
}

}  // namespace pack::lighting
