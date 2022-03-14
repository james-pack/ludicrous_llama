#include "lighting/lights.h"

#include "color/colors.h"

namespace pack::lighting {

Light Lights::as_packed(const Light& light) {
  Light result{light};
  *result.mutable_position() = light.position();
  *result.mutable_ambient() = color::Colors::as_floats(light.ambient());
  *result.mutable_diffuse() = color::Colors::as_floats(light.diffuse());
  *result.mutable_specular() = color::Colors::as_floats(light.specular());
  return result;
}

}  // namespace pack::lighting
