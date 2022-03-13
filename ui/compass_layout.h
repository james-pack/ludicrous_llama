#pragma once

#include <array>
#include <stdexcept>
#include <string>
#include <tuple>

#include "ui/pane.h"

namespace pack::ui {

class CompassLayout final {
 public:
  enum class Region {
    NORTH = 0,
    EAST,
    SOUTH,
    WEST,
    CENTER,
  };

  enum class Unit {
    // Value is expressed in the number of pixels.
    PIXEL,
    // Value is expressed as a floating point value on [0, 1], with 1 meaning the maximum available.
    RELATIVE,
  };

  enum class Dominance {
    NORTH_SOUTH,
    EAST_WEST,
  };

 private:
  static constexpr int NUM_REGIONS{5};

  struct RegionConfiguration {
    const Pane* pane{nullptr};

    float size{0.f};
    Unit size_unit{Unit::RELATIVE};
    int computed_origin_x{0};
    int computed_origin_y{0};
    int computed_width{0};
    int computed_height{0};
  };

  std::array<RegionConfiguration, NUM_REGIONS> configurations_{};
  Dominance dominance_{Dominance::NORTH_SOUTH};

  static constexpr auto as_int(Region region) { return static_cast<std::underlying_type_t<Region>>(region); }

  static std::tuple<int, int, int> distribute_space(int size, const RegionConfiguration& region_1,
                                                    const RegionConfiguration& region_2,
                                                    bool allocate_center_space);

 public:
  void place(const Pane& pane, Region region) { configurations_[as_int(region)].pane = &pane; }

  void remove(const Pane& pane) {
    for (auto& config : configurations_) {
      if (config.pane == &pane) {
        config.pane = nullptr;
      }
    }
  }

  void remove_pane(Region region) { configurations_[as_int(region)].pane = nullptr; }

  Dominance dominance() const { return dominance_; }
  void set_dominance(Dominance dominance) { dominance_ = dominance; }

  void configure(Region region, float size, Unit unit) {
    if (region == Region::CENTER) {
      throw std::domain_error(
          "Not allowed to set the size of the CENTER region. The CENTER region gets whatever size isn't allocated by "
          "the other regions.");
    }
    configurations_[as_int(region)].size = size;
    configurations_[as_int(region)].size_unit = unit;
  }

  void set_bounds(int origin_x, int origin_y, int width, int height);

  std::tuple<int, int, int, int> compute_bounds(Region region) const {
    return {configurations_[as_int(region)].computed_origin_x, configurations_[as_int(region)].computed_origin_y,
            configurations_[as_int(region)].computed_width, configurations_[as_int(region)].computed_height};
  }

  std::tuple<int, int, int, int> compute_bounds(const Pane& pane) const {
    for (Region region : {Region::NORTH, Region::EAST, Region::SOUTH, Region::WEST, Region::CENTER}) {
      const RegionConfiguration& config = configurations_[as_int(region)];
      if (config.pane == &pane) {
        return compute_bounds(region);
      }
    }
    return {0, 0, 0, 0};
  }
};

std::string to_string(CompassLayout::Region region);

}  // namespace pack::ui
