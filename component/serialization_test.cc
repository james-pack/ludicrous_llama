#include <filesystem>
#include <string_view>

#include "component/component.h"
#include "component/components.h"
#include "component/expression.h"
#include "component/parameter.h"
#include "component/property.h"
#include "component/proto/component.pb.h"
#include "component/serialize.h"
#include "component/value.h"
#include "gtest/gtest.h"
#include "proto/proto_utils.h"
#include "testing/test_temp_directory.h"

namespace pack::component {

namespace fs = std::filesystem;
using pack::proto::load_text_proto;
using pack::proto::save_text_proto;

fs::path create_path(const std::string_view& filename, bool persist_after_test = false) {
  if (persist_after_test) {
    return filename;
  } else {
    fs::path tmp_dir = testing::TestTempDirectory::getInstance()->createTestSubdirectory();
    return tmp_dir / filename;
  }
}

TEST(ProtoTest, CanRoundTripEmptyValue) {
  Value original{};
  proto::Value proto = to_proto<Value, proto::Value>(original);
  Value cycled{from_proto<Value, proto::Value>(proto)};
  EXPECT_EQ(original, cycled);
}

TEST(ProtoTest, CanRoundTripEmptyExpression) {
  Expression original{};
  proto::Expression proto = to_proto<Expression, proto::Expression>(original);
  Expression cycled{from_proto<Expression, proto::Expression>(proto)};
  EXPECT_EQ(original, cycled);
}

TEST(ProtoTest, CanRoundTripEmptyPrecision) {
  Precision original{};
  proto::Precision proto = to_proto<Precision, proto::Precision>(original);
  Precision cycled{from_proto<Precision, proto::Precision>(proto)};
  EXPECT_EQ(original, cycled);
}

TEST(ProtoTest, CanRoundTripEmptyValueDomain) {
  ValueDomain original{};
  proto::ValueDomain proto = to_proto<ValueDomain, proto::ValueDomain>(original);
  ValueDomain cycled{from_proto<ValueDomain, proto::ValueDomain>(proto)};
  EXPECT_EQ(original, cycled);
}

TEST(ProtoTest, CanRoundTripEmptyParameter) {
  Parameter original{};
  proto::Parameter proto = to_proto<Parameter, proto::Parameter>(original);
  Parameter cycled{from_proto<Parameter, proto::Parameter>(proto)};
  EXPECT_EQ(original, cycled);
}

TEST(ProtoTest, CanRoundTripEmptyParameterBinding) {
  ParameterBinding original{};
  proto::ParameterBinding proto = to_proto<ParameterBinding, proto::ParameterBinding>(original);
  ParameterBinding cycled{from_proto<ParameterBinding, proto::ParameterBinding>(proto)};
  EXPECT_EQ(original, cycled);
}

TEST(ProtoTest, CanRoundTripEmptyProperty) {
  Property original{};
  proto::Property proto = to_proto<Property, proto::Property>(original);
  Property cycled{from_proto<Property, proto::Property>(proto)};
  EXPECT_EQ(original, cycled);
}

TEST(ProtoTest, CanRoundTripEmptySubcomponent) {
  Subcomponent original{};
  proto::Subcomponent proto = to_proto<Subcomponent, proto::Subcomponent>(original);
  Subcomponent cycled{from_proto<Subcomponent, proto::Subcomponent>(proto)};
  EXPECT_EQ(original, cycled);
}

TEST(ProtoTest, CanRoundTripEmptyComponent) {
  Component original{};
  proto::Component proto = to_proto<Component, proto::Component>(original);
  Component cycled{from_proto<Component, proto::Component>(proto)};
  EXPECT_EQ(original, cycled);
}

}  // namespace pack::component
