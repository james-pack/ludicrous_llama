#include "gtest/gtest.h"
#include "material/color.h"
#include "material/material.h"
#include "material/proto/color.pb.h"
#include "material/proto/material.pb.h"
#include "material/proto/proto_serialization.h"
#include "serialization/serialize.h"

namespace pack::material::proto {

TEST(ProtoTest, CanRoundTripEmptyColor) {
  material::Color original{};
  Rgba proto = to_proto<material::Color, Rgba>(original);
  material::Color cycled{from_proto<material::Color, Rgba>(proto)};
  EXPECT_EQ(original, cycled);
}

TEST(ProtoTest, CanRoundTripEmptyMaterial) {
  material::Material original{};
  Material proto = to_proto<material::Material, Material>(original);
  material::Material cycled{from_proto<material::Material, Material>(proto)};
  EXPECT_EQ(original, cycled);
}

}  // namespace pack::material::proto
