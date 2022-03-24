#include <filesystem>
#include <string_view>

#include "component/components.h"
#include "gtest/gtest.h"
#include "proto/proto_utils.h"
#include "serialization/component.pb.h"
#include "testing/test_temp_directory.h"

namespace pack::component {

namespace fs = std::filesystem;

using namespace pack::serialization;

fs::path create_path(const std::string_view& filename, bool persist_after_test = false) {
  if (persist_after_test) {
    return filename;
  } else {
    fs::path tmp_dir = testing::TestTempDirectory::getInstance()->createTestSubdirectory();
    return tmp_dir / filename;
  }
}

TEST(ApiTest, CanConstructTrivialComponentFromPrimitive) {
  Component shelf{};
  shelf.set_id("id");
  shelf.set_name("shelf");

  ProcessedPrimitive* processed = shelf.add_primitives();
  Primitive* primitive = processed->mutable_primitive();
  primitive->set_name("box");
  ParameterBinding* width = primitive->add_parameters();
  width->set_parameter_name("width");
  width->mutable_value()->mutable_literal()->set_float_value(1390.f);
  ParameterBinding* height = primitive->add_parameters();
  height->set_parameter_name("height");
  height->mutable_value()->mutable_literal()->set_float_value(18.f);
  ParameterBinding* depth = primitive->add_parameters();
  depth->set_parameter_name("depth");
  depth->mutable_value()->mutable_literal()->set_float_value(400.f);
}

TEST(ApiTest, CanConstructCompoonentsForSimpleBox) {
  Components box_components{};
  Component* box = box_components.add_components();
  std::string id{};

  Component* component{nullptr};
  component = box_components.add_components();
  id = create_component("top", "box", "width", 1390.f, "height", 18.f, "depth", 400.f, component);
  box->add_children()->set_child_id(id);

  component = box_components.add_components();
  id = create_component("bottom", "box", "width", 1390.f, "height", 18.f, "depth", 400.f, component);
  box->add_children()->set_child_id(id);

  component = box_components.add_components();
  id = create_component("front", "box", "width", 1390.f, "height", 18.f, "depth", 400.f, component);
  box->add_children()->set_child_id(id);

  component = box_components.add_components();
  id = create_component("back", "box", "width", 1390.f, "height", 18.f, "depth", 400.f, component);
  box->add_children()->set_child_id(id);

  component = box_components.add_components();
  id = create_component("left", "box", "width", 1390.f, "height", 18.f, "depth", 400.f, component);
  box->add_children()->set_child_id(id);

  component = box_components.add_components();
  id = create_component("right", "box", "width", 1390.f, "height", 18.f, "depth", 400.f, component);
  box->add_children()->set_child_id(id);

  EXPECT_EQ(6, box->children_size());
  // The box_components contains all of the components of the box, plus 1 for the box itself.
  EXPECT_EQ(box_components.components_size(), box->children_size() + 1);
}

TEST(ApiTest, CanConstructSimpleBoxWithProperties) {
  Components box_components{};
  Component* box = box_components.add_components();
  {
    Property* property = box->add_properties();
    property->set_name("width");
    property->mutable_value()->mutable_literal()->set_float_value(1390.f);
  }
  {
    Property* property = box->add_properties();
    property->set_name("height");
    property->mutable_value()->mutable_literal()->set_float_value(450.f);
  }
  {
    Property* property = box->add_properties();
    property->set_name("depth");
    property->mutable_value()->mutable_literal()->set_float_value(400.f);
  }
  {
    Property* property = box->add_properties();
    property->set_name("material_thickness");
    property->mutable_value()->mutable_literal()->set_float_value(18.f);
  }

  Component* component{nullptr};
  Subcomponent* child{nullptr};
  std::string id{};

  component = box_components.add_components();
  id = create_component("top", "box", "width", "../../width", "height", "../../material_thickness", "depth",
                        "../../depth", component);
  child = box->add_children();
  child->set_child_id(id);

  component = box_components.add_components();
  id = create_component("top", "box", "width", "../../width", "height", "../../material_thickness", "depth",
                        "../../depth", component);
  child = box->add_children();
  child->set_child_id(id);

  component = box_components.add_components();
  id = create_component("left", "box", "width", "../../height", "height", "../../material_thickness", "depth",
                        "../../depth", component);
  child = box->add_children();
  child->set_child_id(id);

  component = box_components.add_components();
  id = create_component("right", "box", "width", "../../height", "height", "../../material_thickness", "depth",
                        "../../depth", component);
  child = box->add_children();
  child->set_child_id(id);

  component = box_components.add_components();
  id = create_component("front", "box", "width", "../../width", "height", "../../material_thickness", "depth",
                        "../../height", component);
  child = box->add_children();
  child->set_child_id(id);

  component = box_components.add_components();
  id = create_component("back", "box", "width", "../../width", "height", "../../material_thickness", "depth",
                        "../../height", component);
  child = box->add_children();
  child->set_child_id(id);

  EXPECT_EQ(6, box->children_size());
  // The box_components contains all of the components of the box, plus 1 for the box itself.
  EXPECT_EQ(box_components.components_size(), box->children_size() + 1);
}

TEST(ApiTest, CanConstructSimpleBoxWithParametersBoundInASpecificBox) {
  Components box_components{};
  Component* box = box_components.add_components();
  {
    Parameter* parameter = box->add_parameters();
    parameter->set_name("width");
    ValueDomain* domain = parameter->mutable_domain();
    domain->mutable_type()->set_type(Type::FLOAT);
    domain->mutable_min_value()->set_float_value(1.f);
    domain->mutable_max_value()->set_float_value(2435.f);
  }
  {
    Parameter* parameter = box->add_parameters();
    parameter->set_name("height");
    ValueDomain* domain = parameter->mutable_domain();
    domain->mutable_type()->set_type(Type::FLOAT);
    domain->mutable_min_value()->set_float_value(1.f);
    domain->mutable_max_value()->set_float_value(2435.f);
  }
  {
    Parameter* parameter = box->add_parameters();
    parameter->set_name("depth");
    ValueDomain* domain = parameter->mutable_domain();
    domain->mutable_type()->set_type(Type::FLOAT);
    domain->mutable_min_value()->set_float_value(1.f);
    domain->mutable_max_value()->set_float_value(2435.f);
  }
  {
    Parameter* parameter = box->add_parameters();
    parameter->set_name("material_thickness");
    ValueDomain* domain = parameter->mutable_domain();
    domain->mutable_type()->set_type(Type::FLOAT);
    domain->mutable_min_value()->set_float_value(1.f);
    domain->mutable_max_value()->set_float_value(2435.f);
  }

  Component* specific_box = box_components.add_components();
  {
    Property* property = specific_box->add_properties();
    property->set_name("width");
    property->mutable_value()->mutable_literal()->set_float_value(1390.f);
  }
  {
    Property* property = specific_box->add_properties();
    property->set_name("height");
    property->mutable_value()->mutable_literal()->set_float_value(450.f);
  }
  {
    Property* property = specific_box->add_properties();
    property->set_name("depth");
    property->mutable_value()->mutable_literal()->set_float_value(400.f);
  }
  {
    Property* property = specific_box->add_properties();
    property->set_name("material_thickness");
    property->mutable_value()->mutable_literal()->set_float_value(18.f);
  }

  Subcomponent* child{nullptr};
  child = specific_box->add_children();
  child->set_child_id(box->id());
  {
    ParameterBinding* binding = child->add_parameters();
    binding->set_parameter_name("width");
    binding->mutable_value()->set_expression("../width");
  }
  {
    ParameterBinding* binding = child->add_parameters();
    binding->set_parameter_name("height");
    binding->mutable_value()->set_expression("../height");
  }
  {
    ParameterBinding* binding = child->add_parameters();
    binding->set_parameter_name("depth");
    binding->mutable_value()->set_expression("../depth");
  }
  {
    ParameterBinding* binding = child->add_parameters();
    binding->set_parameter_name("material_thickness");
    binding->mutable_value()->set_expression("../material_thickness");
  }

  Component* component{nullptr};
  std::string id{};

  component = box_components.add_components();
  id = create_component("top", "box", "width", "../../width", "height", "../../material_thickness", "depth",
                        "../../depth", component);
  child = box->add_children();
  child->set_child_id(id);

  component = box_components.add_components();
  id = create_component("top", "box", "width", "../../width", "height", "../../material_thickness", "depth",
                        "../../depth", component);
  child = box->add_children();
  child->set_child_id(id);

  component = box_components.add_components();
  id = create_component("left", "box", "width", "../../height", "height", "../../material_thickness", "depth",
                        "../../depth", component);
  child = box->add_children();
  child->set_child_id(id);

  component = box_components.add_components();
  id = create_component("right", "box", "width", "../../height", "height", "../../material_thickness", "depth",
                        "../../depth", component);
  child = box->add_children();
  child->set_child_id(id);

  component = box_components.add_components();
  id = create_component("front", "box", "width", "../../width", "height", "../../material_thickness", "depth",
                        "../../height", component);
  child = box->add_children();
  child->set_child_id(id);

  component = box_components.add_components();
  id = create_component("back", "box", "width", "../../width", "height", "../../material_thickness", "depth",
                        "../../height", component);
  child = box->add_children();
  child->set_child_id(id);

  EXPECT_EQ(6, box->children_size());
  // The box_components contains all of the components of the box, plus 1 for the generic box, plus 1 for the specific
  // box itself.
  EXPECT_EQ(box_components.components_size(), box->children_size() + 2);

  proto::save_text_proto(create_path("simple_box.pb.txt", true), box_components);
}

TEST(ApiTest, CanConstructNestingBoxes) {
  Components box_components{};
  Component* template_box = box_components.add_components();
  {
    Parameter* parameter = template_box->add_parameters();
    parameter->set_name("width");
    ValueDomain* domain = parameter->mutable_domain();
    domain->mutable_type()->set_type(Type::FLOAT);
    domain->mutable_min_value()->set_float_value(1.f);
    domain->mutable_max_value()->set_float_value(2435.f);
  }
  {
    Parameter* parameter = template_box->add_parameters();
    parameter->set_name("height");
    ValueDomain* domain = parameter->mutable_domain();
    domain->mutable_type()->set_type(Type::FLOAT);
    domain->mutable_min_value()->set_float_value(1.f);
    domain->mutable_max_value()->set_float_value(2435.f);
  }
  {
    Parameter* parameter = template_box->add_parameters();
    parameter->set_name("depth");
    ValueDomain* domain = parameter->mutable_domain();
    domain->mutable_type()->set_type(Type::FLOAT);
    domain->mutable_min_value()->set_float_value(1.f);
    domain->mutable_max_value()->set_float_value(2435.f);
  }
  {
    Parameter* parameter = template_box->add_parameters();
    parameter->set_name("material_thickness");
    ValueDomain* domain = parameter->mutable_domain();
    domain->mutable_type()->set_type(Type::FLOAT);
    domain->mutable_min_value()->set_float_value(1.f);
    domain->mutable_max_value()->set_float_value(2435.f);
  }

  Subcomponent* child{nullptr};
  Component* component{nullptr};
  std::string id{};

  component = box_components.add_components();
  id = create_component("top", "box", "width", "../../width", "height", "../../material_thickness", "depth",
                        "../../depth", component);
  child = template_box->add_children();
  child->set_child_id(id);

  component = box_components.add_components();
  id = create_component("top", "box", "width", "../../width", "height", "../../material_thickness", "depth",
                        "../../depth", component);
  child = template_box->add_children();
  child->set_child_id(id);

  component = box_components.add_components();
  id = create_component("left", "box", "width", "../../height", "height", "../../material_thickness", "depth",
                        "../../depth", component);
  child = template_box->add_children();
  child->set_child_id(id);

  component = box_components.add_components();
  id = create_component("right", "box", "width", "../../height", "height", "../../material_thickness", "depth",
                        "../../depth", component);
  child = template_box->add_children();
  child->set_child_id(id);

  component = box_components.add_components();
  id = create_component("front", "box", "width", "../../width", "height", "../../material_thickness", "depth",
                        "../../height", component);
  child = template_box->add_children();
  child->set_child_id(id);

  component = box_components.add_components();
  id = create_component("back", "box", "width", "../../width", "height", "../../material_thickness", "depth",
                        "../../height", component);
  child = template_box->add_children();
  child->set_child_id(id);

  constexpr int num_boxes = 5;

  for (int box_number = 0; box_number < num_boxes; ++box_number) {
    Component* nested_box = box_components.add_components();
    if (box_number == 0) {
      nested_box->set_name("outermost_box");
    }
    {
      Property* property = nested_box->add_properties();
      property->set_name("box_number");
      property->mutable_value()->mutable_literal()->set_int_value(box_number);
    }
    {
      Property* property = nested_box->add_properties();
      property->set_name("width");
      if (box_number == 0) {
        property->mutable_value()->mutable_literal()->set_float_value(1390.f);
      } else {
        property->mutable_value()->set_expression(
            "#outermost_box/width - box_number * (2 * #outermost_box/material_thickness + 0.2)");
      }
    }
    {
      Property* property = nested_box->add_properties();
      property->set_name("height");
      if (box_number == 0) {
        property->mutable_value()->mutable_literal()->set_float_value(450.f);
      } else {
        property->mutable_value()->set_expression(
            "#outermost_box/height - box_number * (2 * #outermost_box/material_thickness + 0.2)");
      }
    }
    {
      Property* property = nested_box->add_properties();
      property->set_name("depth");
      if (box_number == 0) {
        property->mutable_value()->mutable_literal()->set_float_value(400.f);
      } else {
        property->mutable_value()->set_expression(
            "#outermost_box/depth - box_number * (2 * #outermost_box/material_thickness + 0.2)");
      }
    }
    {
      Property* property = nested_box->add_properties();
      property->set_name("material_thickness");
      if (box_number == 0) {
        property->mutable_value()->mutable_literal()->set_float_value(18.f);
      }
    }
    child = nested_box->add_children();
    child->set_child_id(template_box->id());
    {
      ParameterBinding* binding = child->add_parameters();
      binding->set_parameter_name("width");
      binding->mutable_value()->set_expression("../width");
    }
    {
      ParameterBinding* binding = child->add_parameters();
      binding->set_parameter_name("height");
      binding->mutable_value()->set_expression("../height");
    }
    {
      ParameterBinding* binding = child->add_parameters();
      binding->set_parameter_name("depth");
      binding->mutable_value()->set_expression("../depth");
    }
    {
      ParameterBinding* binding = child->add_parameters();
      binding->set_parameter_name("material_thickness");
      binding->mutable_value()->set_expression("../material_thickness");
    }
  }

  EXPECT_EQ(6, template_box->children_size());
  // The box_components contains all of the components of the box, plus 1 for the template box itself, plus 1 for each
  // box instance.
  EXPECT_EQ(box_components.components_size(), template_box->children_size() + 1 + num_boxes);

  proto::save_text_proto(create_path("nested_boxes.pb.txt", true), box_components);
}

}  // namespace pack::component
