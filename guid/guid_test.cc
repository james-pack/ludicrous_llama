#include "guid/guid.h"

#include <string>
#include <string_view>
#include <unordered_set>

#include "gtest/gtest.h"

namespace pack::guid {

TEST(GuidTest, CanCompile) {
  Guid guid{};
  EXPECT_TRUE(guid.is_valid());
}

TEST(GuidTest, CanBeUsedInUnorderedSet) {
  Guid guid{};
  std::unordered_set<Guid, GuidHash> guids{};
  guids.insert(guid);
  EXPECT_TRUE(guids.find(guid) != guids.cend());
}

TEST(GuidTest, CanCompareToStringForm) {
  Guid guid{};
  const std::string guid_str{guid.as_string()};
  EXPECT_EQ(guid, guid_str);
  EXPECT_EQ(guid_str, guid);
}

TEST(GuidTest, CanCreateFromString) {
  Guid guid1{};
  const std::string guid1_str{guid1.as_string()};
  Guid guid2{guid1_str};
  EXPECT_EQ(guid1, guid2);
}

TEST(GuidTest, ThrowsForNonGuidString) {
  const std::string not_a_guid{"Not a GUID"};
  EXPECT_ANY_THROW(Guid{not_a_guid});
}

}  // namespace pack::guid
