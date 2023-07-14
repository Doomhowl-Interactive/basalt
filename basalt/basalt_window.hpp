#pragma once
#include <string>

#include "basalt_macros.hpp"

void SetWindowTitle(std::string title);

/// Get number of frames elapsed since the start of the simulation.
ulong GetFrameIndex();

/// Get time elapsed since the start of the simulation.
double GetTimeElapsed();

/// Get deltaTime
double GetDeltaTime();
