#pragma once

#include <string>

#include "component/gear.pb.h"
#include "ui/model/material.h"
#include "ui/model/animate.h"

namespace pack::ui::model {

struct Gear final {
  std::string id{};
  std::string name{};
  float inner_radius{};
  float outer_radius{};
  float width{};
  int32_t teeth{};
  float tooth_depth{};
  float angle_coefficient{};
  float phase{};

  Material material{};

  static Gear from_proto(const component::Gear& proto);
  static void from_proto(const component::Gear& proto, Gear* gear);
  static component::Gear to_proto(const Gear& gear);
  static void to_proto(const Gear& gear, component::Gear* proto);
};

/**
 * Construct a gear as a display list, returning the id of the display list that was created.
 */
int build_gear(const Gear& gear);

Animate construct_gear_animator();

}  // namespace pack::ui::model
