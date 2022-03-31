#include "gtest/gtest.h"
#include "material/color.h"
#include "material/material.h"
#include "material/proto/color.pb.h"
#include "material/proto/material.pb.h"
#include "serialization/serialize.h"

namespace pack::material {

TEST(ProtoTest, CanRoundTripEmptyColor) {
  Color original{};
  proto::Rgba proto = to_proto<Color, proto::Rgba>(original);
  Color cycled{from_proto<Color, proto::Rgba>(proto)};
  EXPECT_EQ(original, cycled);
}

TEST(ProtoTest, CanRoundTripEmptyMaterial) {
  Material original{};
  proto::Material proto = to_proto<Material, proto::Material>(original);
  Material cycled{from_proto<Material, proto::Material>(proto)};
  EXPECT_EQ(original, cycled);
}

}  // namespace pack::material
