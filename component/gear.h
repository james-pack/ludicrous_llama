#pragma once

#include "component/parameter.h"
#include "third_party/glfw/glfw.h"

namespace pack::component {

/**
 * Construct a gear as a display list, returning the id of the display list that was created.
 */
GLint build_gear(const ParameterBinding::Set& bindings);

}  // namespace pack::component
