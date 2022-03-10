#include "ui/lighting_model.h"

#include "google/protobuf/util/message_differencer.h"
#include "gtest/gtest.h"
#include "proto/proto_utils.h"
#include "ui/event_queue.h"
#include "ui/lighting_model.h"

namespace pack::ui {

LightingModelSignal signal_received{};
const LightingModel* model_received{nullptr};
int num_signals_received{0};
void receive_signal(LightingModelSignal signal, const LightingModel& lighting) {
  signal_received = signal;
  model_received = &lighting;
  ++num_signals_received;
}

class LightingModelTest : public ::testing::Test {
 public:
  void clear_reception_state() {
    signal_received = LightingModelSignal::INVALID;
    model_received = nullptr;
    num_signals_received = 0;
  }

  void SetUp() override { clear_reception_state(); }
};

TEST_F(LightingModelTest, EnableLightFiresSignal) {
  LightingModel lighting{};
  lighting.connect(LightingModelSignal::POSITION_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::COLOR_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::ENABLED_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::RESET_UPDATE, receive_signal);

  ASSERT_FALSE(lighting.is_enabled(0));
  lighting.enable(0);

  EventQueue::distribute();

  EXPECT_EQ(LightingModelSignal::ENABLED_UPDATE, signal_received);
  EXPECT_EQ(&lighting, model_received);
  EXPECT_EQ(1, num_signals_received);
}

TEST_F(LightingModelTest, EnableLightOnlyFiresSignalIfDisabled) {
  LightingModel lighting{};
  lighting.connect(LightingModelSignal::POSITION_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::COLOR_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::ENABLED_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::RESET_UPDATE, receive_signal);

  ASSERT_FALSE(lighting.is_enabled(0));
  lighting.enable(0);

  EventQueue::distribute();

  ASSERT_EQ(LightingModelSignal::ENABLED_UPDATE, signal_received);
  ASSERT_EQ(&lighting, model_received);
  ASSERT_EQ(1, num_signals_received);
  clear_reception_state();

  ASSERT_TRUE(lighting.is_enabled(0));
  lighting.enable(0);

  EventQueue::distribute();

  EXPECT_EQ(LightingModelSignal::INVALID, signal_received);
  EXPECT_EQ(nullptr, model_received);
  EXPECT_EQ(0, num_signals_received);
}

TEST_F(LightingModelTest, ChangingPositionFiresSignal) {
  constexpr GLint LIGHT_NUM{0};
  LightingModel lighting{};
  lighting.connect(LightingModelSignal::POSITION_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::COLOR_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::ENABLED_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::RESET_UPDATE, receive_signal);

  lighting.set_position(LIGHT_NUM, 1.f, 2.f, 3.f);
  EventQueue::distribute();

  EXPECT_EQ(LightingModelSignal::POSITION_UPDATE, signal_received);
  EXPECT_EQ(&lighting, model_received);
  EXPECT_EQ(1, num_signals_received);
}

TEST_F(LightingModelTest, ChangingAmbientColorFiresSignal) {
  constexpr GLint LIGHT_NUM{0};
  LightingModel lighting{};
  lighting.connect(LightingModelSignal::POSITION_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::COLOR_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::ENABLED_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::RESET_UPDATE, receive_signal);

  lighting.set_ambient(LIGHT_NUM, 1.f, 1.f, 1.f, 1.f);

  EventQueue::distribute();

  EXPECT_EQ(LightingModelSignal::COLOR_UPDATE, signal_received);
  EXPECT_EQ(&lighting, model_received);
  EXPECT_EQ(1, num_signals_received);
}

TEST_F(LightingModelTest, ChangingDiffuseColorFiresSignal) {
  constexpr GLint LIGHT_NUM{0};
  LightingModel lighting{};
  lighting.connect(LightingModelSignal::POSITION_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::COLOR_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::ENABLED_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::RESET_UPDATE, receive_signal);

  lighting.set_diffuse(LIGHT_NUM, 1.f, 1.f, 1.f, 1.f);

  EventQueue::distribute();

  EXPECT_EQ(LightingModelSignal::COLOR_UPDATE, signal_received);
  EXPECT_EQ(&lighting, model_received);
  EXPECT_EQ(1, num_signals_received);
}

TEST_F(LightingModelTest, ChangingSpecularColorFiresSignal) {
  constexpr GLint LIGHT_NUM{0};
  LightingModel lighting{};
  lighting.connect(LightingModelSignal::POSITION_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::COLOR_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::ENABLED_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::RESET_UPDATE, receive_signal);

  lighting.set_specular(LIGHT_NUM, 1.f, 1.f, 1.f, 1.f);

  EventQueue::distribute();

  EXPECT_EQ(LightingModelSignal::COLOR_UPDATE, signal_received);
  EXPECT_EQ(&lighting, model_received);
  EXPECT_EQ(1, num_signals_received);
}

TEST_F(LightingModelTest, ResetFiresSignal) {
  constexpr GLint LIGHT_NUM{0};
  LightingModel lighting{};
  lighting.connect(LightingModelSignal::POSITION_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::COLOR_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::ENABLED_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::RESET_UPDATE, receive_signal);

  // Ensure the lighting model is not empty.
  lighting.set_specular(LIGHT_NUM, 1.f, 1.f, 1.f, 1.f);
  EventQueue::distribute();
  clear_reception_state();

  lighting.reset();
  EventQueue::distribute();

  EXPECT_EQ(LightingModelSignal::RESET_UPDATE, signal_received);
  EXPECT_EQ(&lighting, model_received);
  EXPECT_EQ(1, num_signals_received);
}

TEST_F(LightingModelTest, MaxLightNumIsNegativeWhenEmpty) {
  LightingModel lighting{};
  EXPECT_LT(lighting.max_light_num(), 0);
}

TEST_F(LightingModelTest, MaxLightNumWorks) {
  LightingModel lighting{};
  lighting.enable(0);
  lighting.enable(3);
  lighting.enable(1);
  lighting.enable(2);
  lighting.enable(23);
  lighting.enable(17);
  lighting.enable(1);
  EXPECT_EQ(23, lighting.max_light_num());
}

TEST_F(LightingModelTest, CanConfigureFromProto) {
  const lighting::LightingConfiguration configuration =
      proto::load_text_proto<lighting::LightingConfiguration>("ui/testing/lighting_configuration.pb.txt");
  LightingModel lighting{};
  lighting.load(configuration);

  lighting::LightingConfiguration returned{};
  lighting.save(&returned);

  google::protobuf::util::MessageDifferencer differencer{};
  std::string difference_report{};
  differencer.ReportDifferencesToString(&difference_report);
  EXPECT_TRUE(differencer.Compare(configuration, returned)) << difference_report;
}

}  // namespace pack::ui
