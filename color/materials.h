#pragma once

#include "color/colors.h"
#include "color/material.pb.h"

namespace pack::color {

class Materials final {
 public:
  /**
   * Set the ambient color on a material using RGBA values.
   */
  static void set_ambient(Material* material, float red, float green, float blue, float alpha = 1.f) {
    material->mutable_ambient()->mutable_float_values()->set_red(red);
    material->mutable_ambient()->mutable_float_values()->set_green(green);
    material->mutable_ambient()->mutable_float_values()->set_blue(blue);
    material->mutable_ambient()->mutable_float_values()->set_alpha(alpha);
  }

  /**
   * Set the diffuse color on a material using RGBA values.
   */
  static void set_diffuse(Material* material, float red, float green, float blue, float alpha = 1.f) {
    material->mutable_diffuse()->mutable_float_values()->set_red(red);
    material->mutable_diffuse()->mutable_float_values()->set_green(green);
    material->mutable_diffuse()->mutable_float_values()->set_blue(blue);
    material->mutable_diffuse()->mutable_float_values()->set_alpha(alpha);
  }

  /**
   * Set the specular color on a material using RGBA values.
   */
  static void set_specular(Material* material, float red, float green, float blue, float alpha = 1.f) {
    material->mutable_specular()->mutable_float_values()->set_red(red);
    material->mutable_specular()->mutable_float_values()->set_green(green);
    material->mutable_specular()->mutable_float_values()->set_blue(blue);
    material->mutable_specular()->mutable_float_values()->set_alpha(alpha);
  }

  /**
   * Set the shininess on a material. Trivial function; only here for completeness.
   */
  static void set_shininess(Material* material, float value) { material->set_shininess(value); }

  /**
   * Create a simple material using RGBA values as the ambient and diffuse colors.
   */
  static Material create(float red, float green, float blue, float alpha = 1.f) {
    Material result{};
    set_diffuse(&result, red, green, blue, alpha);

    // Copy diffuse color as ambient color.
    *result.mutable_ambient() = result.diffuse();

    return result;
  }
};

}  // namespace pack::color
