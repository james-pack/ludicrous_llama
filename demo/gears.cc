#include "demo/gears_ui.h"
#include "entt/entity/registry.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "lighting/light.h"
#include "lighting/proto/light.pb.h"
#include "material/materials.h"
#include "position/position.h"
#include "proto/proto_utils.h"
#include "ui/animator.h"
#include "ui/application.h"
#include "ui/camera.h"

/*
void populate_registry(entt::registry& registry, pack::ui::model::Gear gear, pack::position::Position position,
                       pack::position::Orientation orientation) {
  using namespace pack::ui;
  using namespace pack::ui::model;

  int draw_list_id = build_gear(gear);

  const auto gear_id = registry.create();
  registry.emplace<Render>(gear_id, construct_draw_list_renderer(draw_list_id));
  registry.emplace<Animate>(gear_id, construct_gear_animator());
  registry.emplace<Gear>(gear_id, std::move(gear));
  registry.emplace<pack::position::Position>(gear_id, std::move(position));
  registry.emplace<pack::position::Orientation>(gear_id, std::move(orientation));
}
*/

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
  using namespace pack::material;
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

  /*
  {
    pack::component::Gears gears = load_text_proto<pack::component::Gears>("demo/trivial_demo_gears.pb.txt");
    for (pack::component::Gear gear : gears.gear()) {
      populate_registry(application.registry(), Gear::from_proto(gear), pack::position::Position{-3.1f, 4.2f, 0.f},
                        pack::position::Orientation{});
    }
  }
  */

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
