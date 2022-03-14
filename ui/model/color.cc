#include "ui/model/color.h"

#include <string>

#include "color/color.pb.h"
#include "color/colors.h"

namespace pack::ui::model {

void Color::from_proto(const color::Rgba& proto, Color* color) { color::Colors::to_array(proto, color->values); }

Color Color::from_proto(const color::Rgba& proto) {
  Color result{};
  from_proto(proto, &result);
  return result;
}

void Color::to_proto(const Color& color, color::Rgba* proto) { color::Colors::from_array(color.values, proto); }

color::Rgba Color::to_proto(const Color& color) {
  color::Rgba proto{};
  to_proto(color, &proto);
  return proto;
}

std::string to_string(const Color& color) {
  using std::to_string;
  return "[" + to_string(color.values[0]) + ", " + to_string(color.values[1]) + ", " + to_string(color.values[2]) +
         ", " + to_string(color.values[3]) + "]";
}

}  // namespace pack::ui::model
