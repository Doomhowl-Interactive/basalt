#pragma once
#include <string>

#include "basalt_macros.hpp"

namespace basalt {

using namespace std;

void SetWindowTitle(string title);

// NOTE: Get number of frames elapsed since the start of the simulation.
ulong GetFrameIndex();

// NOTE: Get time elapsed since the start of the simulation.
double GetTimeElapsed();

}  // namespace basalt