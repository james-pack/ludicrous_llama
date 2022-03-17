#pragma once

#include <array>
#include <string>
#include <string_view>

namespace pack::guid {

class Guid final {
 private:
  std::array<unsigned char, 16> bytes_;

  // This method is OS dependent. Different implementations exist for different OSes or library availability.
  static void initialize(Guid *guid);

  void zero() { std::fill(bytes_.begin(), bytes_.end(), static_cast<unsigned char>(0)); }

 public:
  Guid() { initialize(this); }

  explicit Guid(const std::array<unsigned char, 16> &bytes) : bytes_(bytes) {}
  explicit Guid(std::array<unsigned char, 16> &&bytes) : bytes_(std::move(bytes)) {}
  explicit Guid(std::string_view value);

  Guid(const Guid &rhs) = default;
  Guid &operator=(const Guid &rhs) = default;

  Guid(Guid &&rhs) = default;
  Guid &operator=(Guid &&rhs) = default;

  void swap(Guid &rhs) { bytes_.swap(rhs.bytes_); }

  bool operator==(const Guid &rhs) const { return bytes_ == rhs.bytes_; }
  bool operator!=(const Guid &rhs) const { return bytes_ != rhs.bytes_; }

  std::string as_string() const;
  operator std::string() const { return as_string(); }
  const std::array<unsigned char, 16> &bytes() const { return bytes_; }

  bool is_valid() const {
    for (int i = 0; i < 16; ++i) {
      if (bytes_[i] != 0) {
        return true;
      }
    }
    return false;
  }
};

}  // namespace pack::guid
