#include "entt/entity/observer.hpp"
#include "entt/entity/registry.hpp"
#include "gtest/gtest.h"

namespace {

struct Foo {
  int a{0};
};

struct Bar {
  int b{0};
};

TEST(EnttTest, CanAddToRegistry) {
  constexpr int VALUE{1};
  entt::registry registry{};

  {
    const auto foo = registry.create();
    registry.emplace<Foo>(foo, Foo{VALUE});

    const auto bar = registry.create();
    registry.emplace<Bar>(bar, Bar{VALUE});
  }

  int num_found{0};
  {
    auto foos = registry.view<Foo>();
    foos.each([&num_found, VALUE](const Foo& foo) {
      ++num_found;
      EXPECT_EQ(VALUE, foo.a);
    });
    EXPECT_EQ(1, num_found);
  }

  num_found = 0;

  auto bars = registry.view<Bar>();
  bars.each([&num_found, VALUE](const Bar& bar) {
    ++num_found;
    EXPECT_EQ(VALUE, bar.b);
  });
  EXPECT_EQ(1, num_found);
}

TEST(EnttTest, CanAddCompoundEntriesToRegistry) {
  constexpr int VALUE1{1};
  constexpr int VALUE2{2};
  entt::registry registry{};

  const auto foobar1 = registry.create();
  registry.emplace<Foo>(foobar1, Foo{VALUE1});
  registry.emplace<Bar>(foobar1, Bar{VALUE1});

  const auto foobar2 = registry.create();
  registry.emplace<Foo>(foobar2, Foo{VALUE2});
  registry.emplace<Bar>(foobar2, Bar{VALUE2});

  int num_found{0};
  {
    auto foos = registry.view<Foo>();
    foos.each([&num_found, VALUE1, VALUE2, &foobar1, &foobar2](const auto entity, const Foo& foo) {
      ++num_found;
      if (entity == foobar1) {
        EXPECT_EQ(VALUE1, foo.a);
      } else {
        EXPECT_EQ(VALUE2, foo.a);
      }
    });
    EXPECT_EQ(2, num_found);
  }

  num_found = 0;

  auto bars = registry.view<Bar>();
  bars.each([&num_found, VALUE1, VALUE2, &foobar1, &foobar2](const auto entity, const Bar& bar) {
    ++num_found;
    if (entity == foobar1) {
      EXPECT_EQ(VALUE1, bar.b);
    } else {
      EXPECT_EQ(VALUE2, bar.b);
    }
  });
  EXPECT_EQ(2, num_found);
}

TEST(EnttTest, CanUpdateEntriesDirectlyInLambda) {
  constexpr int VALUE1{1};
  constexpr int VALUE2{2};
  entt::registry registry{};

  const auto foobar1 = registry.create();
  registry.emplace<Foo>(foobar1, Foo{VALUE1});
  registry.emplace<Bar>(foobar1, Bar{VALUE1});

  {
    auto foos = registry.view<Foo>();
    foos.each([VALUE2](Foo& foo) { foo.a = VALUE2; });
  }

  int num_found{0};
  auto bars = registry.view<Foo, Bar>();
  bars.each([&num_found, VALUE1, VALUE2, &foobar1](const auto entity, const Foo& foo, const Bar& bar) {
    ++num_found;
    EXPECT_EQ(foobar1, entity);
    EXPECT_EQ(VALUE1, bar.b);
    EXPECT_EQ(VALUE2, foo.a);
  });
  EXPECT_EQ(1, num_found);
}

TEST(EnttTest, CanUpdateEntriesViaPatchMethod) {
  constexpr int VALUE1{1};
  constexpr int VALUE2{2};
  entt::registry registry{};

  const auto foobar1 = registry.create();
  registry.emplace<Foo>(foobar1, Foo{VALUE1});
  registry.emplace<Bar>(foobar1, Bar{VALUE1});

  {
    auto foos = registry.view<Foo>();
    foos.each([&registry, VALUE2](const auto entity, const Foo& /*ignored*/) {  //
      registry.patch<Foo>(entity, [VALUE2](auto& foo) { foo.a = VALUE2; });
    });
  }

  int num_found{0};
  auto bars = registry.view<Foo, Bar>();
  bars.each([&num_found, VALUE1, VALUE2, &foobar1](const auto entity, const Foo& foo, const Bar& bar) {
    ++num_found;
    EXPECT_EQ(foobar1, entity);
    EXPECT_EQ(VALUE1, bar.b);
    EXPECT_EQ(VALUE2, foo.a);
  });
  EXPECT_EQ(1, num_found);
}

TEST(EnttTest, CanObserveUpdatesViaPatchMethod) {
  constexpr int VALUE1{1};
  constexpr int VALUE2{2};
  entt::registry registry{};

  entt::observer observer{registry, entt::collector.update<Foo>()};

  const auto foobar1 = registry.create();
  registry.emplace<Foo>(foobar1, Foo{VALUE1});
  registry.emplace<Bar>(foobar1, Bar{VALUE1});

  int changes_detected{0};
  observer.each([&changes_detected, foobar1](const auto entity) {
    EXPECT_EQ(foobar1, entity);
    ++changes_detected;
  });
  EXPECT_EQ(0, changes_detected);
  changes_detected = 0;

  {
    auto foos = registry.view<Foo>();
    foos.each([&registry, VALUE2](const auto entity, const Foo& /*ignored*/) {  //
      registry.patch<Foo>(entity, [VALUE2](auto& foo) { foo.a = VALUE2; });
    });
  }

  observer.each([&changes_detected, foobar1](const auto entity) {
    EXPECT_EQ(foobar1, entity);
    ++changes_detected;
  });
  EXPECT_EQ(1, changes_detected);
}

TEST(EnttTest, CanObserveMultipleUpdatesInOneObserver) {
  constexpr int VALUE1{1};
  constexpr int VALUE2{2};
  entt::registry registry{};

  entt::observer observer{registry, entt::collector.update<Foo>().update<Bar>()};

  const auto foobar1 = registry.create();
  registry.emplace<Foo>(foobar1, Foo{VALUE1});
  registry.emplace<Bar>(foobar1, Bar{VALUE1});

  int changes_detected{0};
  observer.each([&changes_detected, foobar1](const auto entity) {
    EXPECT_EQ(foobar1, entity);
    ++changes_detected;
  });
  EXPECT_EQ(0, changes_detected);
  changes_detected = 0;

  {
    auto foos = registry.view<Foo>();
    foos.each([&registry, VALUE2](const auto entity, const Foo& /*ignored*/) {  //
      registry.patch<Foo>(entity, [VALUE2](auto& foo) { foo.a = VALUE2; });
    });
  }

  observer.each([&changes_detected, foobar1](const auto entity) {
    EXPECT_EQ(foobar1, entity);
    ++changes_detected;
  });
  EXPECT_EQ(1, changes_detected);
  changes_detected = 0;

  {
    auto bars = registry.view<Bar>();
    bars.each([&registry, VALUE2](const auto entity, const Bar& /*ignored*/) {  //
      registry.patch<Bar>(entity, [VALUE2](auto& bar) { bar.b = VALUE2; });
    });
  }

  observer.each([&changes_detected, foobar1](const auto entity) {
    EXPECT_EQ(foobar1, entity);
    ++changes_detected;
  });
  EXPECT_EQ(1, changes_detected);
}

TEST(EnttTest, CanObserveCompoundUpdates) {
  constexpr int VALUE1{1};
  constexpr int VALUE2{2};
  entt::registry registry{};

  entt::observer observer{registry, entt::collector.update<Foo>().update<Bar>()};

  const auto foobar1 = registry.create();
  registry.emplace<Foo>(foobar1, Foo{VALUE1});
  registry.emplace<Bar>(foobar1, Bar{VALUE1});

  int changes_detected{0};
  observer.each([&changes_detected, foobar1](const auto entity) {
    EXPECT_EQ(foobar1, entity);
    ++changes_detected;
  });
  EXPECT_EQ(0, changes_detected);
  changes_detected = 0;

  {
    auto foobars = registry.view<Foo, Bar>();
    foobars.each([&registry, VALUE2](const auto entity, const Foo& /*ignored*/, const Bar& /*ignored*/) {  //
      registry.patch<Foo>(entity, [VALUE2](auto& foo) { foo.a = VALUE2; });
      registry.patch<Bar>(entity, [VALUE2](auto& bar) { bar.b = VALUE2; });
    });
  }

  // This will get called for each *entity* that changes, not for each change on an entity. So, two changes above to
  // this one entity will result in one call to this lambda.
  observer.each([&changes_detected, foobar1](const auto entity) {
    EXPECT_EQ(foobar1, entity);
    ++changes_detected;
  });
  EXPECT_EQ(1, changes_detected);
  changes_detected = 0;
}

}  // namespace
