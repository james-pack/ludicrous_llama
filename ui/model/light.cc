#include "ui/model/light.h"

#include <string>

#include "component/position.h"
#include "lighting/proto/light.pb.h"
#include "ui/model/color.h"

namespace pack::ui::model {

void Light::from_proto(const lighting::proto::Light& proto, Light* light, component::Position* position,
                       component::Orientation* orientation) {
  using std::to_string;

  light->light_num = proto.light_num();
  light->id = "Light " + to_string(light->light_num);
  light->name = light->id;
  light->enabled = proto.enabled();

  component::from_proto(proto.position(), position);
  component::from_proto(proto.orientation(), orientation);
  Color::from_proto(proto.ambient(), &light->ambient);
  Color::from_proto(proto.diffuse(), &light->diffuse);
  Color::from_proto(proto.specular(), &light->specular);
}

void Light::to_proto(const Light& light, const component::Position& position, const component::Orientation& orientation,
                     lighting::proto::Light* proto) {
  proto->set_light_num(light.light_num);
  component::to_proto(position, proto->mutable_position());
  component::to_proto(orientation, proto->mutable_orientation());
  Color::to_proto(light.ambient, proto->mutable_ambient());
  Color::to_proto(light.diffuse, proto->mutable_diffuse());
  Color::to_proto(light.specular, proto->mutable_specular());
  proto->set_enabled(light.enabled);
}

lighting::proto::Light Light::to_proto(const Light& light, const component::Position& position,
                                       const component::Orientation& orientation) {
  lighting::proto::Light proto{};
  to_proto(light, position, orientation, &proto);
  return proto;
}

std::string to_string(const Light& light) {
  using std::to_string;
  return "{id: " + light.id + ", enabled: " + to_string(light.enabled) + ", name: " + light.name +
         ", light_num: " + to_string(light.light_num) + ", ambient: " + to_string(light.ambient) +
         ", diffuse: " + to_string(light.diffuse) + ", specular: " + to_string(light.specular) + "}";
}

}  // namespace pack::ui::model
