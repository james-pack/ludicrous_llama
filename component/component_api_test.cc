#include <filesystem>
#include <string_view>

#include "component/components.h"
#include "component/parameter.h"
#include "component/primitive.h"
#include "component/property.h"
#include "gtest/gtest.h"
#include "guid/guid.h"
#include "proto/proto_utils.h"
#include "testing/test_temp_directory.h"

namespace pack::component {

/*
namespace fs = std::filesystem;

fs::path create_path(const std::string_view& filename, bool persist_after_test = false) {
  if (persist_after_test) {
    return filename;
  } else {
    fs::path tmp_dir = testing::TestTempDirectory::getInstance()->createTestSubdirectory();
    return tmp_dir / filename;
  }
}
*/

TEST(ApiTest, CanConstructTrivialComponentFromPrimitive) {
  Component shelf{};
  shelf.name = "shelf";

  shelf.primitive = Primitive::by_name("box");

  {
    ParameterBinding binding{"width"};
    binding.value = Expression{Value{1390.f}};
    shelf.bindings.insert(std::move(binding));
  }
  {
    ParameterBinding binding{"height"};
    binding.value = Expression{Value{18.f}};
    shelf.bindings.insert(std::move(binding));
  }
  {
    ParameterBinding binding{"depth"};
    binding.value = Expression{Value{400.f}};
    shelf.bindings.insert(std::move(binding));
  }
}

TEST(ApiTest, CanConstructComponentsForSimpleBox) {
  Components box_components{};
  Component box{};

  {
    Component component{create_component("top", "box", "width", 1390.f, "height", 18.f, "depth", 400.f)};
    box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }

  {
    Component component{create_component("top", "box", "width", 1390.f, "height", 18.f, "depth", 400.f)};
    box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }

  {
    Component component{create_component("top", "box", "width", 1390.f, "height", 18.f, "depth", 400.f)};
    box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }

  {
    Component component{create_component("top", "box", "width", 1390.f, "height", 18.f, "depth", 400.f)};
    box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }

  {
    Component component{create_component("top", "box", "width", 1390.f, "height", 18.f, "depth", 400.f)};
    box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }

  {
    Component component{create_component("top", "box", "width", 1390.f, "height", 18.f, "depth", 400.f)};
    box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }

  box_components.insert(box);

  EXPECT_EQ(6, box.children.size());
  // The box_components contains all of the components of the box, plus 1 for the box itself.
  EXPECT_EQ(box_components.size(), box.children.size() + 1);
}

TEST(ApiTest, CanConstructSimpleBoxWithProperties) {
  Components box_components{};
  Component box{};
  {
    Property property{"width"};
    as_float(as_literal(property.value)) = 1390.f;
    box.properties.insert(std::move(property));
  }
  {
    Property property{"height"};
    as_float(as_literal(property.value)) = 450.f;
    box.properties.insert(std::move(property));
  }
  {
    Property property{"depth"};
    as_float(as_literal(property.value)) = 400.f;
    box.properties.insert(std::move(property));
  }
  {
    Property property{"material_thickness"};
    as_float(as_literal(property.value)) = 18.f;
    box.properties.insert(std::move(property));
  }

  {
    Component component{
        create_component("top", "box", "width", "../width", "height", "../material_thickness", "depth", "../depth")};
    box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }

  {
    Component component{
        create_component("bottom", "box", "width", "../width", "height", "../material_thickness", "depth", "../depth")};
    box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }
  {
    Component component{
        create_component("left", "box", "width", "../height", "height", "../material_thickness", "depth", "../depth")};
    box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }
  {
    Component component{
        create_component("right", "box", "width", "../height", "height", "../material_thickness", "depth", "../depth")};
    box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }
  {
    Component component{
        create_component("front", "box", "width", "../width", "height", "../material_thickness", "depth", "../height")};
    box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }
  {
    Component component{
        create_component("back", "box", "width", "../width", "height", "../material_thickness", "depth", "../height")};
    box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }

  box_components.insert(box);

  EXPECT_EQ(6, box.children.size());
  // The box_components contains all of the components of the box, plus 1 for the box itself.
  EXPECT_EQ(box_components.size(), box.children.size() + 1);
}

TEST(ApiTest, CanConstructNestingBoxes) {
  Components box_components{};
  Component template_box{};
  {
    Parameter parameter{"width"};
    parameter.domain.type = Type::FLOAT;
    as_float(parameter.domain.min_value) = 1.f;
    as_float(parameter.domain.max_value) = 2435.f;
    template_box.parameters.insert(std::move(parameter));
  }
  {
    Parameter parameter{"height"};
    parameter.domain.type = Type::FLOAT;
    as_float(parameter.domain.min_value) = 1.f;
    as_float(parameter.domain.max_value) = 2435.f;
    template_box.parameters.insert(std::move(parameter));
  }
  {
    Parameter parameter{"depth"};
    parameter.domain.type = Type::FLOAT;
    as_float(parameter.domain.min_value) = 1.f;
    as_float(parameter.domain.max_value) = 2435.f;
    template_box.parameters.insert(std::move(parameter));
  }
  {
    Parameter parameter{"material_thickness"};
    parameter.domain.type = Type::FLOAT;
    as_float(parameter.domain.min_value) = 1.f;
    as_float(parameter.domain.max_value) = 25.4f;
    template_box.parameters.insert(std::move(parameter));
  }
  ASSERT_EQ(4, template_box.parameters.size());
  {
    Component component{
        create_component("top", "box", "width", "../width", "height", "../material_thickness", "depth", "../depth")};
    template_box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }

  {
    Component component{
        create_component("bottom", "box", "width", "../width", "height", "../material_thickness", "depth", "../depth")};
    template_box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }
  {
    Component component{
        create_component("left", "box", "width", "../height", "height", "../material_thickness", "depth", "../depth")};
    template_box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }
  {
    Component component{
        create_component("right", "box", "width", "../height", "height", "../material_thickness", "depth", "../depth")};
    template_box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }
  {
    Component component{
        create_component("front", "box", "width", "../width", "height", "../material_thickness", "depth", "../height")};
    template_box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }
  {
    Component component{
        create_component("back", "box", "width", "../width", "height", "../material_thickness", "depth", "../height")};
    template_box.children.insert(Subcomponent{component.id});
    box_components.insert(std::move(component));
  }

  constexpr int NUM_BOXES{5};

  for (int box_number = 0; box_number < NUM_BOXES; ++box_number) {
    Component nested_box{};
    if (box_number == 0) {
      nested_box.name = "outermost_box";
    }
    {
      Property property{"box_number"};
      as_integer(as_literal(property.value)) = box_number;
      nested_box.properties.insert(std::move(property));
    }
    {
      Property property{"width"};
      if (box_number == 0) {
        as_float(as_literal(property.value)) = 1390.f;
      } else {
        as_expression(property.value) =
            "#outermost_box/width - box_number * (2 * #outermost_box/material_thickness + 0.2)";
      }
      nested_box.properties.insert(std::move(property));
    }
    {
      Property property{"height"};
      if (box_number == 0) {
        as_float(as_literal(property.value)) = 450.f;
      } else {
        as_expression(property.value) =
            "#outermost_box/height - box_number * (2 * #outermost_box/material_thickness + 0.2)";
      }
      nested_box.properties.insert(std::move(property));
    }
    {
      Property property{"depth"};
      if (box_number == 0) {
        as_float(as_literal(property.value)) = 400.f;
      } else {
        as_expression(property.value) =
            "#outermost_box/depth - box_number * (2 * #outermost_box/material_thickness + 0.2)";
      }
      nested_box.properties.insert(std::move(property));
    }
    {
      Property property{"material_thickness"};
      as_float(as_literal(property.value)) = 18.f;
      nested_box.properties.insert(std::move(property));
    }

    Subcomponent child{template_box.id};
    {
      ParameterBinding binding{"width"};
      as_expression(binding.value) = "../width";
      child.bindings.insert(std::move(binding));
    }
    {
      ParameterBinding binding{"height"};
      as_expression(binding.value) = "../height";
      child.bindings.insert(std::move(binding));
    }
    {
      ParameterBinding binding{"depth"};
      as_expression(binding.value) = "../depth";
      child.bindings.insert(std::move(binding));
    }
    {
      ParameterBinding binding{"material_thickness"};
      as_expression(binding.value) = "../material_thickness";
      child.bindings.insert(std::move(binding));
    }
    nested_box.children.insert(std::move(child));
    box_components.insert(std::move(nested_box));
  }
  box_components.insert(template_box);

  EXPECT_EQ(6, template_box.children.size());
  // The box_components contains all of the components of the box, plus 1 for the template box itself, plus 1 for each
  // box instance.
  EXPECT_EQ(box_components.size(), template_box.children.size() + 1 + NUM_BOXES);
}

}  // namespace pack::component
