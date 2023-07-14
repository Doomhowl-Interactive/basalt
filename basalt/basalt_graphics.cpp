#include <string.h>
#include <malloc.h>
#include <iostream>
#include <vector>
#include <cassert>

#include "basalt_macros.hpp"
#include "basalt_graphics.hpp"
#include "basalt_math.hpp"
#include "basalt_console.hpp"

#define OPEN_SIMPLEX_NOISE_IMPLEMENTATION
#include "external/open-simplex-noise.h"
#include <spdlog/spdlog.h>

using namespace std;

constexpr int BLEND_VALUE = 180;

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

// NOTE: Taken from https://github.com/tsoding/olive.c/blob/master/olive.c
struct Olivec_Normalized_Rect {
    // Safe ranges to iterate over.
    int x1, x2;
    int y1, y2;

    // Original uncut ranges some parts of which may be outside of the canvas
    // boundaries.
    int ox1, ox2;
    int oy1, oy2;
};

void Texture::DrawDot(int posX, int posY, Color color)
{
    auto& pix = *pixels.get();
    pix[posY * width + posX] = color;
}

// NOTE: Taken from https://github.com/tsoding/olive.c/blob/master/olive.c
void Texture::DrawLine(int startX, int startY, int endX, int endY, Color color)
{
    int dx = endX - startX;
    int dy = endY - startY;

    // If both of the differences are 0 there will be a division by 0 below.
    if (dx == 0 && dy == 0) {
        if (0 <= startX && startX < (int)width && 0 <= startY && startY < (int)height) {
            DrawDot(startX, startY, color);
        }
        return;
    }

    if (abs(dx) > abs(dy)) {
        if (startX > endX) {
            swap(startX, endX);
            swap(startY, endY);
        }

        // Cull out invisible line
        if (startX > (int)width)
            return;
        if (endX < 0)
            return;

        // Clamp the line to the boundaries
        if (startX < 0)
            startX = 0;
        if (endX >= (int)width)
            endX = (int)width - 1;

        for (int x = startX; x <= endX; ++x) {
            int y = dy * (x - startX) / dx + startY;
            if (0 <= y && y < (int)height) {
                DrawDot(x, y, color);
            }
        }
    } else {
        if (startY > endY) {
            swap(startX, endX);
            swap(startY, endY);
        }

        // Cull out invisible line
        if (startY > (int)height)
            return;
        if (endY < 0)
            return;

        // Clamp the line to the boundaries
        if (startY < 0)
            startY = 0;
        if (endY >= (int)height)
            endY = (int)height - 1;

        for (int y = startY; y <= endY; ++y) {
            int x = dx * (y - startY) / dy + startX;
            if (0 <= x && x < (int)width) {
                DrawDot(x, y, color);
            }
        }
    }
}

void Texture::DrawRectangle(int posX, int posY, int width, int height, Color color)
{
    auto& pix = *pixels.get();

    // assume color is opaque
    color |= 0x000000FF;

    for (int y = max(0, posY); y < min(posY + height, (int)height); y++) {
        for (int x = max(0, posX); x < min(posX + width, (int)width); x++) {
            int j = y * width + x;
            pix[j] = color;
        }
    }
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
    auto& p = *pixels.get();
    for (int i = 0; i < width * height; i++) {
        p[i] = color;
    }
}

void Texture::Blit(Texture texture,
                   int posX,
                   int posY,
                   Color tint,
                   int srcX,
                   int srcY,
                   int srcWidth,
                   int srcHeight)
{
    const auto& oPix = *texture.pixels.get();
    auto& pix = *pixels.get();

    if (srcWidth < 0)
        srcWidth = texture.width;
    if (srcHeight < 0)
        srcHeight = texture.width;

    // Switch out the texture if hotreloading is on
    // HotReloadTexture(texture);

    for (int destY = max(0, posY); destY < Clamp(posY + srcHeight, 0, (int)height); destY++) {
        for (int destX = max(0, posX); destX < Clamp(posX + srcWidth, 0, (int)width); destX++) {
            int sourceX = destX - posX + srcX;
            int sourceY = destY - posY + srcY;

            int srcIndex = sourceY * texture.width + sourceX;
            int destIndex = destY * width + destX;

            Color srcColor = oPix[srcIndex];
            unsigned char alpha = srcColor & 0x000000FF;
            Color tintedColor = BlendColors(srcColor, tint, BLEND_VALUE);
            Color finalColor = BlendColors(oPix[destIndex], tintedColor, alpha);

            pix[destIndex] = finalColor;
        }
    }
}

