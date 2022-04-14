#include "gtest/gtest.h"
#include "render/camera.h"
#include "render/proto/proto_serialization.h"
#include "render/scene.h"
#include "serialization/serialize.h"

namespace pack::render {

TEST(ProtoTest, CanRoundTripEmptyCamera) {
  Camera original{};
  proto::Camera proto = to_proto<Camera, proto::Camera>(original);
  Camera cycled{from_proto<Camera, proto::Camera>(proto)};
  EXPECT_EQ(original, cycled);
}

TEST(ProtoTest, CanRoundTripEmptyScene) {
  Scene original{};
  proto::Scene proto = to_proto<Scene, proto::Scene>(original);
  Scene cycled{from_proto<Scene, proto::Scene>(proto)};
  EXPECT_EQ(original, cycled);
}

}  // namespace pack::render
