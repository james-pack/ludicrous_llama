#include "gtest/gtest.h"

#include "absl/container/flat_hash_set.h"

TEST(AbslTest, CanCompile) {
  absl::flat_hash_set<int> set1 = { 1, 2, 3, 4};
  set1.insert(5);
  set1.insert(4);
  EXPECT_EQ(5, set1.size());
}
