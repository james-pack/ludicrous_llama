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

using namespace pack;
using namespace pack::component;
using namespace pack::lighting;
using namespace pack::position;
using namespace pack::proto;
using namespace pack::render;
using namespace pack::ui;

void populate_component(ComponentTable& table, Light light, Position position, Orientation orientation) {
  const auto id = table.create();
  table.emplace<Light>(id, std::move(light));
  table.emplace<Position>(id, std::move(position));
  table.emplace<Orientation>(id, std::move(orientation));
}

void populate_component(ComponentTable& table, Camera camera, Position position, Orientation orientation) {
  const auto id = table.create();
  table.emplace<Camera>(id, std::move(camera));
  table.emplace<Position>(id, std::move(position));
  table.emplace<Orientation>(id, std::move(orientation));
}

void populate_table(ComponentTable& table, const Component& component) {
  table.add_id(component.id);
  table.emplace<Component>(component.id, component);
}

void populate_table(ComponentTable& table, const Subcomponent& subcomponent) {
  table.add_id(subcomponent.id);
  const auto& inserted = table.emplace<Subcomponent>(subcomponent.id, subcomponent);
  table.emplace<RenderNode>(subcomponent.id, RenderNode(inserted));
}

void populate_table(ComponentTable& table, const Scene& scene) {
  for (const auto& subcomponent : scene.root_components) {
    populate_table(table, subcomponent);
  }
  for (const auto& component : scene.components) {
    populate_table(table, component);
  }
}

int main(int argc, char* argv[]) {
  // Default logging configuration.
  FLAGS_logtostderr = true;
  FLAGS_stderrthreshold = 0;
  google::InitGoogleLogging(argv[0]);

  // But allow user to override via the command line.
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  Application application{};
  demo::GearsUi ui{};
  ui.assemble(application);

  Animator animator{};
  application.set_animator(animator);
  application.add_service(animator);

  // TODO(james): Move camera to scene.
  populate_component(application.component_table(), Camera{}, Position{}, Orientation{});

  {
    component::proto::Scene proto = load_text_proto<component::proto::Scene>("demo/trivial_demo_scene.pb.txt");
    Scene scene{};
    from_proto(proto, &scene);
    LOG(INFO) << "Read scene:\n" << to_string(scene);
    populate_table(application.component_table(), scene);
  }

  {
    // TODO(james): Move lighting to scene.
    lighting::proto::LightingConfiguration lights =
        load_text_proto<lighting::proto::LightingConfiguration>("demo/lighting_configuration.pb.txt");
    for (const auto& proto : lights.light()) {
      Light light{};
      Position position{};
      Orientation orientation{};
      Light::from_proto(proto, &light, &position, &orientation);
      populate_component(application.component_table(), light, position, orientation);
    }
  }

  application.execute();
}
