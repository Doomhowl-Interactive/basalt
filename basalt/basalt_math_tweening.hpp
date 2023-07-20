#pragma once
#include "basalt_math.hpp"

//
// See the Basalt Tween Viewer program
// for a visual representation of these functions.
//

float TweenLinear(float t, float a = 0.f, float b = 1.f);
float TweenSmooth(float t, float a = 0.f, float b = 1.f);
float TweenSmooth2(float t, float a = 0.f, float b = 1.f);
float TweenInvExp(float t, float a = 0.f, float b = 1.f);
float TweenInvExp2(float t, float a = 0.f, float b = 1.f);
float TweenExp(float t, float a = 0.f, float b = 1.f);
float TweenExp2(float t, float a = 0.f, float b = 1.f);
