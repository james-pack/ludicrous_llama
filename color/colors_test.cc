#include "color/colors.h"

#include <limits>
#include <stdexcept>

#include "gtest/gtest.h"

namespace pack::color {

::testing::AssertionResult has_matching_packed_color_data(const std::string& expected, const std::string& actual,
                                                          int bits_accuracy = 32) {
  if (bits_accuracy == 0) {
    throw std::logic_error("Required accuracy is zero bits?!?");
  }
  if (bits_accuracy > 32) {
    throw std::logic_error("Required accuracy is too large (> 32 bits)");
  }

  if (expected.size() != 4 * sizeof(int32_t)) {
    throw std::logic_error("Inappropriate data size for expected color.");
  }
  if (actual.empty()) {
    return ::testing::AssertionFailure() << "Color data not initialized.";
  }
  if (actual.size() != 4 * sizeof(int32_t)) {
    return ::testing::AssertionFailure() << "Bad size for color data.";
  }

  int32_t allowed_delta;
  // Right shifting the number of bits in a value (32 bits in this case) is undefined behavior, so we handle this case
  // explicitly.
  if (bits_accuracy == 32) {
    allowed_delta = 0;
  } else {
    // Here, bits_accuracy will be in the interval [0, 31].
    allowed_delta = std::numeric_limits<int32_t>::max() >> bits_accuracy;
  }

  int32_t actual_value;
  int32_t expected_value;
  actual_value = *(reinterpret_cast<const int32_t*>(actual.data()) + 0);
  expected_value = *(reinterpret_cast<const int32_t*>(expected.data()) + 0);
  if ((actual_value + allowed_delta < expected_value) || (actual_value - allowed_delta > expected_value)) {
    return ::testing::AssertionFailure() << "Red channel does not match: " << expected_value << " (expected) vs "
                                         << actual_value << " (actual)";
  }
  actual_value = *(reinterpret_cast<const int32_t*>(actual.data()) + 1);
  expected_value = *(reinterpret_cast<const int32_t*>(expected.data()) + 1);
  if ((actual_value + allowed_delta < expected_value) || (actual_value - allowed_delta > expected_value)) {
    return ::testing::AssertionFailure() << "Green channel does not match: " << expected_value << " (expected) vs "
                                         << actual_value << " (actual)";
  }
  actual_value = *(reinterpret_cast<const int32_t*>(actual.data()) + 2);
  expected_value = *(reinterpret_cast<const int32_t*>(expected.data()) + 2);
  if ((actual_value + allowed_delta < expected_value) || (actual_value - allowed_delta > expected_value)) {
    return ::testing::AssertionFailure() << "Blue channel does not match: " << expected_value << " (expected) vs "
                                         << actual_value << " (actual)";
  }
  actual_value = *(reinterpret_cast<const int32_t*>(actual.data()) + 3);
  expected_value = *(reinterpret_cast<const int32_t*>(expected.data()) + 3);
  if ((actual_value + allowed_delta < expected_value) || (actual_value - allowed_delta > expected_value)) {
    return ::testing::AssertionFailure() << "Alpha channel does not match: " << expected_value << " (expected) vs "
                                         << actual_value << " (actual)";
  }

  return ::testing::AssertionSuccess();
}

::testing::AssertionResult has_matching_packed_color_data(Rgba expected, Rgba actual, int bits_accuracy = 32) {
  if (!expected.has_packed()) {
    expected = Colors::as_packed(expected);
  }
  if (!actual.has_packed()) {
    actual = Colors::as_packed(actual);
  }
  return has_matching_packed_color_data(expected.packed().bytes(), actual.packed().bytes(), bits_accuracy);
}

::testing::AssertionResult has_matching_float_color_data(Rgba expected, Rgba actual, int bits_accuracy = 32) {
  if (bits_accuracy == 0) {
    throw std::logic_error("Required accuracy is zero bits?!?");
  }
  if (bits_accuracy > 32) {
    throw std::logic_error("Required accuracy is too large (> 32 bits)");
  }

  if (!expected.has_float_values()) {
    throw std::logic_error("Expected color not represented with float data");
  }
  if (!actual.has_float_values()) {
    throw std::logic_error("Actual color not represented with float data");
  }

  uint32_t allowed_delta;
  // Right shifting the number of bits in a value (32 bits in this case) is undefined behavior, so we handle this case
  // explicitly.
  if (bits_accuracy == 32) {
    allowed_delta = 0;
  } else {
    // Here, bits_accuracy will be in the interval [0, 31].
    allowed_delta = std::numeric_limits<uint32_t>::max() >> bits_accuracy;
  }

  float actual_value;
  float expected_value;
  actual_value = actual.float_values().red();
  expected_value = expected.float_values().red();
  if ((actual_value + allowed_delta < expected_value) || (actual_value - allowed_delta > expected_value)) {
    return ::testing::AssertionFailure() << "Red channel does not match: " << expected_value << " (expected) vs "
                                         << actual_value << " (actual)";
  }
  actual_value = actual.float_values().green();
  expected_value = expected.float_values().green();
  if ((actual_value + allowed_delta < expected_value) || (actual_value - allowed_delta > expected_value)) {
    return ::testing::AssertionFailure() << "Green channel does not match: " << expected_value << " (expected) vs "
                                         << actual_value << " (actual)";
  }
  actual_value = actual.float_values().blue();
  expected_value = expected.float_values().blue();
  if ((actual_value + allowed_delta < expected_value) || (actual_value - allowed_delta > expected_value)) {
    return ::testing::AssertionFailure() << "Blue channel does not match: " << expected_value << " (expected) vs "
                                         << actual_value << " (actual)";
  }
  actual_value = actual.float_values().alpha();
  expected_value = expected.float_values().alpha();
  if ((actual_value + allowed_delta < expected_value) || (actual_value - allowed_delta > expected_value)) {
    return ::testing::AssertionFailure() << "Alpha channel does not match: " << expected_value << " (expected) vs "
                                         << actual_value << " (actual)";
  }

  return ::testing::AssertionSuccess();
}

::testing::AssertionResult has_matching_int_color_data(Rgba expected, Rgba actual, int bits_accuracy = 8) {
  if (bits_accuracy == 0) {
    throw std::logic_error("Required accuracy is zero bits?!?");
  }
  if (bits_accuracy > 8) {
    throw std::logic_error("Required accuracy is too large (> 8 bits)");
  }

  if (!expected.has_int_values()) {
    throw std::logic_error("Expected color not represented with int data");
  }
  if (!actual.has_int_values()) {
    throw std::logic_error("Actual color not represented with int data");
  }

  int32_t allowed_delta;
  // Right shifting the number of bits in a value (32 bits in this case) is undefined behavior, so we handle this case
  // explicitly.
  if (bits_accuracy == 8) {
    allowed_delta = 0;
  } else {
    // Here, bits_accuracy will be in the interval [0, 7].
    allowed_delta = std::numeric_limits<uint8_t>::max() >> bits_accuracy;
  }

  int32_t actual_value;
  int32_t expected_value;
  actual_value = actual.int_values().red();
  expected_value = expected.int_values().red();
  if ((actual_value + allowed_delta < expected_value) || (actual_value - allowed_delta > expected_value)) {
    return ::testing::AssertionFailure() << "Red channel does not match: " << expected_value << " (expected) vs "
                                         << actual_value << " (actual)";
  }
  actual_value = actual.int_values().green();
  expected_value = expected.int_values().green();
  if ((actual_value + allowed_delta < expected_value) || (actual_value - allowed_delta > expected_value)) {
    return ::testing::AssertionFailure() << "Green channel does not match: " << expected_value << " (expected) vs "
                                         << actual_value << " (actual)";
  }
  actual_value = actual.int_values().blue();
  expected_value = expected.int_values().blue();
  if ((actual_value + allowed_delta < expected_value) || (actual_value - allowed_delta > expected_value)) {
    return ::testing::AssertionFailure() << "Blue channel does not match: " << expected_value << " (expected) vs "
                                         << actual_value << " (actual)";
  }
  actual_value = actual.int_values().alpha();
  expected_value = expected.int_values().alpha();
  if ((actual_value + allowed_delta < expected_value) || (actual_value - allowed_delta > expected_value)) {
    return ::testing::AssertionFailure() << "Alpha channel does not match: " << expected_value << " (expected) vs "
                                         << actual_value << " (actual)";
  }

  return ::testing::AssertionSuccess();
}

::testing::AssertionResult has_matching_uint32_color_data(Rgba expected, Rgba actual, int bits_accuracy = 32) {
  if (bits_accuracy == 0) {
    throw std::logic_error("Required accuracy is zero bits?!?");
  }
  if (bits_accuracy > 32) {
    throw std::logic_error("Required accuracy is too large (> 32 bits)");
  }

  if (!expected.has_uint32_values()) {
    throw std::logic_error("Expected color not represented with uint32 data");
  }
  if (!actual.has_uint32_values()) {
    throw std::logic_error("Actual color not represented with uint32 data");
  }

  uint32_t allowed_delta;
  // Right shifting the number of bits in a value (32 bits in this case) is undefined behavior, so we handle this case
  // explicitly.
  if (bits_accuracy == 32) {
    allowed_delta = 0;
  } else {
    // Here, bits_accuracy will be in the interval [0, 31].
    allowed_delta = std::numeric_limits<uint32_t>::max() >> bits_accuracy;
  }

  uint32_t actual_value;
  uint32_t expected_value;
  actual_value = actual.uint32_values().red();
  expected_value = expected.uint32_values().red();
  if ((actual_value + allowed_delta < expected_value) || (actual_value - allowed_delta > expected_value)) {
    return ::testing::AssertionFailure() << "Red channel does not match: " << expected_value << " (expected) vs "
                                         << actual_value << " (actual)";
  }
  actual_value = actual.uint32_values().green();
  expected_value = expected.uint32_values().green();
  if ((actual_value + allowed_delta < expected_value) || (actual_value - allowed_delta > expected_value)) {
    return ::testing::AssertionFailure() << "Green channel does not match: " << expected_value << " (expected) vs "
                                         << actual_value << " (actual)";
  }
  actual_value = actual.uint32_values().blue();
  expected_value = expected.uint32_values().blue();
  if ((actual_value + allowed_delta < expected_value) || (actual_value - allowed_delta > expected_value)) {
    return ::testing::AssertionFailure() << "Blue channel does not match: " << expected_value << " (expected) vs "
                                         << actual_value << " (actual)";
  }
  actual_value = actual.uint32_values().alpha();
  expected_value = expected.uint32_values().alpha();
  if ((actual_value + allowed_delta < expected_value) || (actual_value - allowed_delta > expected_value)) {
    return ::testing::AssertionFailure() << "Alpha channel does not match: " << expected_value << " (expected) vs "
                                         << actual_value << " (actual)";
  }

  return ::testing::AssertionSuccess();
}

::testing::AssertionResult is_same_color(const Rgba& expected, const Rgba& actual, int bits_accuracy = 32) {
  if (bits_accuracy == 0) {
    throw std::logic_error("Required accuracy is zero bits?!?");
  }
  if (bits_accuracy > 32) {
    throw std::logic_error("Required accuracy is too large (> 32 bits)");
  }

  if (actual.color_representation_case() == Rgba::ColorRepresentationCase::COLOR_REPRESENTATION_NOT_SET) {
    return ::testing::AssertionFailure() << "Actual color is missing a color representation";
  }
  if (expected.color_representation_case() == Rgba::ColorRepresentationCase::COLOR_REPRESENTATION_NOT_SET) {
    return ::testing::AssertionFailure() << "Expected color is missing a color representation";
  }
  if (expected.color_representation_case() != actual.color_representation_case()) {
    return ::testing::AssertionFailure() << "Colors are using different representations";
  }

  switch (expected.color_representation_case()) {
    case Rgba::ColorRepresentationCase::kFloatValues:
      return has_matching_float_color_data(expected, actual, bits_accuracy);
    case Rgba::ColorRepresentationCase::kIntValues:
      return has_matching_int_color_data(expected, actual, bits_accuracy);
    case Rgba::ColorRepresentationCase::kUint32Values:
      return has_matching_uint32_color_data(expected, actual, bits_accuracy);
    default:
      return ::testing::AssertionFailure() << "No color representation set";
  }
}

TEST(AsPackedTest, MigratesFromUnitializedData) {
  Rgba uninitialized_color{};
  Rgba packed = Colors::as_packed(uninitialized_color);
  const std::string& bytes{packed.packed().bytes()};
  EXPECT_EQ(4 * sizeof(int32_t), bytes.size());
  for (int i = 0; i < 4; ++i) {
    const int32_t value = *(reinterpret_cast<const int32_t*>(bytes.data()) + i);
    EXPECT_EQ(0, value);
  }
}

TEST(AsPackedTest, CanMigrateColorsFrom32bitInt) {
  Rgba color_32bit{};

  {
    color_32bit.mutable_uint32_values()->set_red(std::numeric_limits<uint32_t>::max());
    color_32bit.mutable_uint32_values()->set_green(std::numeric_limits<uint32_t>::max());
    color_32bit.mutable_uint32_values()->set_blue(std::numeric_limits<uint32_t>::max());
    color_32bit.mutable_uint32_values()->set_alpha(std::numeric_limits<uint32_t>::max());
    Rgba packed = Colors::as_packed(color_32bit);
    const std::string& bytes{packed.packed().bytes()};
    EXPECT_EQ(4 * sizeof(int32_t), bytes.size());
    for (int i = 0; i < 4; ++i) {
      const int32_t value = *(reinterpret_cast<const int32_t*>(bytes.data()) + i);
      EXPECT_EQ(std::numeric_limits<int32_t>::max(), value);
    }
  }

  {
    color_32bit.mutable_int_values()->set_red(0);
    color_32bit.mutable_int_values()->set_green(0);
    color_32bit.mutable_int_values()->set_blue(0);
    color_32bit.mutable_int_values()->set_alpha(0);
    Rgba packed = Colors::as_packed(color_32bit);
    const std::string& bytes{packed.packed().bytes()};
    EXPECT_EQ(4 * sizeof(int32_t), bytes.size());
    for (int i = 0; i < 4; ++i) {
      const int32_t value = *(reinterpret_cast<const int32_t*>(bytes.data()) + i);
      EXPECT_EQ(0, value);
    }
  }
}

TEST(AsPackedTest, CanMigrateColorsFrom8bitInt) {
  Rgba color_8bit{};
  color_8bit.mutable_int_values()->set_red(255);
  color_8bit.mutable_int_values()->set_green(255);
  color_8bit.mutable_int_values()->set_blue(255);
  color_8bit.mutable_int_values()->set_alpha(255);
  Rgba color_32bit{};
  color_32bit.mutable_uint32_values()->set_red(std::numeric_limits<uint32_t>::max());
  color_32bit.mutable_uint32_values()->set_green(std::numeric_limits<uint32_t>::max());
  color_32bit.mutable_uint32_values()->set_blue(std::numeric_limits<uint32_t>::max());
  color_32bit.mutable_uint32_values()->set_alpha(std::numeric_limits<uint32_t>::max());
  EXPECT_TRUE(has_matching_packed_color_data(color_32bit, color_8bit));
}

TEST(AsPackedTest, CanMigrateColorsFromFloat) {
  Rgba color_float{};
  color_float.mutable_float_values()->set_red(1.f);
  color_float.mutable_float_values()->set_green(1.f);
  color_float.mutable_float_values()->set_blue(1.f);
  color_float.mutable_float_values()->set_alpha(1.f);
  Rgba color_32bit{};
  color_32bit.mutable_uint32_values()->set_red(std::numeric_limits<uint32_t>::max());
  color_32bit.mutable_uint32_values()->set_green(std::numeric_limits<uint32_t>::max());
  color_32bit.mutable_uint32_values()->set_blue(std::numeric_limits<uint32_t>::max());
  color_32bit.mutable_uint32_values()->set_alpha(std::numeric_limits<uint32_t>::max());
  EXPECT_TRUE(has_matching_packed_color_data(color_32bit, color_float));
}

TEST(AsPackedTest, NoSwappedChannels) {
  Rgba color_float{};
  Rgba color_8bit{};
  Rgba color_32bit{};
  color_float.mutable_float_values()->set_red(1.f);
  color_8bit.mutable_int_values()->set_red(255);
  color_32bit.mutable_uint32_values()->set_red(std::numeric_limits<uint32_t>::max());
  EXPECT_TRUE(has_matching_packed_color_data(color_8bit, color_float));
  EXPECT_TRUE(has_matching_packed_color_data(color_32bit, color_float));
  color_float.mutable_float_values()->set_red(0.f);
  color_8bit.mutable_int_values()->set_red(0);
  color_32bit.mutable_uint32_values()->set_red(0);

  color_float.mutable_float_values()->set_green(1.f);
  color_8bit.mutable_int_values()->set_green(255);
  color_32bit.mutable_uint32_values()->set_green(std::numeric_limits<uint32_t>::max());
  EXPECT_TRUE(has_matching_packed_color_data(color_8bit, color_float));
  EXPECT_TRUE(has_matching_packed_color_data(color_32bit, color_float));
  color_float.mutable_float_values()->set_green(0.f);
  color_8bit.mutable_int_values()->set_green(0);
  color_32bit.mutable_uint32_values()->set_green(0);

  color_float.mutable_float_values()->set_blue(1.f);
  color_8bit.mutable_int_values()->set_blue(255);
  color_32bit.mutable_uint32_values()->set_blue(std::numeric_limits<uint32_t>::max());
  EXPECT_TRUE(has_matching_packed_color_data(color_8bit, color_float));
  EXPECT_TRUE(has_matching_packed_color_data(color_32bit, color_float));
  color_float.mutable_float_values()->set_blue(0.f);
  color_8bit.mutable_int_values()->set_blue(0);
  color_32bit.mutable_uint32_values()->set_blue(0);

  color_float.mutable_float_values()->set_alpha(1.f);
  color_8bit.mutable_int_values()->set_alpha(255);
  color_32bit.mutable_uint32_values()->set_alpha(std::numeric_limits<uint32_t>::max());
  EXPECT_TRUE(has_matching_packed_color_data(color_8bit, color_float));
  EXPECT_TRUE(has_matching_packed_color_data(color_32bit, color_float));
  color_float.mutable_float_values()->set_alpha(0.f);
  color_8bit.mutable_int_values()->set_alpha(0);
  color_32bit.mutable_uint32_values()->set_alpha(0);
}

TEST(AsPackedTest, NonTrivialValues) {
  Rgba color_float{};
  Rgba color_8bit{};
  Rgba color_32bit{};
  color_float.mutable_float_values()->set_red(0.5f);
  color_8bit.mutable_int_values()->set_red(128);
  color_32bit.mutable_uint32_values()->set_red(std::numeric_limits<uint32_t>::max() / 2);
  EXPECT_TRUE(has_matching_packed_color_data(color_8bit, color_float, 8));
  EXPECT_TRUE(has_matching_packed_color_data(color_32bit, color_float));
  color_float.mutable_float_values()->set_red(0.f);
  color_8bit.mutable_int_values()->set_red(0);
  color_32bit.mutable_uint32_values()->set_red(0);

  color_float.mutable_float_values()->set_green(0.5f);
  color_8bit.mutable_int_values()->set_green(128);
  color_32bit.mutable_uint32_values()->set_green(std::numeric_limits<uint32_t>::max() / 2);
  EXPECT_TRUE(has_matching_packed_color_data(color_8bit, color_float, 8));
  EXPECT_TRUE(has_matching_packed_color_data(color_32bit, color_float));
  color_float.mutable_float_values()->set_green(0.f);
  color_8bit.mutable_int_values()->set_green(0);
  color_32bit.mutable_uint32_values()->set_green(0);

  color_float.mutable_float_values()->set_blue(0.5f);
  color_8bit.mutable_int_values()->set_blue(128);
  color_32bit.mutable_uint32_values()->set_blue(std::numeric_limits<uint32_t>::max() / 2);
  EXPECT_TRUE(has_matching_packed_color_data(color_8bit, color_float, 8));
  EXPECT_TRUE(has_matching_packed_color_data(color_32bit, color_float));
  color_float.mutable_float_values()->set_blue(0.f);
  color_8bit.mutable_int_values()->set_blue(0);
  color_32bit.mutable_uint32_values()->set_blue(0);

  color_float.mutable_float_values()->set_alpha(0.5f);
  color_8bit.mutable_int_values()->set_alpha(128);
  color_32bit.mutable_uint32_values()->set_alpha(std::numeric_limits<uint32_t>::max() / 2);
  EXPECT_TRUE(has_matching_packed_color_data(color_8bit, color_float, 8));
  EXPECT_TRUE(has_matching_packed_color_data(color_32bit, color_float));
  color_float.mutable_float_values()->set_alpha(0.f);
  color_8bit.mutable_int_values()->set_alpha(0);
  color_32bit.mutable_uint32_values()->set_alpha(0);
}

TEST(AsFloatsTest, CanRoundTripViaPacked) {
  constexpr int REQUIRED_ACCURACY_BITS{30};
  for (float value = 0.f; value < 1.f; value += 0.05f) {
    Rgba color_float{};
    color_float.mutable_float_values()->set_red(value);
    EXPECT_TRUE(is_same_color(color_float, Colors::as_floats(Colors::as_packed(color_float)), REQUIRED_ACCURACY_BITS));
    color_float.mutable_float_values()->set_red(0.f);

    color_float.mutable_float_values()->set_green(value);
    EXPECT_TRUE(is_same_color(color_float, Colors::as_floats(Colors::as_packed(color_float)), REQUIRED_ACCURACY_BITS));
    color_float.mutable_float_values()->set_green(0.f);

    color_float.mutable_float_values()->set_blue(value);
    EXPECT_TRUE(is_same_color(color_float, Colors::as_floats(Colors::as_packed(color_float)), REQUIRED_ACCURACY_BITS));
    color_float.mutable_float_values()->set_blue(0.f);

    color_float.mutable_float_values()->set_alpha(value);
    EXPECT_TRUE(is_same_color(color_float, Colors::as_floats(Colors::as_packed(color_float)), REQUIRED_ACCURACY_BITS));
    color_float.mutable_float_values()->set_alpha(0.f);
  }
}

TEST(AsIntsTest, CanRoundTripViaPacked) {
  constexpr int REQUIRED_ACCURACY_BITS{8};
  for (int value = 0; value < 256; value += 5) {
    Rgba color_int{};
    color_int.mutable_int_values()->set_red(value);
    EXPECT_TRUE(is_same_color(color_int, Colors::as_ints(Colors::as_packed(color_int)), REQUIRED_ACCURACY_BITS));
    color_int.mutable_int_values()->set_red(0);

    color_int.mutable_int_values()->set_green(value);
    EXPECT_TRUE(is_same_color(color_int, Colors::as_ints(Colors::as_packed(color_int)), REQUIRED_ACCURACY_BITS));
    color_int.mutable_int_values()->set_green(0);

    color_int.mutable_int_values()->set_blue(value);
    EXPECT_TRUE(is_same_color(color_int, Colors::as_ints(Colors::as_packed(color_int)), REQUIRED_ACCURACY_BITS));
    color_int.mutable_int_values()->set_blue(0);

    color_int.mutable_int_values()->set_alpha(value);
    EXPECT_TRUE(is_same_color(color_int, Colors::as_ints(Colors::as_packed(color_int)), REQUIRED_ACCURACY_BITS));
    color_int.mutable_int_values()->set_alpha(0);
  }
}

TEST(AsIntsTest, CanRoundTripViaPackedStride3) {
  constexpr int REQUIRED_ACCURACY_BITS{8};
  for (int value = 0; value < 256; value += 3) {
    Rgba color_int{};
    color_int.mutable_int_values()->set_red(value);
    EXPECT_TRUE(is_same_color(color_int, Colors::as_ints(Colors::as_packed(color_int)), REQUIRED_ACCURACY_BITS));
    color_int.mutable_int_values()->set_red(0);

    color_int.mutable_int_values()->set_green(value);
    EXPECT_TRUE(is_same_color(color_int, Colors::as_ints(Colors::as_packed(color_int)), REQUIRED_ACCURACY_BITS));
    color_int.mutable_int_values()->set_green(0);

    color_int.mutable_int_values()->set_blue(value);
    EXPECT_TRUE(is_same_color(color_int, Colors::as_ints(Colors::as_packed(color_int)), REQUIRED_ACCURACY_BITS));
    color_int.mutable_int_values()->set_blue(0);

    color_int.mutable_int_values()->set_alpha(value);
    EXPECT_TRUE(is_same_color(color_int, Colors::as_ints(Colors::as_packed(color_int)), REQUIRED_ACCURACY_BITS));
    color_int.mutable_int_values()->set_alpha(0);
  }
}

TEST(AsIntsTest, RoundTripsAreStable) {
  constexpr int REQUIRED_ACCURACY_BITS{8};
  for (int value = 0; value < 256; value += 3) {
    Rgba color_int{};
    color_int.mutable_int_values()->set_red(value);
    EXPECT_TRUE(is_same_color(color_int,
                              Colors::as_ints(Colors::as_packed(
                                  Colors::as_ints(Colors::as_packed(Colors::as_ints(Colors::as_packed(color_int)))))),
                              REQUIRED_ACCURACY_BITS));
    color_int.mutable_int_values()->set_red(0);

    color_int.mutable_int_values()->set_green(value);
    EXPECT_TRUE(is_same_color(color_int,
                              Colors::as_ints(Colors::as_packed(
                                  Colors::as_ints(Colors::as_packed(Colors::as_ints(Colors::as_packed(color_int)))))),
                              REQUIRED_ACCURACY_BITS));
    color_int.mutable_int_values()->set_green(0);

    color_int.mutable_int_values()->set_blue(value);
    EXPECT_TRUE(is_same_color(color_int,
                              Colors::as_ints(Colors::as_packed(
                                  Colors::as_ints(Colors::as_packed(Colors::as_ints(Colors::as_packed(color_int)))))),
                              REQUIRED_ACCURACY_BITS));
    color_int.mutable_int_values()->set_blue(0);

    color_int.mutable_int_values()->set_alpha(value);
    EXPECT_TRUE(is_same_color(color_int,
                              Colors::as_ints(Colors::as_packed(
                                  Colors::as_ints(Colors::as_packed(Colors::as_ints(Colors::as_packed(color_int)))))),
                              REQUIRED_ACCURACY_BITS));
    color_int.mutable_int_values()->set_alpha(0);
  }
}

}  // namespace pack::color
