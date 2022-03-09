#include "color/colors.h"

#include <limits>
#include <stdexcept>
#include <string>

#include "color/color.pb.h"

namespace pack::color {

Rgba Colors::as_packed(const Rgba& color) {
  if (color.has_packed()) {
    return color;
  }

  Rgba result{};
  std::string* bytes = result.mutable_packed()->mutable_bytes();
  bytes->resize(4 * sizeof(int32_t), 0);
  constexpr int32_t MAX_VALUE{std::numeric_limits<int32_t>::max()};
  int32_t values[4];
  switch (color.color_representation_case()) {
    case Rgba::ColorRepresentationCase::kFloatValues: {
      values[0] = static_cast<double>(color.float_values().red()) * MAX_VALUE;
      values[1] = static_cast<double>(color.float_values().green()) * MAX_VALUE;
      values[2] = static_cast<double>(color.float_values().blue()) * MAX_VALUE;
      values[3] = static_cast<double>(color.float_values().alpha()) * MAX_VALUE;
      break;
    }
    case Rgba::ColorRepresentationCase::kIntValues: {
      constexpr double scale = static_cast<double>(MAX_VALUE) / 255;
      values[0] = color.int_values().red() * scale;
      values[1] = color.int_values().green() * scale;
      values[2] = color.int_values().blue() * scale;
      values[3] = color.int_values().alpha() * scale;
      break;
    }
    case Rgba::ColorRepresentationCase::kUint32Values:
      values[0] = color.uint32_values().red() >> 1;
      values[1] = color.uint32_values().green() >> 1;
      values[2] = color.uint32_values().blue() >> 1;
      values[3] = color.uint32_values().alpha() >> 1;
      break;
    default:
      values[0] = 0;
      values[1] = 0;
      values[2] = 0;
      values[3] = 0;
  }
  std::memcpy(bytes->data(), values, 4 * sizeof(int32_t));

  return result;
}

Rgba Colors::as_floats(const Rgba& color) {
  if (color.has_float_values()) {
    return color;
  }

  Rgba result{};
  switch (color.color_representation_case()) {
    case Rgba::ColorRepresentationCase::kPacked: {
      const std::string& bytes = color.packed().bytes();
      if (bytes.length() != 4 * sizeof(int32_t)) {
        throw std::logic_error("Color in invalid format");
      }
      constexpr double scale = 1. / std::numeric_limits<int32_t>::max();
      result.mutable_float_values()->set_red(*(reinterpret_cast<const int32_t*>(bytes.data()) + 0) * scale);
      result.mutable_float_values()->set_green(*(reinterpret_cast<const int32_t*>(bytes.data()) + 1) * scale);
      result.mutable_float_values()->set_blue(*(reinterpret_cast<const int32_t*>(bytes.data()) + 2) * scale);
      result.mutable_float_values()->set_alpha(*(reinterpret_cast<const int32_t*>(bytes.data()) + 3) * scale);
      break;
    }
    case Rgba::ColorRepresentationCase::kIntValues: {
      constexpr double scale = 1. / 255;
      result.mutable_float_values()->set_red(color.int_values().red() * scale);
      result.mutable_float_values()->set_green(color.int_values().green() * scale);
      result.mutable_float_values()->set_blue(color.int_values().blue() * scale);
      result.mutable_float_values()->set_alpha(color.int_values().alpha() * scale);
      break;
    }
    case Rgba::ColorRepresentationCase::kUint32Values: {
      constexpr double scale = 1. / std::numeric_limits<uint32_t>::max();
      result.mutable_float_values()->set_red(color.int_values().red() * scale);
      result.mutable_float_values()->set_green(color.int_values().green() * scale);
      result.mutable_float_values()->set_blue(color.int_values().blue() * scale);
      result.mutable_float_values()->set_alpha(color.int_values().alpha() * scale);
      break;
    }
    default:
      result.mutable_float_values()->set_red(0.f);
      result.mutable_float_values()->set_green(0.f);
      result.mutable_float_values()->set_blue(0.f);
      result.mutable_float_values()->set_alpha(0.f);
  }

  return result;
}

Rgba Colors::as_ints(const Rgba& color) {
  if (color.has_int_values()) {
    return color;
  }

  Rgba result{};
  switch (color.color_representation_case()) {
    case Rgba::ColorRepresentationCase::kPacked: {
      const std::string& bytes = color.packed().bytes();
      if (bytes.length() != 4 * sizeof(int32_t)) {
        throw std::logic_error("Color in invalid format");
      }
      constexpr double scale = 256. / std::numeric_limits<int32_t>::max();
      result.mutable_int_values()->set_red(*(reinterpret_cast<const int32_t*>(bytes.data()) + 0) * scale);
      result.mutable_int_values()->set_green(*(reinterpret_cast<const int32_t*>(bytes.data()) + 1) * scale);
      result.mutable_int_values()->set_blue(*(reinterpret_cast<const int32_t*>(bytes.data()) + 2) * scale);
      result.mutable_int_values()->set_alpha(*(reinterpret_cast<const int32_t*>(bytes.data()) + 3) * scale);
      break;
    }
    case Rgba::ColorRepresentationCase::kFloatValues: {
      constexpr double scale = 256.;
      result.mutable_int_values()->set_red(color.float_values().red() * scale);
      result.mutable_int_values()->set_green(color.float_values().green() * scale);
      result.mutable_int_values()->set_blue(color.float_values().blue() * scale);
      result.mutable_int_values()->set_alpha(color.float_values().alpha() * scale);
      break;
    }
    case Rgba::ColorRepresentationCase::kUint32Values: {
      constexpr double scale = 256. / std::numeric_limits<uint32_t>::max();
      result.mutable_int_values()->set_red(color.uint32_values().red() * scale);
      result.mutable_int_values()->set_green(color.uint32_values().green() * scale);
      result.mutable_int_values()->set_blue(color.uint32_values().blue() * scale);
      result.mutable_int_values()->set_alpha(color.uint32_values().alpha() * scale);
      break;
    }
    default:
      result.mutable_int_values()->set_red(0);
      result.mutable_int_values()->set_green(0);
      result.mutable_int_values()->set_blue(0);
      result.mutable_int_values()->set_alpha(0);
  }

  // Clamp maximum value to 255.
  if (result.int_values().red() > 255) {
    result.mutable_int_values()->set_red(255);
  }
  if (result.int_values().green() > 255) {
    result.mutable_int_values()->set_green(255);
  }
  if (result.int_values().blue() > 255) {
    result.mutable_int_values()->set_blue(255);
  }
  if (result.int_values().alpha() > 255) {
    result.mutable_int_values()->set_alpha(255);
  }

  return result;
}

Rgba Colors::as_uint32s(const Rgba& color) {
  if (color.has_uint32_values()) {
    return color;
  }

  Rgba result{};
  switch (color.color_representation_case()) {
    case Rgba::ColorRepresentationCase::kPacked: {
      const std::string& bytes = color.packed().bytes();
      if (bytes.length() != 4 * sizeof(int32_t)) {
        throw std::logic_error("Color in invalid format");
      }
      constexpr double scale =
          static_cast<double>(std::numeric_limits<uint32_t>::max()) / std::numeric_limits<int32_t>::max();
      result.mutable_uint32_values()->set_red(*(reinterpret_cast<const int32_t*>(bytes.data()) + 0) * scale);
      result.mutable_uint32_values()->set_green(*(reinterpret_cast<const int32_t*>(bytes.data()) + 1) * scale);
      result.mutable_uint32_values()->set_blue(*(reinterpret_cast<const int32_t*>(bytes.data()) + 2) * scale);
      result.mutable_uint32_values()->set_alpha(*(reinterpret_cast<const int32_t*>(bytes.data()) + 3) * scale);
      break;
    }
    case Rgba::ColorRepresentationCase::kFloatValues: {
      constexpr double scale = std::numeric_limits<int32_t>::max();
      result.mutable_uint32_values()->set_red(color.float_values().red() * scale);
      result.mutable_uint32_values()->set_green(color.float_values().green() * scale);
      result.mutable_uint32_values()->set_blue(color.float_values().blue() * scale);
      result.mutable_uint32_values()->set_alpha(color.float_values().alpha() * scale);
      break;
    }
    case Rgba::ColorRepresentationCase::kIntValues: {
      constexpr double scale = std::numeric_limits<uint32_t>::max() / 255.;
      result.mutable_uint32_values()->set_red(color.int_values().red() * scale);
      result.mutable_uint32_values()->set_green(color.int_values().green() * scale);
      result.mutable_uint32_values()->set_blue(color.int_values().blue() * scale);
      result.mutable_uint32_values()->set_alpha(color.int_values().alpha() * scale);
      break;
    }
    default:
      result.mutable_uint32_values()->set_red(0);
      result.mutable_uint32_values()->set_green(0);
      result.mutable_uint32_values()->set_blue(0);
      result.mutable_uint32_values()->set_alpha(0);
  }

  return result;
}

}  // namespace pack::color
