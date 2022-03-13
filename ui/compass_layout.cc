#include "ui/compass_layout.h"

#include <array>
#include <stdexcept>
#include <string>
#include <tuple>

namespace pack::ui {

std::string to_string(CompassLayout::Region region) {
  using std::to_string;
  switch (region) {
    case CompassLayout::Region::NORTH:
      return "NORTH";
    case CompassLayout::Region::EAST:
      return "EAST";
    case CompassLayout::Region::SOUTH:
      return "SOUTH";
    case CompassLayout::Region::WEST:
      return "WEST";
    case CompassLayout::Region::CENTER:
      return "CENTER";
    default:
      throw std::domain_error("No to_string() implementation for CompassLayout::Region " +
                              to_string(static_cast<std::underlying_type_t<CompassLayout::Region>>(region)));
  }
}

constexpr auto ALL_REGIONS = {CompassLayout::Region::NORTH, CompassLayout::Region::EAST, CompassLayout::Region::SOUTH,
                              CompassLayout::Region::WEST, CompassLayout::Region::CENTER};

std::tuple<int, int, int> CompassLayout::distribute_space(int size, const RegionConfiguration& region_1,
                                                          const RegionConfiguration& region_2,
                                                          bool allocate_center_space) {
  std::array<int, 3> sizes{0, 0, 0};
  const std::array<const RegionConfiguration*, 2> configurations = {&region_1, &region_2};

  int total_relative_size{size};

  float total_size_weight{0.f};

  for (int i = 0; i < 2; ++i) {
    const RegionConfiguration* config = configurations[i];
    if (config->pane == nullptr) {
      sizes[i] = 0;
    } else {
      if (config->size_unit == CompassLayout::Unit::PIXEL) {
        sizes[i] = config->size;
        total_relative_size -= config->size;
      } else {
        total_size_weight += config->size;
      }
    }
  }

  if (allocate_center_space) {
    if (total_size_weight < 1.f) {
      sizes[2] = total_relative_size * (1.f - total_size_weight);
      total_size_weight = 1.f;
    }
  }
  for (int i = 0; i < 2; ++i) {
    const RegionConfiguration* config = configurations[i];
    if (config->pane != nullptr && config->size_unit == CompassLayout::Unit::RELATIVE) {
      sizes[i] = config->size * total_relative_size / total_size_weight;
    }
  }

  return {sizes[0], sizes[1], sizes[2]};
}

void CompassLayout::set_bounds(int origin_x, int origin_y, int width, int height) {
  bool center_needs_height{false};
  for (Region region : {Region::WEST, Region::CENTER, Region::EAST}) {
    const auto& configuration = configurations_[as_int(region)];
    if (configuration.pane != nullptr) {
      center_needs_height = true;
      break;
    }
  }
  const auto [north_height, south_height, center_height] = distribute_space(
      height, configurations_[as_int(Region::NORTH)], configurations_[as_int(Region::SOUTH)], center_needs_height);

  bool center_needs_width{false};
  for (Region region : {Region::NORTH, Region::CENTER, Region::SOUTH}) {
    const auto& configuration = configurations_[as_int(region)];
    if (configuration.pane != nullptr) {
      center_needs_width = true;
      break;
    }
  }
  const auto [west_width, east_width, center_width] = distribute_space(
      width, configurations_[as_int(Region::WEST)], configurations_[as_int(Region::EAST)], center_needs_width);

  configurations_[as_int(Region::NORTH)].computed_origin_x = origin_x;
  configurations_[as_int(Region::NORTH)].computed_origin_y = origin_y + south_height + center_height;
  configurations_[as_int(Region::NORTH)].computed_width = width;
  configurations_[as_int(Region::NORTH)].computed_height = north_height;

  configurations_[as_int(Region::SOUTH)].computed_origin_x = origin_x;
  configurations_[as_int(Region::SOUTH)].computed_origin_y = origin_y;
  configurations_[as_int(Region::SOUTH)].computed_width = width;
  configurations_[as_int(Region::SOUTH)].computed_height = south_height;

  configurations_[as_int(Region::EAST)].computed_origin_x = origin_x + west_width + center_width;
  configurations_[as_int(Region::EAST)].computed_origin_y = origin_y + south_height;
  configurations_[as_int(Region::EAST)].computed_width = east_width;
  configurations_[as_int(Region::EAST)].computed_height = center_height;

  configurations_[as_int(Region::WEST)].computed_origin_x = origin_x;
  configurations_[as_int(Region::WEST)].computed_origin_y = origin_y + south_height;
  configurations_[as_int(Region::WEST)].computed_width = west_width;
  configurations_[as_int(Region::WEST)].computed_height = center_height;

  configurations_[as_int(Region::CENTER)].computed_origin_x = origin_x + west_width;
  configurations_[as_int(Region::CENTER)].computed_origin_y = origin_y + south_height;
  configurations_[as_int(Region::CENTER)].computed_width = center_width;
  configurations_[as_int(Region::CENTER)].computed_height = center_height;

  // Normalize the width and height for empty regions.
  for (CompassLayout::Region region : ALL_REGIONS) {
    if (configurations_[as_int(region)].computed_width == 0) {
      configurations_[as_int(region)].computed_height = 0;
    }
    if (configurations_[as_int(region)].computed_height == 0) {
      configurations_[as_int(region)].computed_width = 0;
    }
  }
}

}  // namespace pack::ui
