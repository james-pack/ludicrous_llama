#pragma once

#include "component/parameter.h"
#include "material/material.h"
#include "third_party/glfw/glfw.h"

namespace pack::component {

/**
 * Construct a gear as a display list, returning the id of the display list that was created.
 */
GLint build_gear(const ParameterBinding::Set& bindings, const material::Material& material);

}  // namespace pack::component
