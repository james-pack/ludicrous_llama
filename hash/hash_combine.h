#pragma once

#include <cstdint>
#include <functional>

namespace pack::hash {

namespace {

// Note that these two constants are the golden ratio times 2^32 and 2^64. They are arbitrary values that have
// approximately half of their bits set. See https://lkml.org/lkml/2016/4/29/838 and
// http://burtleburtle.net/bob/hash/doobs.html for more information about these constants.
constexpr uint32_t fixed_point_golden_ratio_32() { return 0x9e3779b9; }
constexpr uint64_t fixed_point_golden_ratio_64() { return 0x9e3779b97f4a7c16; }

constexpr std::size_t fixed_point_golden_ratio() {
  if constexpr (sizeof(std::size_t) == 4) {
    return fixed_point_golden_ratio_32();
  } else {
    return fixed_point_golden_ratio_64();
  }
}

}  // namespace

template <typename T>
uint32_t hash_combine_32(uint32_t seed, const T& t) {
  using std::hash;
  seed ^= hash<T>{}(t) + fixed_point_golden_ratio_32() + (seed << 6) + (seed >> 2);
  return seed;
}

template <typename T, typename... U>
uint32_t hash_combine_32(uint32_t seed, const T& t, const U&... u) {
  using std::hash;
  seed ^= hash<T>{}(t) + fixed_point_golden_ratio_32() + (seed << 6) + (seed >> 2);
  return hash_combine_32(seed, u...);
}

template <typename T>
uint64_t hash_combine_64(uint64_t seed, const T& t) {
  using std::hash;
  seed ^= hash<T>{}(t) + fixed_point_golden_ratio_64() + (seed << 12) + (seed >> 4);
  return seed;
}

template <typename T, typename... U>
uint64_t hash_combine_64(uint64_t seed, const T& t, const U&... u) {
  using std::hash;
  seed ^= hash<T>{}(t) + fixed_point_golden_ratio_64() + (seed << 12) + (seed >> 4);
  return hash_combine_64(seed, u...);
}

template <typename T, typename... U>
std::size_t hash_combine(std::size_t seed, const T& t, const U&... u) {
  if constexpr (sizeof(std::size_t) == 4) {
    return hash_combine_32(seed, t, u...);
  } else {
    return hash_combine_64(seed, t, u...);
  }
}

}  // namespace pack::hash
