#include "lighting/lights.h"

#include "color/colors.h"
#include "component/positions.h"

namespace pack::lighting {

Light Lights::as_packed(const Light& light) {
  Light result{light};
  *result.mutable_position() = component::Positions::as_packed(light.position());
  *result.mutable_ambient() = color::Colors::as_packed(light.ambient());
  *result.mutable_diffuse() = color::Colors::as_packed(light.diffuse());
  *result.mutable_specular() = color::Colors::as_packed(light.specular());
  return result;
}

}  // namespace pack::lighting
