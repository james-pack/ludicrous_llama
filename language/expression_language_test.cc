#include "language/expression_language.h"

#include <iostream>

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace pack::language {

// Disallowing empty strings, and whitespace-only strings, allows us to fall back to a default behavior when the
// expression is empty without traversing the parse tree. We expect this situation will happen often as the UI will
// provide textboxes for entering expressions, but the user won't fill them in every time, relying on that default
// behavior.
TEST(ParserTest, EmptyStringDoesNotMatchGrammar) {
  constexpr std::string_view expression{""};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_FALSE(root);
}

TEST(ParserTest, SingleSpaceStringDoesNotMatchGrammar) {
  constexpr std::string_view expression{" "};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_FALSE(root);
}

TEST(ParserTest, WhitespaceStringDoesNotMatchGrammar) {
  constexpr std::string_view expression{" \t\n\r"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_FALSE(root);
}

TEST(ParserTest, CanParseInteger) {
  constexpr std::string_view expression{"42"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root);
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  EXPECT_TRUE(root->children[0]->is_type<integer_literal>());
}

TEST(ParserTest, CanParseFloat) {
  constexpr std::string_view expression{"42.001"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root);
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  EXPECT_TRUE(root->children[0]->is_type<float_literal>());
}

TEST(ParserTest, CanParseIntegerExpression) {
  constexpr std::string_view expression{"21+21"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root);
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  ASSERT_EQ(2, root->children[0]->children.size());
  EXPECT_TRUE(root->children[0]->is_type<add>());
  EXPECT_TRUE(root->children[0]->children[0]->is_type<integer_literal>());
  EXPECT_TRUE(root->children[0]->children[1]->is_type<integer_literal>());
}

TEST(ParserTest, CanParseFloatExpression) {
  constexpr std::string_view expression{"21.001+20.999"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root);
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  ASSERT_EQ(2, root->children[0]->children.size());
  EXPECT_TRUE(root->children[0]->is_type<add>());
  EXPECT_TRUE(root->children[0]->children[0]->is_type<float_literal>());
  EXPECT_TRUE(root->children[0]->children[1]->is_type<float_literal>());
}

TEST(ParserTest, CanParseMixedLiteralExpression) {
  constexpr std::string_view expression{"21+20.999"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root);
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  ASSERT_EQ(2, root->children[0]->children.size());
  EXPECT_TRUE(root->children[0]->is_type<add>());
  EXPECT_TRUE(root->children[0]->children[0]->is_type<integer_literal>());
  EXPECT_TRUE(root->children[0]->children[1]->is_type<float_literal>());
}

TEST(ParserTest, ExpressionsCanHaveSpaces) {
  constexpr std::array<std::string_view, 7> expressions{"21 + 20.999", "21 +20.999",  "21+ 20.999",   " 21+20.999",
                                                        "21+20.999 ",  " 21+20.999 ", " 21 + 20.999 "};
  for (const auto expression : expressions) {
    auto root{ExpressionLanguage::parse(expression)};
    ASSERT_TRUE(root) << expression;
    ASSERT_EQ(1, root->children.size());
    ASSERT_EQ(2, root->children[0]->children.size());
    EXPECT_TRUE(root->children[0]->is_type<add>());
    EXPECT_TRUE(root->children[0]->children[0]->is_type<integer_literal>());
    EXPECT_TRUE(root->children[0]->children[1]->is_type<float_literal>());
  }
}

TEST(ParserTest, CanParseAddExpression) {
  constexpr std::string_view expression{"21+21"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root);
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  ASSERT_EQ(2, root->children[0]->children.size());
  EXPECT_TRUE(root->children[0]->is_type<add>());
  EXPECT_TRUE(root->children[0]->children[0]->is_type<integer_literal>());
  EXPECT_TRUE(root->children[0]->children[1]->is_type<integer_literal>());
}

TEST(ParserTest, CanParseSubtractExpression) {
  constexpr std::string_view expression{"63-21"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root);
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  ASSERT_EQ(2, root->children[0]->children.size());
  EXPECT_TRUE(root->children[0]->is_type<subtract>());
  EXPECT_TRUE(root->children[0]->children[0]->is_type<integer_literal>());
  EXPECT_TRUE(root->children[0]->children[1]->is_type<integer_literal>());
}

TEST(ParserTest, CanParseMultiplyExpression) {
  constexpr std::string_view expression{"21*2"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root);
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  ASSERT_EQ(2, root->children[0]->children.size());
  EXPECT_TRUE(root->children[0]->is_type<multiply>());
  EXPECT_TRUE(root->children[0]->children[0]->is_type<integer_literal>());
  EXPECT_TRUE(root->children[0]->children[1]->is_type<integer_literal>());
}

TEST(ParserTest, CanParseDivideExpression) {
  constexpr std::string_view expression{"126/3"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root);
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  ASSERT_EQ(2, root->children[0]->children.size());
  EXPECT_TRUE(root->children[0]->is_type<divide>());
  EXPECT_TRUE(root->children[0]->children[0]->is_type<integer_literal>());
  EXPECT_TRUE(root->children[0]->children[1]->is_type<integer_literal>());
}

TEST(ParserTest, CanParsePowerExpression) {
  constexpr std::string_view expression{"2**3"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root);
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  ASSERT_EQ(2, root->children[0]->children.size());
  EXPECT_TRUE(root->children[0]->is_type<power>());
  EXPECT_TRUE(root->children[0]->children[0]->is_type<integer_literal>());
  EXPECT_TRUE(root->children[0]->children[1]->is_type<integer_literal>());
}

TEST(ParserTest, CanParseTwoOperatorExpression) {
  constexpr std::string_view expression{"5**2 + 17"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root);
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  ASSERT_EQ(2, root->children[0]->children.size());
  ASSERT_EQ(2, root->children[0]->children[0]->children.size());

  EXPECT_TRUE(root->children[0]->is_type<add>());

  EXPECT_TRUE(root->children[0]->children[0]->is_type<power>());
  EXPECT_TRUE(root->children[0]->children[0]->children[0]->is_type<integer_literal>());
  EXPECT_TRUE(root->children[0]->children[0]->children[1]->is_type<integer_literal>());

  EXPECT_TRUE(root->children[0]->children[1]->is_type<integer_literal>());
}

TEST(ParserTest, CanParseZeroArgumentFunctionCall) {
  constexpr std::string_view expression{"func()"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root);
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  EXPECT_TRUE(root->children[0]->is_type<function_call>());
  ASSERT_EQ(1, root->children[0]->children.size());
  EXPECT_TRUE(root->children[0]->children[0]->is_type<function_name>());
}

TEST(ParserTest, CanParseZeroArgumentFunctionCallWithSpaces) {
  constexpr std::array<std::string_view, 4> expressions{" func()", "func() ", "func( )", "func ()"};
  for (const auto expression : expressions) {
    auto root{ExpressionLanguage::parse(expression)};
    ASSERT_TRUE(root) << expression;
    DLOG(INFO) << to_string(*root);
    ASSERT_EQ(1, root->children.size());
    ASSERT_EQ(1, root->children[0]->children.size());
    EXPECT_TRUE(root->children[0]->is_type<function_call>());
    EXPECT_TRUE(root->children[0]->children[0]->is_type<function_name>());
  }
}

TEST(ParserTest, CanParseOneArgumentFunctionCall) {
  constexpr std::string_view expression{"func(42)"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root) << expression;
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  EXPECT_TRUE(root->children[0]->is_type<function_call>());
  ASSERT_EQ(2, root->children[0]->children.size());
  EXPECT_TRUE(root->children[0]->children[0]->is_type<function_name>());
  EXPECT_TRUE(root->children[0]->children[1]->is_type<integer_literal>());
}

TEST(ParserTest, CanParseOneArgumentFunctionCallWithSpaces) {
  constexpr std::array<std::string_view, 5> expressions{" func(42)", "func(42) ", "func( 42)", "func (42)",
                                                        "func( 42 )"};
  for (const auto expression : expressions) {
    auto root{ExpressionLanguage::parse(expression)};
    ASSERT_TRUE(root) << expression;
    DLOG(INFO) << to_string(*root);
    ASSERT_EQ(1, root->children.size()) << expression;
    ASSERT_EQ(2, root->children[0]->children.size()) << expression;
    EXPECT_TRUE(root->children[0]->is_type<function_call>()) << expression;
    EXPECT_TRUE(root->children[0]->children[0]->is_type<function_name>()) << expression;
    EXPECT_TRUE(root->children[0]->children[1]->is_type<integer_literal>()) << expression;
  }
}

TEST(ParserTest, CanParseDirectPropertyReference) {
  constexpr std::string_view expression{"width"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root) << expression;
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  EXPECT_TRUE(root->children[0]->is_type<component_reference>());
}

TEST(ParserTest, CanParseSelfPropertyReference) {
  constexpr std::string_view expression{"./width"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root) << expression;
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  EXPECT_TRUE(root->children[0]->is_type<component_reference>());
}

TEST(ParserTest, CanParseComponentIdPropertyReference) {
  constexpr std::string_view expression{"#component/width"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root) << expression;
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  EXPECT_TRUE(root->children[0]->is_type<component_reference>());
}

TEST(ParserTest, ComponentIdWithoutPropertyReferenceDoesNotParse) {
  constexpr std::string_view expression{"#component"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_FALSE(root) << expression;
}

TEST(ParserTest, CanParseParentComponentPropertyReference) {
  constexpr std::string_view expression{"../width"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root) << expression;
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  EXPECT_TRUE(root->children[0]->is_type<component_reference>());
}

TEST(ParserTest, CanParseGrandparentComponentPropertyReference) {
  constexpr std::string_view expression{"../../width"};
  auto root{ExpressionLanguage::parse(expression)};
  ASSERT_TRUE(root) << expression;
  DLOG(INFO) << to_string(*root);
  ASSERT_EQ(1, root->children.size());
  EXPECT_TRUE(root->children[0]->is_type<component_reference>());
}

TEST(ParserTest, CanParseMultiOperatorExpression) {
  constexpr std::string_view expression{"5**(2 * 1) + 6 * 4 - 7"};
  auto root{ExpressionLanguage::parse(expression)};
  EXPECT_TRUE(root);
  DLOG(INFO) << to_string(*root);
}

TEST(ParserTest, CanParseMultiOperatorExpressionWithFunctionCalls) {
  constexpr std::string_view expression{"f1(5**(2 * 1)) + f2(6 * 4)**32 - 7 + f3()"};
  auto root{ExpressionLanguage::parse(expression)};
  EXPECT_TRUE(root);
  DLOG(INFO) << to_string(*root);
}

}  // namespace pack::language
