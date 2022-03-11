/**
 * Tests for the Signaller class. As it is designed to be used as a base class, we use the LightingModel class, a
 * subclass of Signaller, in these tests. LightingModel is tested elsewhere.
 */
#include "ui/signaller.h"

#include <memory>

#include "gtest/gtest.h"
#include "ui/lighting_model.h"
#include "ui/application.h"

namespace pack::ui {

LightingModelSignal signal_received{};
const LightingModel* model_received{nullptr};
int num_signals_received{0};
void receive_signal(LightingModelSignal signal, const LightingModel& lighting) {
  signal_received = signal;
  model_received = &lighting;
  ++num_signals_received;
}

LightingModelSignal signal_received2{};
const LightingModel* model_received2{nullptr};
int num_signals_received2{0};
void receive_signal2(LightingModelSignal signal, const LightingModel& lighting) {
  signal_received2 = signal;
  model_received2 = &lighting;
  ++num_signals_received2;
}

class SignallerTest : public ::testing::Test {
 public:
  std::unique_ptr<Application> application{};

  void clear_reception_state() {
    signal_received = LightingModelSignal::INVALID;
    model_received = nullptr;
    num_signals_received = 0;

    signal_received2 = LightingModelSignal::INVALID;
    model_received2 = nullptr;
    num_signals_received2 = 0;
  }


  void SetUp() override { application.reset(new Application()); }

  void TearDown() override {
    clear_reception_state();
    application.reset();
  }
};

TEST_F(SignallerTest, DISABLED_CanFireSignal) {
  LightingModel lighting{};
  lighting.connect(LightingModelSignal::POSITION_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::COLOR_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::ENABLED_UPDATE, receive_signal);

  ASSERT_FALSE(lighting.is_enabled(0));
  lighting.enable(0);

  application->distribute_events();

  EXPECT_EQ(LightingModelSignal::ENABLED_UPDATE, signal_received);
  EXPECT_EQ(&lighting, model_received);
  EXPECT_EQ(1, num_signals_received);
}

// clang-format off
/* Doesn't work. See TODO in Signaller implementation.
TEST_F(SignallerTest, DISABLED_CanDisconnectSlot) {
  LightingModel lighting{};
  lighting.connect(LightingModelSignal::POSITION_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::COLOR_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::ENABLED_UPDATE, receive_signal);

  ASSERT_FALSE(lighting.is_enabled(0));
  lighting.enable(0);

  application->distribute_events();

  ASSERT_EQ(LightingModelSignal::ENABLED_UPDATE, signal_received);
  ASSERT_EQ(&lighting, model_received);
  ASSERT_EQ(1, num_signals_received);

  clear_reception_state();

  lighting.disconnect(LightingModelSignal::POSITION_UPDATE, receive_signal);
  lighting.disconnect(LightingModelSignal::COLOR_UPDATE, receive_signal);
  lighting.disconnect(LightingModelSignal::ENABLED_UPDATE, receive_signal);

  ASSERT_TRUE(lighting.is_enabled(0));
  lighting.disable(0);

  application->distribute_events();

  EXPECT_EQ(LightingModelSignal::INVALID, signal_received);
  EXPECT_EQ(nullptr, model_received);
  EXPECT_EQ(0, num_signals_received);
}
*/
// clang-format on

TEST_F(SignallerTest, DISABLED_CanFireMultipleSignalsOfSameTypeFromSameObjectToSameDestination) {
  LightingModel lighting{};
  lighting.connect(LightingModelSignal::POSITION_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::COLOR_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::ENABLED_UPDATE, receive_signal);

  ASSERT_FALSE(lighting.is_enabled(0));
  lighting.enable(0);
  lighting.disable(0);
  lighting.enable(0);

  application->distribute_events();

  EXPECT_EQ(LightingModelSignal::ENABLED_UPDATE, signal_received);
  EXPECT_EQ(&lighting, model_received);
  EXPECT_EQ(3, num_signals_received);
}

TEST_F(SignallerTest, DISABLED_CanFireMultipleSignalsToDifferentDestinations) {
  LightingModel lighting{};
  lighting.connect(LightingModelSignal::POSITION_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::ENABLED_UPDATE, receive_signal);

  lighting.connect(LightingModelSignal::COLOR_UPDATE, receive_signal2);
  lighting.connect(LightingModelSignal::ENABLED_UPDATE, receive_signal2);

  ASSERT_FALSE(lighting.is_enabled(0));
  lighting.enable(0);
  lighting.disable(0);
  lighting.enable(0);

  application->distribute_events();

  EXPECT_EQ(LightingModelSignal::ENABLED_UPDATE, signal_received);
  EXPECT_EQ(&lighting, model_received);
  EXPECT_EQ(3, num_signals_received);

  EXPECT_EQ(LightingModelSignal::ENABLED_UPDATE, signal_received2);
  EXPECT_EQ(&lighting, model_received2);
  EXPECT_EQ(3, num_signals_received2);
}

TEST_F(SignallerTest, DISABLED_CanFireSignalsToDifferentDestinations) {
  constexpr GLint LIGHT_NUM{0};
  LightingModel lighting{};
  lighting.connect(LightingModelSignal::POSITION_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::ENABLED_UPDATE, receive_signal);

  lighting.connect(LightingModelSignal::COLOR_UPDATE, receive_signal2);
  lighting.connect(LightingModelSignal::ENABLED_UPDATE, receive_signal2);

  lighting.set_ambient(LIGHT_NUM, 1.f, 1.f, 1.f, 1.f);
  application->distribute_events();

  EXPECT_EQ(LightingModelSignal::INVALID, signal_received);
  EXPECT_EQ(nullptr, model_received);
  EXPECT_EQ(0, num_signals_received);

  EXPECT_EQ(LightingModelSignal::COLOR_UPDATE, signal_received2);
  EXPECT_EQ(&lighting, model_received2);
  EXPECT_EQ(1, num_signals_received2);

  clear_reception_state();

  lighting.set_position(LIGHT_NUM, 1.f, 2.f, 3.f);
  application->distribute_events();

  EXPECT_EQ(LightingModelSignal::POSITION_UPDATE, signal_received);
  EXPECT_EQ(&lighting, model_received);
  EXPECT_EQ(1, num_signals_received);

  EXPECT_EQ(LightingModelSignal::INVALID, signal_received2);
  EXPECT_EQ(nullptr, model_received2);
  EXPECT_EQ(0, num_signals_received2);
}

TEST_F(SignallerTest, DISABLED_CanFireMultipleSignalsOfDifferentTypes) {
  constexpr GLint LIGHT_NUM{0};
  LightingModel lighting{};
  lighting.connect(LightingModelSignal::POSITION_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::COLOR_UPDATE, receive_signal);
  lighting.connect(LightingModelSignal::ENABLED_UPDATE, receive_signal);

  ASSERT_FALSE(lighting.is_enabled(0));
  lighting.enable(0);
  lighting.set_ambient(LIGHT_NUM, 1.f, 1.f, 1.f, 1.f);

  application->distribute_events();

  // No guarantee of signal order when fired in the same distribute_events() call.
  EXPECT_TRUE((signal_received == LightingModelSignal::ENABLED_UPDATE) ||
              (signal_received == LightingModelSignal::COLOR_UPDATE));
  EXPECT_EQ(&lighting, model_received);
  EXPECT_EQ(2, num_signals_received);
}

}  // namespace pack::ui
