#include <vector>

#include "basalt_images.hpp"
#include "basalt_colors.hpp"

using namespace std;

#define OPEN_SIMPLEX_NOISE_IMPLEMENTATION
#include "external/open-simplex-noise.h"

Image GenerateNoise(int width, int height, Color bg, Color fg, double scale, int seed)
{
    Image texture(width, height);
    auto pix = vector<Color>(width * height);

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
