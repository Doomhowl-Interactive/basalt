#include "basalt_math_tweening.hpp"

float TweenLinear(float t, float a, float b)
{
    return Lerp(t, a, b);
}

float TweenSmooth(float t, float a, float b)
{
    return Lerp(t * t * (3.0f - 2.0f * t), a, b);
}

float TweenSmooth2(float t, float a, float b)
{
    return Lerp(t * t * t * (t * (6.0f * t - 15.0f) + 10.0f), a, b);
}

float TweenInvExp(float t, float a, float b)
{
    return Lerp(t * t, a, b);
}

float TweenInvExp2(float t, float a, float b)
{
    return Lerp(t * t * t, a, b);
}

float TweenExp(float t, float a, float b)
{
    return Lerp(1.0f - (1.0f - t) * (1.0f - t), a, b);
}

float TweenExp2(float t, float a, float b)
{
    return Lerp(1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t), a, b);
}