static bool olivec_normalize_rect(int x,
                                  int y,
                                  int w,
                                  int h,
                                  size_t canvas_width,
                                  size_t canvas_height,
                                  Olivec_Normalized_Rect* nr);

// NOTE: Taken from https://github.com/tsoding/olive.c/blob/master/olive.c
void Texture::BlitScaled(Texture texture,
                         int destX,
                         int destY,
                         int destWidth,
                         int destHeight,
                         Color tint)
{
    const auto& oPix = *texture.pixels.get();
    auto& pix = *pixels.get();

    Olivec_Normalized_Rect nr = { 0 };
    if (!olivec_normalize_rect(destX, destY, destWidth, destHeight, width, height, &nr))
        return;

    int xa = nr.ox1;
    if (destWidth < 0)
        xa = nr.ox2;
    int ya = nr.oy1;
    if (destHeight < 0)
        ya = nr.oy2;
    for (int y = nr.y1; y <= nr.y2; ++y) {
        for (int x = nr.x1; x <= nr.x2; ++x) {
            size_t nx = (x - xa) * ((int)texture.width) / destWidth;
            size_t ny = (y - ya) * ((int)texture.height) / destHeight;
            size_t srcIndex = ny * texture.width + nx;
            size_t destIndex = y * width + x;
            Color color = oPix[srcIndex];
            if (tint != WHITE)
                color = BlendColors(color, tint, BLEND_VALUE);
            pix[destIndex] = color;
        }
    }
}

void Texture::BlitSheet(TextureSheet sheet, int frame, Vec2 pos, Color tint)
{
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
    Blit(sheet.texture, V2(pos), tint, R2(source));
}

void Texture::DrawWeirdTestGradient()
{
    auto& pix = *pixels.get();

    static int xOffset = 0;
    static int yOffset = 0;

    int i = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char red = x + xOffset;
            unsigned char green = y + xOffset;
            pix[i] = MakeRGB(red, green, 0);
            i++;
        }
    }

    xOffset++;
    yOffset++;
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

Color BlendColors(Color src, Color dst, unsigned char t)
{
    assert(t <= 255);
    if (t == 255)
        return dst;
    if (t == 0 || dst == WHITE)
        return src;

    const Color s = 255 - t;
    return ((((((src >> 0) & 0xff) * s + ((dst >> 0) & 0xff) * t) >> 8))
            | (((((src >> 8) & 0xff) * s + ((dst >> 8) & 0xff) * t)) & ~0xff)
            | (((((src >> 16) & 0xff) * s + ((dst >> 16) & 0xff) * t) << 8) & ~0xffff)
            | (((((src >> 24) & 0xff) * s + ((dst >> 24) & 0xff) * t) << 16) & ~0xffffff));
}

inline Color ColorAlpha(Color col, float a)
{
    return (col & 0x00FFFFFF) | ((unsigned char)(a * 255) << 24);
}

// NOTE: Taken from https://github.com/tsoding/olive.c/blob/master/olive.c
static bool olivec_normalize_rect(int x,
                                  int y,
                                  int w,
                                  int h,
                                  size_t canvas_width,
                                  size_t canvas_height,
                                  Olivec_Normalized_Rect* nr)
{
    // No need to render empty rectangle
    if (w == 0)
        return false;
    if (h == 0)
        return false;

    nr->ox1 = x;
    nr->oy1 = y;

    // Convert the rectangle to 2-points representation
    nr->ox2 = nr->ox1 + Sign(w) * (abs(w) - 1);
    if (nr->ox1 > nr->ox2)
        swap(nr->ox1, nr->ox2);
    nr->oy2 = nr->oy1 + Sign(h) * (abs(h) - 1);
    if (nr->oy1 > nr->oy2)
        swap(nr->oy1, nr->oy2);

    // Cull out invisible rectangle
    if (nr->ox1 >= (int)canvas_width)
        return false;
    if (nr->ox2 < 0)
        return false;
    if (nr->oy1 >= (int)canvas_height)
        return false;
    if (nr->oy2 < 0)
        return false;

    nr->x1 = nr->ox1;
    nr->y1 = nr->oy1;
    nr->x2 = nr->ox2;
    nr->y2 = nr->oy2;

    // Clamp the rectangle to the boundaries
    if (nr->x1 < 0)
        nr->x1 = 0;
    if (nr->x2 >= (int)canvas_width)
        nr->x2 = (int)canvas_width - 1;
    if (nr->y1 < 0)
        nr->y1 = 0;
    if (nr->y2 >= (int)canvas_height)
        nr->y2 = (int)canvas_height - 1;

    return true;
}
