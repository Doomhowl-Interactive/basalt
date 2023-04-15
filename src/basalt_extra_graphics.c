#include "basalt.h"
#include "basalt_extra.h"

#define OPEN_SIMPLEX_NOISE_IMPLEMENTATION
#include "external/open-simplex-noise.h"

BASALT Texture GenerateNoiseTextureEx(int width, int height, Color bg, Color fg, double scale, int seed)
{
    Texture texture = InitTexture(width, height);

    struct osn_context* context;
    open_simplex_noise(seed, &context);

    double lowest = 1000;
    double highest = -1000;
    double* values = malloc(texture.width * texture.height * sizeof(double));

    // Determine ranges
    int i = 0;
    for (int y = 0; y < texture.height; y++) {
        for (int x = 0; x < texture.width; x++) {
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
    for (int y = 0; y < texture.height; y++) {
        for (int x = 0; x < texture.width; x++) {
            float percentage = (float)((values[i] - lowest) / (highest - lowest));
            texture.pixels[i++] = BlendColors(bg, fg, percentage);
        }
    }

    open_simplex_noise_free(context);
    free(values);

    return texture;
}

BASALT Texture GenerateNoiseTexture(int width, int height, Color bg, Color fg)
{
    int seed = GetRandomNumber();
    double scale = 60;
    return GenerateNoiseTextureEx(width, height, bg, fg, scale, seed);
}

BASALT void DrawTextureSheet(Texture canvas, TextureSheet sheet, int frame, Vec2 pos, Color tint)
{
    assert(sheet.texture);
    assert(sheet.texture->pixels);

    // wrap frame if needed
    frame = frame % sheet.count;
    if (frame < 0) {
        frame = sheet.count - frame;
    }

    Rect source = {
        sheet.origin.x + frame * sheet.cellSize.x,
        sheet.origin.y,
        sheet.cellSize.x,
        sheet.cellSize.y,
    };
    DrawTextureEx(canvas, *sheet.texture, V2(pos), R2(source), tint);
}
