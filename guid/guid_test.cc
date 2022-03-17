#include "guid/guid.h"

#include "gtest/gtest.h"

namespace pack::guid {

TEST(GuidTest, CanCompile) {
  Guid guid{};
  EXPECT_TRUE(guid.is_valid());
}

}  // namespace pack::guid
