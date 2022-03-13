#include "ui/compass_layout.h"

#include <stdexcept>
#include <string>

#include "gtest/gtest.h"
#include "ui/demo_pane.h"

namespace pack::ui {

constexpr auto ALL_REGIONS = {CompassLayout::Region::NORTH, CompassLayout::Region::EAST, CompassLayout::Region::SOUTH,
                              CompassLayout::Region::WEST, CompassLayout::Region::CENTER};
constexpr auto ALL_REGIONS_EXCEPT_CENTER = {CompassLayout::Region::NORTH, CompassLayout::Region::EAST,
                                            CompassLayout::Region::SOUTH, CompassLayout::Region::WEST};

TEST(CompassLayoutTest, GivesZeroWidthHeightForEmptyRegions) {
  using std::to_string;
  CompassLayout layout{};

  for (CompassLayout::Region region : ALL_REGIONS) {
    layout.set_bounds(0, 0, 100, 100);
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(0, width) << "Region: " << to_string(region);
    EXPECT_EQ(0, height) << "Region: " << to_string(region);
  }
}

TEST(CompassLayoutTest, SingleRegionRelativeSizeGetsFullWidthHeight) {
  using std::to_string;
  constexpr int SIZE{100};
  DemoPane pane{};
  for (CompassLayout::Region place : ALL_REGIONS_EXCEPT_CENTER) {
    CompassLayout layout{};
    layout.place(pane, place);
    layout.configure(place, 1.f, CompassLayout::Unit::RELATIVE);
    layout.set_bounds(0, 0, SIZE, SIZE);
    for (CompassLayout::Region region : ALL_REGIONS) {
      const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
      if (region == place) {
        EXPECT_EQ(SIZE, width) << "Region: " << to_string(region);
        EXPECT_EQ(SIZE, height) << "Region: " << to_string(region);
      } else {
        EXPECT_EQ(0, width) << "Region: " << to_string(region);
        EXPECT_EQ(0, height) << "Region: " << to_string(region);
      }
    }
  }
}

TEST(CompassLayoutTest, SingleRegionPixelSizeGetsSpecifiedWidthHeight) {
  using std::to_string;
  constexpr int SIZE{100};
  constexpr int SPECIFIED_SIZE{SIZE / 2};
  DemoPane pane{};
  for (CompassLayout::Region place : ALL_REGIONS_EXCEPT_CENTER) {
    CompassLayout layout{};
    layout.place(pane, place);
    layout.configure(place, SPECIFIED_SIZE, CompassLayout::Unit::PIXEL);
    layout.set_bounds(0, 0, SIZE, SIZE);
    for (CompassLayout::Region region : ALL_REGIONS) {
      const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
      if (region == place) {
        if (place == CompassLayout::Region::NORTH || place == CompassLayout::Region::SOUTH) {
          EXPECT_EQ(SIZE, width) << "Region: " << to_string(region);
          EXPECT_EQ(SPECIFIED_SIZE, height) << "Region: " << to_string(region);
        } else if (place == CompassLayout::Region::EAST || place == CompassLayout::Region::WEST) {
          EXPECT_EQ(SPECIFIED_SIZE, width) << "Region: " << to_string(region);
          EXPECT_EQ(SIZE, height) << "Region: " << to_string(region);
        } else {
          throw std::domain_error("Region '" + to_string(region) + "' not expected to appear in this test");
        }
      } else {
        EXPECT_EQ(0, width) << "Region: " << to_string(region);
        EXPECT_EQ(0, height) << "Region: " << to_string(region);
      }
    }
  }
}

TEST(CompassLayoutTest, EastWestTwoRegionPixelSizeGetsSpecifiedWidthHeight) {
  using std::to_string;
  constexpr int SIZE{100};
  constexpr int SPECIFIED_SIZE{SIZE / 3};
  DemoPane east_pane{};
  DemoPane west_pane{};
  CompassLayout layout{};

  layout.place(east_pane, CompassLayout::Region::EAST);
  layout.configure(CompassLayout::Region::EAST, SPECIFIED_SIZE, CompassLayout::Unit::PIXEL);
  layout.place(west_pane, CompassLayout::Region::WEST);
  layout.configure(CompassLayout::Region::WEST, SPECIFIED_SIZE, CompassLayout::Unit::PIXEL);

  layout.set_bounds(0, 0, SIZE, SIZE);

  {
    CompassLayout::Region region{CompassLayout::Region::EAST};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(SPECIFIED_SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(SIZE, height) << "Region: " << to_string(region);
  }

  {
    CompassLayout::Region region{CompassLayout::Region::WEST};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(SPECIFIED_SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(SIZE, height) << "Region: " << to_string(region);
  }
}

TEST(CompassLayoutTest, NorthSouthTwoRegionPixelSizeGetsSpecifiedWidthHeight) {
  using std::to_string;
  constexpr int SIZE{100};
  constexpr int SPECIFIED_SIZE{SIZE / 3};
  DemoPane north_pane{};
  DemoPane south_pane{};
  CompassLayout layout{};

  layout.place(north_pane, CompassLayout::Region::NORTH);
  layout.configure(CompassLayout::Region::NORTH, SPECIFIED_SIZE, CompassLayout::Unit::PIXEL);
  layout.place(south_pane, CompassLayout::Region::SOUTH);
  layout.configure(CompassLayout::Region::SOUTH, SPECIFIED_SIZE, CompassLayout::Unit::PIXEL);

  layout.set_bounds(0, 0, SIZE, SIZE);

  {
    CompassLayout::Region region{CompassLayout::Region::NORTH};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(SPECIFIED_SIZE, height) << "Region: " << to_string(region);
  }

  {
    CompassLayout::Region region{CompassLayout::Region::SOUTH};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(SPECIFIED_SIZE, height) << "Region: " << to_string(region);
  }
}

TEST(CompassLayoutTest, EastWestTwoRegionRelativeSizeGetsSpecifiedWidthHeight) {
  using std::to_string;
  constexpr int SIZE{100};
  constexpr float SPECIFIED_SIZE{0.5f};
  constexpr int EXPECTED_SIZE{std::lround(SIZE * SPECIFIED_SIZE)};
  DemoPane east_pane{};
  DemoPane west_pane{};
  CompassLayout layout{};

  layout.place(east_pane, CompassLayout::Region::EAST);
  layout.configure(CompassLayout::Region::EAST, SPECIFIED_SIZE, CompassLayout::Unit::RELATIVE);
  layout.place(west_pane, CompassLayout::Region::WEST);
  layout.configure(CompassLayout::Region::WEST, SPECIFIED_SIZE, CompassLayout::Unit::RELATIVE);

  layout.set_bounds(0, 0, SIZE, SIZE);

  {
    CompassLayout::Region region{CompassLayout::Region::EAST};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(EXPECTED_SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(SIZE, height) << "Region: " << to_string(region);
  }

  {
    CompassLayout::Region region{CompassLayout::Region::WEST};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(EXPECTED_SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(SIZE, height) << "Region: " << to_string(region);
  }
}

TEST(CompassLayoutTest, NorthSouthTwoRegionRelativeSizeGetsSpecifiedWidthHeight) {
  using std::to_string;
  constexpr int SIZE{100};
  constexpr float SPECIFIED_SIZE{0.5f};
  constexpr int EXPECTED_SIZE{std::lround(SIZE * SPECIFIED_SIZE)};
  DemoPane north_pane{};
  DemoPane south_pane{};
  CompassLayout layout{};

  layout.place(north_pane, CompassLayout::Region::NORTH);
  layout.configure(CompassLayout::Region::NORTH, SPECIFIED_SIZE, CompassLayout::Unit::RELATIVE);
  layout.place(south_pane, CompassLayout::Region::SOUTH);
  layout.configure(CompassLayout::Region::SOUTH, SPECIFIED_SIZE, CompassLayout::Unit::RELATIVE);

  layout.set_bounds(0, 0, SIZE, SIZE);

  {
    CompassLayout::Region region{CompassLayout::Region::NORTH};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(EXPECTED_SIZE, height) << "Region: " << to_string(region);
  }

  {
    CompassLayout::Region region{CompassLayout::Region::SOUTH};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(EXPECTED_SIZE, height) << "Region: " << to_string(region);
  }
}

TEST(CompassLayoutTest, EastWestThreeRegionRelativeSizeGetsSpecifiedWidthHeight) {
  using std::to_string;
  constexpr int SIZE{100};
  constexpr float SPECIFIED_SIZE{0.2f};
  ASSERT_LT(2.f * SPECIFIED_SIZE, 1.f) << "Test requires the total size for the non-center region be less than 1.";
  constexpr int EXPECTED_SIZE{std::lround(SIZE * SPECIFIED_SIZE)};
  constexpr int EXPECTED_CENTER_SIZE{std::lround(SIZE * (1.f - 2.f * SPECIFIED_SIZE))};
  DemoPane east_pane{};
  DemoPane west_pane{};
  DemoPane center_pane{};
  CompassLayout layout{};

  layout.place(east_pane, CompassLayout::Region::EAST);
  layout.configure(CompassLayout::Region::EAST, SPECIFIED_SIZE, CompassLayout::Unit::RELATIVE);
  layout.place(west_pane, CompassLayout::Region::WEST);
  layout.configure(CompassLayout::Region::WEST, SPECIFIED_SIZE, CompassLayout::Unit::RELATIVE);
  layout.place(center_pane, CompassLayout::Region::CENTER);

  layout.set_bounds(0, 0, SIZE, SIZE);

  {
    CompassLayout::Region region{CompassLayout::Region::EAST};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(EXPECTED_SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(SIZE, height) << "Region: " << to_string(region);
  }

  {
    CompassLayout::Region region{CompassLayout::Region::WEST};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(EXPECTED_SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(SIZE, height) << "Region: " << to_string(region);
  }

  {
    CompassLayout::Region region{CompassLayout::Region::CENTER};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(EXPECTED_CENTER_SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(SIZE, height) << "Region: " << to_string(region);
  }
}

TEST(CompassLayoutTest, NorthSouthThreeRegionRelativeSizeGetsSpecifiedWidthHeight) {
  using std::to_string;
  constexpr int SIZE{100};
  constexpr float SPECIFIED_SIZE{0.2f};
  ASSERT_LT(2.f * SPECIFIED_SIZE, 1.f) << "Test requires the total size for the non-center region be less than 1.";
  constexpr int EXPECTED_SIZE{std::lround(SIZE * SPECIFIED_SIZE)};
  constexpr int EXPECTED_CENTER_SIZE{std::lround(SIZE * (1.f - 2.f * SPECIFIED_SIZE))};
  DemoPane north_pane{};
  DemoPane south_pane{};
  DemoPane center_pane{};
  CompassLayout layout{};

  layout.place(north_pane, CompassLayout::Region::NORTH);
  layout.configure(CompassLayout::Region::NORTH, SPECIFIED_SIZE, CompassLayout::Unit::RELATIVE);
  layout.place(south_pane, CompassLayout::Region::SOUTH);
  layout.configure(CompassLayout::Region::SOUTH, SPECIFIED_SIZE, CompassLayout::Unit::RELATIVE);
  layout.place(center_pane, CompassLayout::Region::CENTER);

  layout.set_bounds(0, 0, SIZE, SIZE);

  {
    CompassLayout::Region region{CompassLayout::Region::NORTH};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(EXPECTED_SIZE, height) << "Region: " << to_string(region);
  }

  {
    CompassLayout::Region region{CompassLayout::Region::SOUTH};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(EXPECTED_SIZE, height) << "Region: " << to_string(region);
  }

  {
    CompassLayout::Region region{CompassLayout::Region::CENTER};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(EXPECTED_CENTER_SIZE, height) << "Region: " << to_string(region);
  }
}

TEST(CompassLayoutTest, EastWestThreeRegionRelativeSizeGivesZeroWidthWhenNoSpaceForCenter) {
  using std::to_string;
  constexpr int SIZE{100};
  constexpr float SPECIFIED_SIZE{0.6f};
  ASSERT_GT(2.f * SPECIFIED_SIZE, 1.f) << "Test requires the total size for the non-center region be greater than 1.";
  constexpr int EXPECTED_SIZE{SIZE / 2};
  constexpr int EXPECTED_CENTER_SIZE{0};
  DemoPane east_pane{};
  DemoPane west_pane{};
  DemoPane center_pane{};
  CompassLayout layout{};

  layout.place(east_pane, CompassLayout::Region::EAST);
  layout.configure(CompassLayout::Region::EAST, SPECIFIED_SIZE, CompassLayout::Unit::RELATIVE);
  layout.place(west_pane, CompassLayout::Region::WEST);
  layout.configure(CompassLayout::Region::WEST, SPECIFIED_SIZE, CompassLayout::Unit::RELATIVE);
  layout.place(center_pane, CompassLayout::Region::CENTER);

  layout.set_bounds(0, 0, SIZE, SIZE);

  {
    CompassLayout::Region region{CompassLayout::Region::EAST};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(EXPECTED_SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(SIZE, height) << "Region: " << to_string(region);
  }

  {
    CompassLayout::Region region{CompassLayout::Region::WEST};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(EXPECTED_SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(SIZE, height) << "Region: " << to_string(region);
  }

  {
    CompassLayout::Region region{CompassLayout::Region::CENTER};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(EXPECTED_CENTER_SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(EXPECTED_CENTER_SIZE, height) << "Region: " << to_string(region);
  }
}

TEST(CompassLayoutTest, NorthSouthThreeRegionRelativeSizeGivesZeroWidthWhenNoSpaceForCenter) {
  using std::to_string;
  constexpr int SIZE{100};
  constexpr float SPECIFIED_SIZE{0.6f};
  ASSERT_GT(2.f * SPECIFIED_SIZE, 1.f) << "Test requires the total size for the non-center region be greater than 1.";
  constexpr int EXPECTED_SIZE{SIZE / 2};
  constexpr int EXPECTED_CENTER_SIZE{0};
  DemoPane north_pane{};
  DemoPane south_pane{};
  DemoPane center_pane{};
  CompassLayout layout{};

  layout.place(north_pane, CompassLayout::Region::NORTH);
  layout.configure(CompassLayout::Region::NORTH, SPECIFIED_SIZE, CompassLayout::Unit::RELATIVE);
  layout.place(south_pane, CompassLayout::Region::SOUTH);
  layout.configure(CompassLayout::Region::SOUTH, SPECIFIED_SIZE, CompassLayout::Unit::RELATIVE);
  layout.place(center_pane, CompassLayout::Region::CENTER);

  layout.set_bounds(0, 0, SIZE, SIZE);

  {
    CompassLayout::Region region{CompassLayout::Region::NORTH};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(EXPECTED_SIZE, height) << "Region: " << to_string(region);
  }

  {
    CompassLayout::Region region{CompassLayout::Region::SOUTH};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(EXPECTED_SIZE, height) << "Region: " << to_string(region);
  }

  {
    CompassLayout::Region region{CompassLayout::Region::CENTER};
    const auto [origin_x, origin_y, width, height] = layout.get_bounds(region);
    EXPECT_EQ(EXPECTED_CENTER_SIZE, width) << "Region: " << to_string(region);
    EXPECT_EQ(EXPECTED_CENTER_SIZE, height) << "Region: " << to_string(region);
  }
}

}  // namespace pack::ui
