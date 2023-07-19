#pragma once
#include "basalt_math.hpp"

inline float TweenLinear(float a, float b, float t)
{
    return Lerp(a, b, t);
}

inline float TweenSmooth(float a, float b, float t)
{
    return Lerp(a, b, t * t * (3.0f - 2.0f * t));
}