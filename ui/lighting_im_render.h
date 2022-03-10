#pragma once

#include "ui/lighting_model.h"

namespace pack::ui {

void lighting_im_render(const LightingModel& lighting);
void lighting_im_signal_render(LightingModelSignal signal, const LightingModel& lighting);

}  // namespace pack::ui
