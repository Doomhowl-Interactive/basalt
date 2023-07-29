#pragma once
#include <vector>

#include "sdl2_basalt.hpp"

std::shared_ptr<Image> SetupDefaultRenderer(SDL_Window* window);
void DrawDefaultFrame();
void DisposeDefaultRenderer();