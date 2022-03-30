#include "component/value.h"

#include "gtest/gtest.h"

namespace pack::component {

TEST(ValueTest, CanUseEnumAsVariantIndex) {
  Value value{};
  as_float(value) = 1.f;
  EXPECT_EQ(1.f, as_float(value));
}

}  // namespace pack::component
