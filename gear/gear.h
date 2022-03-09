#pragma once

#include "color/material.pb.h"
#include "gear/gear.pb.h"

namespace pack::gear {

/**
 * Construct a gear as a display list, returning the id of the display list that was created.
 *
 * Note that the material may be modified, if it is necessary to pack the color data for use with glMaterial().
 */
unsigned int build_gear(const Gear& gear_configuration, color::Material* material);

}  // namespace pack::gear
