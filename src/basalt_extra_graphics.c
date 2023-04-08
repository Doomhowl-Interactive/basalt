#include "basalt.h"
#include "basalt_extra.h"

#define OPEN_SIMPLEX_NOISE_IMPLEMENTATION
#include "external/open-simplex-noise.h"

pubfunc Texture GenerateNoiseTextureEx(int width, int height, Color bg, Color fg, double scale, int seed)
{
    Texture texture = InitTexture(width, height);

    struct osn_context* context;
    open_simplex_noise(seed,&context);

    double lowest = 1000;
    double highest = -1000;
    double values[texture.width*texture.height];

    // Determine ranges
    int i = 0;
    for (int y = 0; y < texture.height; y++)
    {
        for (int x = 0; x < texture.width; x++)
        {
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
    for (int y = 0; y < texture.height; y++)
    {
        for (int x = 0; x < texture.width; x++)
        {
            float percentage = (float) ((values[i] - lowest) / (highest - lowest));
            texture.pixels[i++] = BlendColors(bg, fg, percentage);
        }
    }

    open_simplex_noise_free(context);

    return texture;
}

pubfunc Texture GenerateNoiseTexture(int width, int height, Color bg, Color fg)
{
    int seed = GetRandomNumber();
    double scale = 60;
    return GenerateNoiseTextureEx(width, height, bg, fg, scale, seed);
}

// FIXME: SLOW AND TERRIBLE
static struct osn_context* NoiseContext = NULL;
pubfunc void DrawNoiseRectangle(Texture canvas, Rect region, Vec2 offset, Color bg, Color fg)
{
    double scale = 60;
    if (NoiseContext == NULL)
    {
        int seed = GetRandomNumber();
        open_simplex_noise(seed,&NoiseContext);
    }

    double lowest = 1000;
    double highest = -1000;
    double values[region.width*region.height];

    // TODO: Refactor
    // Determine ranges
    int i = 0;
    for (int y = 0; y < region.height; y++)
    {
        for (int x = 0; x < region.width; x++)
        {
            double xx = x + offset.x;
            double yy = y + offset.y;
            double val = open_simplex_noise2(NoiseContext, xx / scale, yy / scale);
            if (lowest > val)
                lowest = val; 
            if (highest < val)
                highest = val; 
            values[i++] = val;
        }
    }

    // Calculate colors
    for (int y = 0; y < region.height; y++)
    {
        for (int x = 0; x < region.width; x++)
        {
            int i = y * region.width + x;
            float percentage = (float) ((values[i] - lowest) / (highest - lowest));

            int j = Clamp((y+region.y) * region.width + (x+region.x), 0, canvas.width*canvas.height);
            canvas.pixels[j] = BlendColors(bg, fg, percentage);
        }
    }
}

pubfunc void DrawTextureSheet(Texture canvas, TextureSheet sheet, int frame, Vec2 pos)
{
    assert(sheet.texture);
    assert(sheet.texture->pixels);

    // wrap frame if needed
    frame = frame % sheet.count;
    if (frame < 0){
        frame = sheet.count - frame;
    }

    Rect source = {
        sheet.origin.x + frame * sheet.cellSize.x,
        sheet.origin.y,
        sheet.cellSize.x,
        sheet.cellSize.y,
    };
    DrawTextureEx(canvas, *sheet.texture, pos, source);
}
