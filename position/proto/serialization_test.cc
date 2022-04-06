#include "gtest/gtest.h"
#include "position/position.h"
#include "position/proto/position.pb.h"
#include "position/proto/proto_serialization.h"
#include "serialization/serialize.h"

namespace pack::position::proto {

TEST(ProtoTest, CanRoundTripEmptyPosition) {
  position::Position original{};
  Position proto = to_proto<position::Position, Position>(original);
  position::Position cycled{from_proto<position::Position, Position>(proto)};
  EXPECT_EQ(original, cycled);
}

TEST(ProtoTest, CanRoundTripEmptyOrientation) {
  position::Orientation original{};
  Orientation proto = to_proto<position::Orientation, Orientation>(original);
  position::Orientation cycled{from_proto<position::Orientation, Orientation>(proto)};
  EXPECT_EQ(original, cycled);
}

}  // namespace pack::position::proto
