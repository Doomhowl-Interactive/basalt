#include "basalt_math_tweening.hpp"

float TweenLinear(float t, float a, float b)
{
    return Lerp(a, b, t);
}

float TweenSmooth(float t, float a, float b)
{
    return Lerp(a, b, t * t * (3.0f - 2.0f * t));
}

float TweenSmooth2(float t, float a, float b)
{
    return Lerp(a, b, t * t * t * (t * (6.0f * t - 15.0f) + 10.0f));
}

float TweenInvExp(float t, float a, float b)
{
    return Lerp(a, b, t * t);
}

float TweenInvExp2(float t, float a, float b)
{
    return Lerp(a, b, t * t * t);
}

float TweenExp(float t, float a, float b)
{
    return Lerp(a, b, 1.0f - (1.0f - t) * (1.0f - t));
}

float TweenExp2(float t, float a, float b)
{
    return Lerp(a, b, 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t));
}