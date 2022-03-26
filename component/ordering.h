#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <type_traits>

#include "guid/guid.h"

namespace pack::component {

template <typename T, typename HasherT>
struct HashById final {
  size_t operator()(const T& t) const {
    HasherT hasher{};
    return hasher(t.id());
  }
};

template <typename T, typename HasherT>
struct HashByIdField final {
  size_t operator()(const T& t) const {
    HasherT hasher{};
    return hasher(t.id);
  }
};

template <typename T>
struct CompareById final {
  using is_transparent = std::string;
  bool operator()(const T& lhs, const T& rhs) const { return lhs.id() == rhs.id(); }
  bool operator()(std::string_view lhs, const T& rhs) const { return lhs == rhs.id(); }
  bool operator()(const guid::Guid& lhs, const T& rhs) const { return lhs == rhs.id(); }
};

template <typename T>
struct CompareByIdField final {
  using is_transparent = std::string;
  bool operator()(const T& lhs, const T& rhs) const { return lhs.id == rhs.id; }
  bool operator()(std::string_view lhs, const T& rhs) const { return lhs == rhs.id; }
  bool operator()(const guid::Guid& lhs, const T& rhs) const { return lhs == rhs.id; }
};

template <typename T>
struct OrderById final {
  using is_transparent = std::string;
  bool operator()(const T& lhs, const T& rhs) const { return lhs.id() < rhs.id(); }
  bool operator()(std::string_view lhs, const T& rhs) const { return lhs < rhs.id(); }
  bool operator()(const guid::Guid& lhs, const T& rhs) const { return lhs < rhs.id(); }
};

template <typename T>
struct OrderByIdField final {
  using is_transparent = std::string;
  bool operator()(const T& lhs, const T& rhs) const { return lhs.id < rhs.id; }
  bool operator()(std::string_view lhs, const T& rhs) const { return lhs < rhs.id; }
  bool operator()(const guid::Guid& lhs, const T& rhs) const { return lhs < rhs.id; }
};

template <typename T>
struct OrderByName final {
  using is_transparent = std::string;
  bool operator()(const T& lhs, const T& rhs) const { return lhs.name() < rhs.name(); }
  bool operator()(const T& lhs, std::string_view rhs) const { return lhs.name() < rhs; }
  bool operator()(std::string_view lhs, const T& rhs) const { return lhs < rhs.name(); }
};

template <typename T>
struct OrderByNameField final {
  using is_transparent = std::string;
  bool operator()(const T& lhs, const T& rhs) const { return lhs.name < rhs.name; }
  bool operator()(const T& lhs, std::string_view rhs) const { return lhs.name < rhs; }
  bool operator()(std::string_view lhs, const T& rhs) const { return lhs < rhs.name; }
};

}  // namespace pack::component
