#include "component/component.h"
#include "component/components.h"
#include "component/proto/proto_serialization.h"
#include "demo/gears_ui.h"
#include "entt/entity/registry.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "lighting/light.h"
#include "lighting/proto/light.pb.h"
#include "position/position.h"
#include "proto/proto_utils.h"
#include "render/render_node.h"
#include "serialization/serialize.h"
#include "ui/animator.h"
#include "ui/application.h"
#include "ui/camera.h"

void populate_registry(entt::registry& registry, pack::component::Component component) {
  const auto id = registry.create();
  pack::component::Component& stored_component = registry.emplace<pack::component::Component>(id, std::move(component));
  registry.emplace<pack::render::RenderNode>(id, stored_component);
}

void populate_registry(entt::registry& registry, pack::lighting::Light light, pack::position::Position position,
                       pack::position::Orientation orientation) {
  const auto id = registry.create();
  registry.emplace<pack::lighting::Light>(id, std::move(light));
  registry.emplace<pack::position::Position>(id, std::move(position));
  registry.emplace<pack::position::Orientation>(id, std::move(orientation));
}

void populate_registry(entt::registry& registry, pack::ui::Camera camera, pack::position::Position position,
                       pack::position::Orientation orientation) {
  using namespace pack::ui;

  const auto id = registry.create();
  registry.emplace<Camera>(id, std::move(camera));
  registry.emplace<pack::position::Position>(id, std::move(position));
  registry.emplace<pack::position::Orientation>(id, std::move(orientation));
}

int main(int argc, char* argv[]) {
  using namespace pack::demo;
  using namespace pack::proto;
  using namespace pack::ui;

  // Default logging configuration.
  FLAGS_logtostderr = true;
  FLAGS_stderrthreshold = 0;
  google::InitGoogleLogging(argv[0]);

  // But allow user to override via the command line.
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  Application application{};
  GearsUi ui{};
  ui.assemble(application);

  Animator animator{};
  application.set_animator(animator);
  application.add_service(animator);

  populate_registry(application.registry(), Camera{}, pack::position::Position{}, pack::position::Orientation{});

  {
    pack::component::proto::Components proto =
        load_text_proto<pack::component::proto::Components>("demo/trivial_demo_components.pb.txt");
    pack::component::Components components =
        pack::from_proto<pack::component::Components, pack::component::proto::Components>(proto);
    LOG(INFO) << "Read components:\n" << to_string(components);
    for (pack::component::Component component : components) {
      populate_registry(application.registry(), component);
    }
  }

  {
    pack::lighting::proto::LightingConfiguration lights =
        load_text_proto<pack::lighting::proto::LightingConfiguration>("demo/lighting_configuration.pb.txt");
    for (const auto& proto : lights.light()) {
      pack::lighting::Light light{};
      pack::position::Position position{};
      pack::position::Orientation orientation{};
      pack::lighting::Light::from_proto(proto, &light, &position, &orientation);
      populate_registry(application.registry(), light, position, orientation);
    }
  }

  application.execute();
}
