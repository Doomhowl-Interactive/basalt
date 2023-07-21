#include <string.h>
#include <malloc.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <spdlog/spdlog.h>

#include "basalt_macros.hpp"
#include "basalt_graphics.hpp"
#include "basalt_math.hpp"
#include "basalt_console.hpp"

// TODO: Put in extra!
#define OPEN_SIMPLEX_NOISE_IMPLEMENTATION
#include "external/open-simplex-noise.h"

using namespace std;

Texture::Texture(unsigned int width, unsigned int height)
{
    if (width == 0 || height == 0) {
        spdlog::critical("Canvas width and height must be greater than 0!");
        return;
    }

    this->width = width;
    this->height = height;
    this->pixels = make_shared<vector<Color>>(vector<Color>(width * height));
}

Texture Texture::Copy()
{
    Texture copy(width, height);
    CopyInto(copy);
    return copy;
}

void Texture::CopyInto(Texture& dest)
{
    assert(dest.width == width && dest.height == height);
    dest.pixels = pixels;
}

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
    DrawRectangle(posX, posY, width, border, color);  // top
    DrawRectangle(posX + width - border, posY, border, height,
                  color);  // right
    DrawRectangle(posX, posY + height - border, width, border,
                  color);                              // bottom
    DrawRectangle(posX, posY, border, height, color);  // left
}

void Texture::SwapChannels(unsigned char first,
                           unsigned char second,
                           unsigned char third,
                           unsigned char fourth)
{
    assert(first < 4);
    assert(second < 4);
    assert(third < 4);
    assert(fourth < 4);

    // Make copy before swapping
    Texture src = Copy();
    auto& sPixels = *src.pixels.get();
    auto& dPixels = *pixels.get();

    for (int i = 0; i < src.width * src.height; i++) {
        dPixels[i * 4 + 0] = sPixels[i * 4 + first];
        dPixels[i * 4 + 1] = sPixels[i * 4 + second];
        dPixels[i * 4 + 2] = sPixels[i * 4 + third];
        dPixels[i * 4 + 3] = sPixels[i * 4 + fourth];
    }
}

void Texture::Clear(Color color)
{
    DrawRectangle(0, 0, width, height, color);
}

// ==== COLOR UTILTIES ====
inline Color MakeRGB(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}

inline Color MakeRGBf(float r, float g, float b, float a)
{
    return MakeRGB((unsigned char)(r * 255.f),
                   (unsigned char)(g * 255.f),
                   (unsigned char)(b * 255.f),
                   (unsigned char)(a * 255.f));
}

Color BlendColors(Color src, Color dst, int overrideAlpha)
{
    unsigned char alpha = overrideAlpha == -1 ? dst & 0x000000FF : (unsigned char)overrideAlpha;
    assert(alpha <= 255);
    if (alpha == 255)
        return dst;
    if (alpha == 0 || dst == WHITE)
        return src;

    const Color s = 255 - alpha;
    return ((((((src >> 0) & 0xff) * s + ((dst >> 0) & 0xff) * alpha) >> 8))
            | (((((src >> 8) & 0xff) * s + ((dst >> 8) & 0xff) * alpha)) & ~0xff)
            | (((((src >> 16) & 0xff) * s + ((dst >> 16) & 0xff) * alpha) << 8) & ~0xffff) | 255);
}

Color BlendColorsAlpha(Color src, Color dst, int overrideAlpha)
{
    unsigned char alpha = overrideAlpha == -1 ? dst & 0x000000FF : (unsigned char)overrideAlpha;
    assert(alpha <= 255);
    if (alpha == 255)
        return dst;
    if (alpha == 0 || dst == WHITE)
        return src;

    const Color s = 255 - alpha;
    return ((((((src >> 0) & 0xff) * s + ((dst >> 0) & 0xff) * alpha) >> 8))
            | (((((src >> 8) & 0xff) * s + ((dst >> 8) & 0xff) * alpha)) & ~0xff)
            | (((((src >> 16) & 0xff) * s + ((dst >> 16) & 0xff) * alpha) << 8) & ~0xffff)
            | (((((src >> 24) & 0xff) * s + ((dst >> 24) & 0xff) * alpha) << 16) & ~0xffffff));
}

inline Color ColorAlpha(Color col, float a)
{
    uchar alpha = (uchar)(a * 255.f);
    return (col & 0xFFFFFF00) | alpha;
}
