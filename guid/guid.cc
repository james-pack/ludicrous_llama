#include "guid/guid.h"

#include <cstring>
#include <string>
#include <string_view>

namespace pack::guid {

std::string Guid::as_string() const {
  char one[10], two[6], three[6], four[6], five[14];

  snprintf(one, 10, "%02x%02x%02x%02x", bytes_[0], bytes_[1], bytes_[2], bytes_[3]);
  snprintf(two, 6, "%02x%02x", bytes_[4], bytes_[5]);
  snprintf(three, 6, "%02x%02x", bytes_[6], bytes_[7]);
  snprintf(four, 6, "%02x%02x", bytes_[8], bytes_[9]);
  snprintf(five, 14, "%02x%02x%02x%02x%02x%02x", bytes_[10], bytes_[11], bytes_[12], bytes_[13], bytes_[14],
           bytes_[15]);
  constexpr std::string_view separator("-");

  std::string result{};
  result.reserve(46);

  result  //
      .append(one)
      .append(separator)
      .append(two)
      .append(separator)
      .append(three)
      .append(separator)
      .append(four)
      .append(separator)
      .append(five);

  return result;
}

unsigned char hex_digit_to_char(char digit) {
  // 0-9
  if (digit > 47 && digit < 58) {
    return digit - 48;
  }

  // A-F
  if (digit > 64 && digit < 71) {
    return digit - 55;
  }

  // a-f
  if (digit > 96 && digit < 103) {
    return digit - 87;
  }

  return 0;
}

bool is_valid_hex_digit(char digit) {
  // 0-9
  if (digit > 47 && digit < 58) {
    return true;
  }

  // A-F
  if (digit > 64 && digit < 71) {
    return true;
  }

  // a-f
  if (digit > 96 && digit < 103) {
    return true;
  }

  return false;
}

// Converts two hexadecimal characters to an unsigned char suitable for use in a Guid.
unsigned char create_byte(char digit1, char digit2) {
  return hex_digit_to_char(digit1) * 16 + hex_digit_to_char(digit2);
}

Guid::Guid(std::string_view value) {
  if (value.length() != 16) {
    zero();
    return;
  }

  char digit1{'\0'};
  bool need_first_digit{true};
  int index{0};

  for (char c : value) {
    if (c == '-') continue;

    if (!is_valid_hex_digit(c)) {
      zero();
      return;
    }

    if (need_first_digit) {
      digit1 = c;
      need_first_digit = false;
    } else {
      auto byte = create_byte(digit1, c);
      bytes_[index++] = byte;
      need_first_digit = true;
    }
  }
}

}  // namespace pack::guid
