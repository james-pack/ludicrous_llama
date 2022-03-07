#include "proto/proto_utils.h"

#include <filesystem>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "proto/simple.pb.h"
#include "testing/test_temp_directory.h"

namespace pack::proto {

namespace fs = std::filesystem;

class ProtoPersistenceTest : public ::testing::Test {
 public:
  fs::path tmp_dir{};

  void SetUp() override {
    tmp_dir = pack::testing::TestTempDirectory::getInstance()->createTestSubdirectory();
    LOG(INFO) << "tmp_dir: '" << tmp_dir.string() << "'";
  }
};

TEST_F(ProtoPersistenceTest, CanCompile) {
  Simple simple{};
  simple.set_name("test");
  EXPECT_EQ("test", simple.name());
}

TEST_F(ProtoPersistenceTest, CanSaveSmallProto) {
  Simple simple{};
  simple.set_name("small configuration");
  simple.set_num(3);

  const auto path{tmp_dir / "small_simple_test.pb"};
  save_proto(path, simple);

  Simple loaded = load_proto<Simple>(path);

  EXPECT_EQ(simple.name(), loaded.name());
  EXPECT_EQ(simple.num(), loaded.num());
}

TEST_F(ProtoPersistenceTest, CanSaveSmallConfigurationAsTextProto) {
  Simple simple{};
  simple.set_name("small configuration");
  simple.set_num(3);

  const auto path{tmp_dir / "small_simple_test.pb"};
  save_text_proto(path, simple);

  Simple loaded = load_text_proto<Simple>(path);

  EXPECT_EQ(simple.name(), loaded.name());
  EXPECT_EQ(simple.num(), loaded.num());
}

}  // namespace pack::proto
