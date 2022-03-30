#include "ui/model/color.h"

#include <string>

#include "material/colors.h"
#include "material/proto/color.pb.h"

namespace pack::ui::model {

void Color::from_proto(const material::proto::Rgba& proto, Color* color) {
  material::Colors::to_array(proto, color->values);
}

Color Color::from_proto(const material::proto::Rgba& proto) {
  Color result{};
  from_proto(proto, &result);
  return result;
}

void Color::to_proto(const Color& color, material::proto::Rgba* proto) {
  material::Colors::from_array(color.values, proto);
}

material::proto::Rgba Color::to_proto(const Color& color) {
  material::proto::Rgba proto{};
  to_proto(color, &proto);
  return proto;
}

std::string to_string(const Color& color) {
  using std::to_string;
  return "[" + to_string(color.values[0]) + ", " + to_string(color.values[1]) + ", " + to_string(color.values[2]) +
         ", " + to_string(color.values[3]) + "]";
}

}  // namespace pack::ui::model
