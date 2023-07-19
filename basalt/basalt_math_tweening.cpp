#include "basalt_math_tweening.hpp"

float TweenLinear(float t, float a, float b)
{
    return Lerp(a, b, t);
}

float TweenSmooth(float t, float a, float b)
{
    return Lerp(a, b, t * t * (3.0f - 2.0f * t));
}