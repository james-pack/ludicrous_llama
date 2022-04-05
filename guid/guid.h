#pragma once

#include <array>
#include <string>
#include <string_view>

#include "hash/hash_combine.h"

namespace pack::guid {

class Guid final {
 private:
  std::array<unsigned char, 16> bytes_;

  // This method is OS dependent. Different implementations exist for different OSes and library availability.
  static void initialize(Guid *guid);

  void zero() { std::fill(bytes_.begin(), bytes_.end(), static_cast<unsigned char>(0)); }

 public:
  // Allow a Guid to be compared to strings via find() and similar methods when used in a
  // container like std::set.
  using is_transparent = std::string;

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
  bool operator!=(std::string_view rhs) const { return as_string() != rhs; }

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

inline bool operator==(std::string_view lhs, const Guid &rhs) { return rhs == lhs; }
inline bool operator!=(std::string_view lhs, const Guid &rhs) { return rhs != lhs; }

std::string to_string(const Guid &guid);

struct GuidHash {
  size_t operator()(const Guid &guid) const {
    using BytesT = std::invoke_result_t<decltype (&Guid::bytes)()>;
    static_assert(sizeof(BytesT) == 2 * sizeof(uint64_t), "Reimplement this function if this assumption is incorrect");
    const auto &bytes{guid.bytes()};
    const uint64_t *group1 = reinterpret_cast<const uint64_t *>(&bytes[0]);
    const uint64_t *group2 = reinterpret_cast<const uint64_t *>(&bytes[sizeof(uint64_t)]);
    return hash::hash_combine(0, *group1, *group2);
  }
};

}  // namespace pack::guid
