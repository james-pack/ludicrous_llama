#pragma once

#include <stdexcept>
#include <unordered_map>

#include "entt/entity/registry.hpp"
#include "guid/guid.h"

namespace pack::component {

template <typename EntityT = entt::registry::entity_type>
class ComponentTable final {
 public:
  using entity_type = EntityT;
  using registry_type = entt::basic_registry<entity_type>;
  static constexpr entity_type INVALID_ENTITY{entt::null};

 private:
  using Guid = guid::Guid;
  static_assert(
      !std::is_same<entity_type, Guid>(),
      "This class is not needed if the entity_type is Guid. Note that EnTT was not designed to support non-ordered "
      "ids, like Guids, when this code was written. It's also not clear if basing a registry on the Guid type is "
      "beneficial; EnTT uses ordered types for its entity_types and makes extensive optimizations based on that "
      "ordering.");

  registry_type* registry_;

  std::unordered_map<Guid, entity_type, guid::GuidHash> guids_{};

 public:
  ComponentTable(registry_type& registry) : registry_(&registry) {}

  void add_id(const Guid& guid, entity_type entity) { guids_.emplace(guid, entity); }

  entity_type lookup(const Guid& guid) const {
    auto iter = guids_.find(guid);
    if (iter != guids_.cend()) {
      return iter->second;
    }
    return INVALID_ENTITY;
  }

  size_t size() const { return guids_.size(); }

  const registry_type& registry() const { return *registry_; }
  registry_type& registry() { return *registry_; }

  // Helper methods to look up components in an entity by guid. Note that these are deliberately incomplete; we do not
  // want to reimplement entt::registry.
  template <typename T>
  const T* get(const Guid& guid) const {
    auto iter = guids_.find(guid);
    if (iter != guids_.cend()) {
      return registry_->template try_get<T>(iter->second);
    }
    return nullptr;
  }

  template <typename T>
  T* get(const Guid& guid) {
    auto iter = guids_.find(guid);
    if (iter != guids_.cend()) {
      return registry_->template try_get<T>(iter->second);
    }
    return nullptr;
  }

  template <typename T>
  const T& get_existing(const Guid& guid) const {
    const T* result = get<T>(guid);
    if (result != nullptr) {
      return *result;
    }
    throw std::logic_error("ComponentTable does not contain an entity with Guid '" + guid.as_string() + "'");
  }

  template <typename T>
  T& get_existing(const Guid& guid) {
    T* result = get<T>(guid);
    if (result != nullptr) {
      return *result;
    }
    throw std::logic_error("ComponentTable does not contain an entity with Guid '" + guid.as_string() + "'");
  }
};  // namespace pack::component

}  // namespace pack::component
