#include "color/materials.h"
#include "component/gear.pb.h"
#include "demo/gears_ui.h"
#include "entt/entity/registry.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "lighting/proto/light.pb.h"
#include "proto/proto_utils.h"
#include "ui/animator.h"
#include "ui/application.h"
#include "ui/camera.h"
#include "ui/model/gear.h"
#include "ui/model/light.h"
#include "ui/model/position.h"
#include "ui/render.h"

void load_registry(entt::registry& registry, pack::ui::model::Gear gear, pack::ui::model::Position position,
                   pack::ui::model::Orientation orientation) {
  using namespace pack::ui;
  using namespace pack::ui::model;

  int draw_list_id = build_gear(gear);

  const auto gear_id = registry.create();
  registry.emplace<Render>(gear_id, construct_draw_list_renderer(draw_list_id));
  registry.emplace<Animate>(gear_id, construct_gear_animator());
  registry.emplace<Gear>(gear_id, std::move(gear));
  registry.emplace<Position>(gear_id, std::move(position));
  registry.emplace<Orientation>(gear_id, std::move(orientation));
}

void load_registry(entt::registry& registry, pack::ui::model::Light light, pack::ui::model::Position position,
                   pack::ui::model::Orientation orientation) {
  using namespace pack::ui::model;

  const auto id = registry.create();
  registry.emplace<Light>(id, std::move(light));
  registry.emplace<Position>(id, std::move(position));
  registry.emplace<Orientation>(id, std::move(orientation));
}

void load_registry(entt::registry& registry, pack::ui::Camera camera, pack::ui::model::Position position,
                   pack::ui::model::Orientation orientation) {
  using namespace pack::ui;
  using namespace pack::ui::model;

  const auto id = registry.create();
  registry.emplace<Camera>(id, std::move(camera));
  registry.emplace<Position>(id, std::move(position));
  registry.emplace<Orientation>(id, std::move(orientation));
}

int main(int argc, char* argv[]) {
  using namespace pack::color;
  using namespace pack::demo;
  using namespace pack::proto;
  using namespace pack::ui;
  using namespace pack::ui::model;

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

  load_registry(application.registry(), Camera{}, Position{}, Orientation{});

  {
    pack::component::Gears gears = load_text_proto<pack::component::Gears>("demo/trivial_demo_gears.pb.txt");
    for (pack::component::Gear gear : gears.gear()) {
      load_registry(application.registry(), Gear::from_proto(gear), Position{-3.1f, 4.2f, 0.f}, Orientation{});
    }
  }

  {
    pack::lighting::proto::LightingConfiguration lights =
        load_text_proto<pack::lighting::proto::LightingConfiguration>("demo/lighting_configuration.pb.txt");
    for (const auto& proto : lights.light()) {
      Light light{};
      Position position{};
      Orientation orientation{};
      Light::from_proto(proto, &light, &position, &orientation);
      load_registry(application.registry(), light, position, orientation);
    }
  }

  application.execute();
}
