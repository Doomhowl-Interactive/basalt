#include <string.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <spdlog/spdlog.h>

#include "basalt_graphics.hpp"
#include "basalt_textures.hpp"
#include "basalt_colors.hpp"
#include "basalt_macros.hpp"
#include "basalt_math.hpp"
#include "basalt_console.hpp"

using namespace std;

// TODO: Put in extra!
#define OPEN_SIMPLEX_NOISE_IMPLEMENTATION
#include "external/open-simplex-noise.h"

Texture Texture::GenerateNoise(unsigned int width,
                               unsigned int height,
                               Color bg,
                               Color fg,
                               double scale,
                               int seed)
{
    Texture texture(width, height);
    auto& pix = *texture.pixels.get();

    struct osn_context* context;
    open_simplex_noise(seed, &context);

    double lowest = 1000;
    double highest = -1000;
    auto values = vector<double>(width * height);

    // Determine ranges
    int i = 0;
    for (int y = 0; y < (int)texture.height; y++) {
        for (int x = 0; x < (int)texture.width; x++) {
            double val = open_simplex_noise2(context, x / scale, y / scale);
            if (lowest > val)
                lowest = val;
            if (highest < val)
                highest = val;
            values[i++] = val;
        }
    }

    // Calculate colors
    i = 0;
    for (int y = 0; y < (int)texture.height; y++) {
        for (int x = 0; x < (int)texture.width; x++) {
            float percentage = (float)((values[i] - lowest) / (highest - lowest));
            pix[i++] = BlendColors(bg, fg, percentage);
        }
    }

    open_simplex_noise_free(context);
    return texture;
}

void Texture::DrawRectangleLines(int posX, int posY, int width, int height, int border, Color color)
{
    DrawRectangle(posX, posY, width, border, color);   // top
    DrawRectangle(posX + width - border, posY, border, height,
                  color);                              // right
    DrawRectangle(posX, posY + height - border, width, border,
                  color);                              // bottom
    DrawRectangle(posX, posY, border, height, color);  // left
}
