// The Legacy Renderer: The original pixel-by-pixel software renderer.
// Provides the most control over the rendering process, but is also the slowest.
#include <iostream>

#include "pch.h"
#include "framework.h"

#include "basalt_graphics.hpp"

using namespace std;

constexpr int BLEND_VALUE = 180;

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
    unsigned char alpha = color & 0x000000FF;
    int i = posY * width + posX;
    if (i < 0 || i >= width * height)
        return;
    pix[i] = BlendColors(pix[i], color, alpha);
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

    unsigned char alpha = color & 0x000000FF;
    for (int y = max(0, posY); y < min(posY + height, (int)this->height); y++) {
        for (int x = max(0, posX); x < min(posX + width, (int)this->width); x++) {
            int j = y * this->width + x;
            pix[j] = BlendColors(pix[j], color, alpha);
        }
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
    // Slightly faster than using the vectors normally.
    const Color* srcPix = texture.pixels.get()->data();
    Color* pix = pixels.get()->data();

    if (srcWidth < 0)
        srcWidth = texture.width;
    if (srcHeight < 0)
        srcHeight = texture.width;

    // Switch out the texture if hotreloading is on
    // HotReloadTexture(texture);
    //
    // WARN: Horrible slow shit
    for (int destY = max(0, posY); destY < Clamp(posY + srcHeight, 0, (int)height); destY++) {
        for (int destX = max(0, posX); destX < Clamp(posX + srcWidth, 0, (int)width); destX++) {
            int sourceX = destX - posX + srcX;
            int sourceY = destY - posY + srcY;

            int srcIndex = sourceY * texture.width + sourceX;
            int destIndex = destY * width + destX;

            Color srcColor = srcPix[srcIndex];
            Color tintedColor = BlendColors(srcColor, tint, BLEND_VALUE);
            Color finalColor = BlendColors(pix[destIndex], tintedColor);

            pix[destIndex] = finalColor;
        }
    }
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
        (int) (sheet.origin.x + frame * sheet.cellSize.x),
        (int) sheet.origin.y,
        (int) sheet.cellSize.x,
        (int) sheet.cellSize.y,
    };
    Blit(sheet.texture, pos, tint, source);
}
