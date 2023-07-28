#pragma once
#include "basalt_textures.hpp"

Texture GenerateNoise(int width,
                      int height,
                      Color bg,
                      Color fg,
                      double scale,
                      int seed = GetRandomNumber());
