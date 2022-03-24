#pragma once

#include <array>
#include <string>
#include <string_view>

#include "hash/hash_combine.h"

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

  bool operator==(std::string_view rhs) const { return as_string() == rhs; }

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

bool operator==(std::string_view lhs, const Guid &rhs) { return rhs == lhs; }

struct GuidHash {
  size_t operator()(const pack::guid::Guid &guid) const {
    static_assert(sizeof(uint64_t) == 8, "Reimplement this function if this assumption is incorrect");
    const uint64_t *group1 = reinterpret_cast<const uint64_t *>(&guid.bytes()[0]);
    const uint64_t *group2 = reinterpret_cast<const uint64_t *>(&guid.bytes()[8]);
    return hash::hash_combine(0, *group1, *group2);
  }
};

}  // namespace pack::guid
