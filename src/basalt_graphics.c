#include "basalt.h"
#include <math.h>
#include <string.h>

void DrawDot(Texture canvas, int posX, int posY, int radius, int32 color) {
    int halfRadius = MAX(1, radius / 2);
    int topLeftX = posX - halfRadius;
    int topLeftY = posY - halfRadius;
    DrawRectangle(canvas, topLeftX, topLeftY, radius, radius, color);
}

void DrawDotV(Texture canvas, Vec2 pos, int radius, int32 color) {
    DrawDot(canvas, pos.x, pos.y, radius, color);
}

void DrawRectangle(Texture canvas, int posX, int posY, int width, int height, int32 color) {
    Assert(canvas.pixels);

    int i = posY * canvas.width + posX;
    for (int y = posY; y < posY + height; y++) {
        for (int x = posX; x < posX + width; x++) {
            canvas.pixels[i++] = color;
        }
        i -= width;
        i += canvas.width;
    }
}

void DrawRectangleRec(Texture canvas, Rect rect, int32 color) {
    DrawRectangle(canvas, rect.x, rect.y, rect.width, rect.height, color);
}

void DrawRectangleRecF(Texture canvas, RectF rect, int32 color) {
    DrawRectangle(canvas, (int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, color);
}

void DrawRectangleLines(Texture canvas, int posX, int posY, int width, int height, int border, int32 color) {
    DrawRectangle(canvas, posX, posY, width, border, color); // top
    DrawRectangle(canvas, posX + width - border, posY, border, height,
                                color); // right
    DrawRectangle(canvas, posX, posY + height - border, width, border,
                                color); // bottom
    DrawRectangle(canvas, posX, posY, border, height, color); // left
}

void DrawRectangleLinesRec(Texture canvas, Rect rect, int border, int32 color) {
    DrawRectangleLines(canvas, rect.x, rect.y, rect.width, rect.height, border, color);
}

void DrawRectangleLinesRecF(Texture canvas, RectF rect, int border, int32 color) {
    DrawRectangleLines(canvas, (int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, border, color);
}

Texture InitTexture(int width, int height) {
    Texture tex;
    tex.width = width;
    tex.height = height;
    tex.pixels = (uint *)MemAllocEx(width * height, 4);
    return tex;
}

void DisposeTexture(Texture texture) {
    if (texture.pixels) {
        free(texture.pixels);
    }
}

Texture CopyTexture(Texture texture) {
    Texture copy = InitTexture(texture.width, texture.height);
    memcpy(copy.pixels, texture.pixels, texture.width * texture.height * 4);
    return copy;
}

void ClearTexture(Texture canvas, int32 color) {
    Assert(canvas.pixels);
    for (int i = 0; i < canvas.width * canvas.height; i++) {
        canvas.pixels[i] = color;
    }
}

void BlitTexture(Texture canvas, Texture texture, int posX, int posY) {
    Vec2 pos = {posX, posY};
    Rect src = {0, 0, texture.width, texture.height};
    BlitTextureEx(canvas, texture, pos, src);
}

void BlitTextureV(Texture canvas, Texture texture, Vec2 pos) {
    Rect src = {0, 0, texture.width, texture.height};
    BlitTextureEx(canvas, texture, pos, src);
}

void BlitTextureEx(Texture canvas, Texture texture, Vec2 pos, Rect src) {
    Assert(canvas.pixels);
    uint *pixels = (uint *)canvas.pixels;

    // TODO: optimize
    for (int y = 0; y < src.height; y++) {
        for (int x = 0; x < src.width; x++) {
            int xx = pos.x + x;
            int yy = pos.y + y;
            int destIndex = yy * canvas.width + xx;
            int srcIndex = (src.y + y) * texture.width + (src.x + x);
            pixels[destIndex] = texture.pixels[srcIndex];
        }
    }
}

// TODO: this entire function could be optimized
void BlitTextureScaled(Texture canvas, Texture texture, Vec2 pos, float scale) {
    Assert(canvas.pixels);
    Assert(texture.pixels);

    uint *dest = (uint *)canvas.pixels;
    const uint *src = (const uint *)texture.pixels;

    int blitHeight = MIN(texture.height * scale, canvas.height - pos.y);
    int blitWidth = MIN(texture.width * scale, canvas.width - pos.x);

    int originX = MAX(0, pos.x);
    int originY = MAX(0, pos.y);

    for (int destY = originY; destY < blitHeight; ++destY) {
        for (int destX = originX; destX < blitWidth; ++destX) {
            int destIndex = destY * canvas.width + destX;

            // TODO: this might look ugly with non-integer scaling
            int x = (int)((destX - originX) / (float)blitWidth * texture.width);
            int y = (int)((destY - originY) / (float)blitHeight * texture.height);
            int srcIndex = y * texture.width + x;

            assert(srcIndex >= 0 && srcIndex < texture.width * texture.height);
            dest[destIndex] = src[srcIndex];
        }
    }
}

void RenderWeirdTestGradient(Texture canvas) {
    Assert(canvas.pixels);

    static int xOffset = 0;
    static int yOffset = 0;

    int i = 0;
    for (int y = 0; y < canvas.height; y++) {
        for (int x = 0; x < canvas.width; x++) {
            uchar blue = (x + xOffset);
            uchar green = (y + yOffset);
            canvas.pixels[i] = ((green << 16) | blue);
            i++;
        }
    }

    xOffset++;
    yOffset++;
}
