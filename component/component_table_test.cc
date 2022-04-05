#include "component/component_table.h"

#include "entt/entity/registry.hpp"
#include "gtest/gtest.h"
#include "guid/guid.h"

namespace pack::component {

using guid::Guid;

TEST(ComponentTableTest, InitializesToEmpty) {
  entt::registry registry{};
  ComponentTable table{registry};
  EXPECT_EQ(0, table.size());
}

TEST(ComponentTableTest, CanAddEntity) {
  entt::registry registry{};
  ComponentTable table{registry};
  const auto entity = registry.create();
  Guid guid{};
  registry.emplace<Guid>(entity, guid);
  table.add_id(guid, entity);
  EXPECT_EQ(1, table.size());
  EXPECT_EQ(entity, table.lookup(guid));
}

TEST(ComponentTableTest, CanAddMultipleEntities) {
  entt::registry registry{};
  ComponentTable table{registry};

  Guid guid1{};
  const auto entity1 = registry.create();
  registry.emplace<Guid>(entity1, guid1);
  table.add_id(guid1, entity1);

  Guid guid2{};
  const auto entity2 = registry.create();
  registry.emplace<Guid>(entity2, guid2);
  table.add_id(guid2, entity2);

  Guid guid3{};
  const auto entity3 = registry.create();
  registry.emplace<Guid>(entity3, guid3);
  table.add_id(guid3, entity3);

  EXPECT_EQ(3, table.size());

  EXPECT_EQ(entity1, table.lookup(guid1));
  EXPECT_EQ(entity2, table.lookup(guid2));
  EXPECT_EQ(entity3, table.lookup(guid3));
}

TEST(ComponentTableTest, CanGetComponentsByGuid) {
  entt::registry registry{};
  ComponentTable table{registry};

  Guid guid1{};
  const auto entity1 = registry.create();
  registry.emplace<Guid>(entity1, guid1);
  registry.emplace<int>(entity1, 1);
  table.add_id(guid1, entity1);

  Guid guid2{};
  const auto entity2 = registry.create();
  registry.emplace<Guid>(entity2, guid2);
  registry.emplace<int>(entity2, 2);
  table.add_id(guid2, entity2);

  Guid guid3{};
  const auto entity3 = registry.create();
  registry.emplace<Guid>(entity3, guid3);
  registry.emplace<int>(entity3, 3);
  table.add_id(guid3, entity3);

  EXPECT_EQ(3, table.size());

  EXPECT_EQ(1, table.get_existing<int>(guid1));
  EXPECT_EQ(2, table.get_existing<int>(guid2));
  EXPECT_EQ(3, table.get_existing<int>(guid3));

  EXPECT_EQ(nullptr, table.get<int>(Guid{}));
}

TEST(ComponentTableTest, CanUseNonDefaultEntityType) {
  using EntityT = uint64_t;
  static_assert(!std::is_same<EntityT, entt::entt_traits<entt::registry::entity_type>::entity_type>());
  // Note that the default entity_type is based on uint32_t.
  static_assert(std::is_same<uint32_t, entt::entt_traits<entt::registry::entity_type>::entity_type>());

  entt::basic_registry<EntityT> registry{};
  ComponentTable<EntityT> table{registry};

  Guid guid1{};
  const auto entity1 = registry.create();
  registry.emplace<Guid>(entity1, guid1);
  table.add_id(guid1, entity1);

  Guid guid2{};
  const auto entity2 = registry.create();
  registry.emplace<Guid>(entity2, guid2);
  table.add_id(guid2, entity2);

  Guid guid3{};
  const auto entity3 = registry.create();
  registry.emplace<Guid>(entity3, guid3);
  table.add_id(guid3, entity3);

  EXPECT_EQ(3, table.size());

  EXPECT_EQ(entity1, table.lookup(guid1));
  EXPECT_EQ(entity2, table.lookup(guid2));
  EXPECT_EQ(entity3, table.lookup(guid3));
}

TEST(ComponentTableTest, LookupMissingEntityGivesInvalidEntity) {
  entt::registry registry{};
  ComponentTable table{registry};
  Guid guid{};
  EXPECT_EQ(ComponentTable<>::INVALID_ENTITY, table.lookup(guid));
}

TEST(ComponentTableTest, InvalidEntityIsNotValidInRegistry) {
  entt::registry registry{};
  ComponentTable table{registry};
  Guid guid{};
  EXPECT_FALSE(registry.valid(table.lookup(guid)));
}

}  // namespace pack::component
