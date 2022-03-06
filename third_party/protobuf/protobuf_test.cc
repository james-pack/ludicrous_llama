#include "gtest/gtest.h"

#include "third_party/protobuf/hello.pb.h"

TEST(ProtobufTest, CanCompile) {
  constexpr char MESSAGE[] = "Hello, world!";
  Greeting greeting{};
  greeting.set_message(MESSAGE);
  EXPECT_EQ(MESSAGE, greeting.message());
}
