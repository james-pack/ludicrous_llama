#include "language/expression_language.h"

#include <string>

#include "gtest/gtest.h"
#include "language/parse_tree.h"

namespace pack::language {

TEST(ParserTest, CanParseInteger) {
  ParseStack result{};
  bool success{ExpressionLanguage::parse("42", result)};
  EXPECT_TRUE(success);
  EXPECT_FLOAT_EQ(42, result.finish());
}

TEST(ParserTest, CanParseDecimalFloat) {
  ParseStack result{};
  bool success{ExpressionLanguage::parse("42.001", result)};
  EXPECT_TRUE(success);
  EXPECT_FLOAT_EQ(42.001, result.finish());
}

TEST(ParserTest, DISABLED_CanParseIdBasedIdentifier) {
  ParseStack result{};
  bool success{ExpressionLanguage::parse("#some_id", result)};
  EXPECT_TRUE(success);
  EXPECT_FLOAT_EQ(42.001, result.finish());
}

TEST(ParserTest, CanParseSimpleIntegerAddition) {
  ParseStack result{};
  bool success{ExpressionLanguage::parse("22 + 20", result)};
  EXPECT_TRUE(success);
  EXPECT_FLOAT_EQ(42, result.finish());
}

TEST(ParserTest, CanParseSimpleFloatAddition) {
  ParseStack result{};
  bool success{ExpressionLanguage::parse("22.1 + 19.9", result)};
  EXPECT_TRUE(success);
  EXPECT_FLOAT_EQ(42., result.finish());
}

TEST(ParserTest, CanParseFloatIntegerAddition) {
  ParseStack result{};
  bool success{ExpressionLanguage::parse("22 + 19.9", result)};
  EXPECT_TRUE(success);
  EXPECT_FLOAT_EQ(41.9, result.finish());
}

}  // namespace pack::language
