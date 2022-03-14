#pragma once

#include "component/gear.pb.h"
#include "third_party/glfw/glfw.h"

namespace pack::component {

/**
 * Construct a gear as a display list, returning the id of the display list that was created.
 *
 * Note that the material(s) in the gear be configured to use packed color data for use with glMaterial().
 */
GLint build_gear(const Gear& gear_configuration);

}  // namespace pack::component
