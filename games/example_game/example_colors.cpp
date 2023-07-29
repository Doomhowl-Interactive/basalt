#include <iostream>

#include "example_game.hpp"

Color InterpolateHue(float t)
{
    float hue = fmodf(t, 1.0f);  // Wrap the interpolation parameter if it goes beyond 1

    // Map the hue to the range of rainbow colors (0-360 degrees)
    float hue_degrees = hue * 360.0f;

    // Convert hue from degrees to RGB values
    float red, green, blue;

    if (hue_degrees < 60.0f) {
        red = 1.0f;
        green = hue_degrees / 60.0f;
        blue = 0.0f;
    } else if (hue_degrees < 120.0f) {
        red = (120.0f - hue_degrees) / 60.0f;
        green = 1.0f;
        blue = 0.0f;
    } else if (hue_degrees < 180.0f) {
        red = 0.0f;
        green = 1.0f;
        blue = (hue_degrees - 120.0f) / 60.0f;
    } else if (hue_degrees < 240.0f) {
        red = 0.0f;
        green = (240.0f - hue_degrees) / 60.0f;
        blue = 1.0f;
    } else if (hue_degrees < 300.0f) {
        red = (hue_degrees - 240.0f) / 60.0f;
        green = 0.0f;
        blue = 1.0f;
    } else {
        red = 1.0f;
        green = 0.0f;
        blue = (360.0f - hue_degrees) / 60.0f;
    }
    return MakeRGBf(red, green, blue, 1.f);
}
