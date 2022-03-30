#include "gtest/gtest.h"
#include "position/position.h"
#include "position/proto/position.pb.h"
#include "serialization/serialize.h"

namespace pack::position {

TEST(ProtoTest, CanRoundTripEmptyPosition) {
  Position original{};
  proto::Position proto = to_proto<Position, proto::Position>(original);
  Position cycled{from_proto<Position, proto::Position>(proto)};
  EXPECT_EQ(original, cycled);
}

TEST(ProtoTest, CanRoundTripEmptyOrientation) {
  Orientation original{};
  proto::Orientation proto = to_proto<Orientation, proto::Orientation>(original);
  Orientation cycled{from_proto<Orientation, proto::Orientation>(proto)};
  EXPECT_EQ(original, cycled);
}

}  // namespace pack::position
