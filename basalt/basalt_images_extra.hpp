#pragma once
#include "basalt_images.hpp"

Image GenerateNoise(int width,
                    int height,
                    Color bg,
                    Color fg,
                    double scale,
                    int seed = GetRandomNumber());
