#pragma once
#include <iostream>

#include "basalt_macros.hpp"

typedef uint32_t Color;

#pragma region Color Constants
constexpr Color WHITE = 0xFFFFFFFF;
constexpr Color GRAY = 0xAAAAAAFF;
constexpr Color LIGHTGRAY = 0xC8C8C8FF;
constexpr Color DARKGRAY = 0x505050FF;
constexpr Color BLACK = 0x000000FF;
constexpr Color BLANK = 0xFFFFFF00;

constexpr Color RED = 0xFF0000FF;
constexpr Color GREEN = 0x00FF00FF;
constexpr Color DARKGREEN = 0x00752CFF;
constexpr Color BLUE = 0x0000FFFF;
constexpr Color DARKBLUE = 0x00527FFF;
constexpr Color YELLOW = 0xFFFF00FF;
constexpr Color PURPLE = 0x00FFFFFF;
constexpr Color BROWN = 0x7F3F00FF;
constexpr Color DARKBROWN = 0x5A2E00FF;
constexpr Color DARKRED = 0x7F0000FF;
#pragma endregion

// ==== COLOR UTILTIES ====
extern Color MakeRGB(uchar r, uchar g, uchar b, uchar a = 255);
extern Color MakeRGBf(float r, float g, float b, float a = 1.f);
extern Color ColorAlpha(Color col, float a);
Color BlendColors(Color src, Color dest, int overrideAlpha = -1);
Color BlendColorsAlpha(Color src, Color dest, int overrideAlpha = -1);