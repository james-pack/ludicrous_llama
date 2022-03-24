#include "lighting/lights.h"

#include "color/colors.h"
#include "serialization/light.pb.h"

namespace pack::lighting {

serialization::Light Lights::as_packed(const serialization::Light& light) {
  serialization::Light result{light};
  *result.mutable_position() = light.position();
  *result.mutable_ambient() = color::Colors::as_floats(light.ambient());
  *result.mutable_diffuse() = color::Colors::as_floats(light.diffuse());
  *result.mutable_specular() = color::Colors::as_floats(light.specular());
  return result;
}

}  // namespace pack::lighting
