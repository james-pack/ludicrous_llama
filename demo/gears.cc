#include "color/materials.h"
#include "component/gear.h"
#include "component/gear.pb.h"
#include "component/position.pb.h"
#include "demo/gears_ui.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "proto/proto_utils.h"
#include "ui/animator.h"
#include "ui/application.h"
#include "ui/camera.h"
#include "ui/lighting_im_render.h"
#include "ui/lighting_model.h"
#include "ui/render.h"

int main(int argc, char* argv[]) {
  using namespace pack::color;
  using namespace pack::component;
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

  pack::ui::Animator animator{};
  application.add_service(animator);

  {
    const auto camera = application.registry().create();
    application.registry().emplace<Camera>(camera, Camera{});
    application.registry().emplace<Position>(camera, Position{});
    application.registry().emplace<Orientation>(camera, Orientation{});
  }

  {
    Gears gears = load_text_proto<Gears>("demo/trivial_demo_gears.pb.txt");
    DLOG(INFO) << "Gears loaded:\n" << gears.DebugString();

    for (Gear gear : gears.gear()) {
      Material* material = gear.mutable_material();
      DLOG(INFO) << "Packing material colors";
      Materials::pack_colors(material);
      const auto gear_id = application.registry().create();
      DLOG(INFO) << "Building the draw list for the gear";
      int draw_list_id = build_gear(gear);
      application.registry().emplace<Render>(gear_id, construct_draw_list_renderer(draw_list_id));
      application.registry().emplace<Animate>(gear_id, construct_gear_animator(gear));
      application.registry().emplace<Gear>(gear_id, std::move(gear));
      Position position{};
      position.mutable_float_values()->set_x(-3.1f);
      position.mutable_float_values()->set_y(4.2f);
      application.registry().emplace<Position>(gear_id, position);
      application.registry().emplace<Orientation>(gear_id, Orientation{});
    }
  }

  DLOG(INFO) << "Creating UI models";
  const auto ui_models = application.registry().create();
  {
    application.registry().emplace<LightingModel>(ui_models, LightingModel{});
    LightingModel& model = application.registry().get<LightingModel>(ui_models);
    model.connect(LightingModelSignal::POSITION_UPDATE, lighting_im_signal_render);
    model.connect(LightingModelSignal::COLOR_UPDATE, lighting_im_signal_render);
    model.connect(LightingModelSignal::ENABLED_UPDATE, lighting_im_signal_render);
    model.connect(LightingModelSignal::RESET_UPDATE, lighting_im_signal_render);
    model.set_lighting_configuration_path("demo/lighting_configuration.pb.txt");
    model.load();
  }

  application.execute();
}
