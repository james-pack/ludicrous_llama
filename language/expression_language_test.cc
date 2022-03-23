#include "language/expression_language.h"

#include <iostream>

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace pack::language {

TEST(ParserTest, CanParseInteger) {
  constexpr std::string_view expression{"42"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root);
  ASSERT_EQ(1, root->children.size());
  EXPECT_TRUE(root->children[0]->is_type<integer_value>());
  LOG(INFO) << to_string(*root);
}

TEST(ParserTest, CanParseDouble) {
  constexpr std::string_view expression{"42.001"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root);
  ASSERT_EQ(1, root->children.size());
  EXPECT_TRUE(root->children[0]->is_type<float_value>());
  LOG(INFO) << to_string(*root);
}

}  // namespace pack::language
