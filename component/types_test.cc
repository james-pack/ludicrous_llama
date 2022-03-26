#include "component/types.h"

#include <variant>

#include "gtest/gtest.h"

namespace pack::component {

TEST(TypesTest, CanUseEnumAsVariantIndex) {
  Value value{};
  as_float(value) = 1.f;
  EXPECT_EQ(1.f, as_float(value));
}

}  // namespace pack::component
