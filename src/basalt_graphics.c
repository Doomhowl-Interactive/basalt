#include "basalt.h"

pubfunc void DrawDot(Texture canvas, int posX, int posY, int radius, Color color) {
    int halfRadius = MAX(1, radius / 2);
    int topLeftX = posX - halfRadius;
    int topLeftY = posY - halfRadius;
    DrawRectangle(canvas, topLeftX, topLeftY, radius, radius, color);
}

pubfunc void DrawDotV(Texture canvas, Vec2 pos, int radius, Color color) {
    DrawDot(canvas, pos.x, pos.y, radius, color);
}

pubfunc void DrawRectangle(Texture canvas, int posX, int posY, int width, int height, Color color) {
    assert(canvas.pixels);

    // assume color is opaque
    color |= 0xFF000000;

    int i = posY * canvas.width + posX;
    for (int y = MAX(0,posY); y < MIN(posY + height, canvas.height); y++) {
        for (int x = MAX(0,posX); x < MIN(posX + width, canvas.width); x++) {
            canvas.pixels[i++] = color;
        }
        i -= width;
        i += canvas.width;
    }
}

pubfunc void DrawRectangleRec(Texture canvas, Rect rect, Color color) {
    DrawRectangle(canvas, rect.x, rect.y, rect.width, rect.height, color);
}

pubfunc void DrawRectangleRecF(Texture canvas, RectF rect, Color color) {
    DrawRectangle(canvas, (int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, color);
}

pubfunc void DrawRectangleLines(Texture canvas, int posX, int posY, int width, int height, int border, Color color) {
    DrawRectangle(canvas, posX, posY, width, border, color); // top
    DrawRectangle(canvas, posX + width - border, posY, border, height, color); // right
    DrawRectangle(canvas, posX, posY + height - border, width, border, color); // bottom
    DrawRectangle(canvas, posX, posY, border, height, color); // left
}

pubfunc void DrawRectangleLinesRec(Texture canvas, Rect rect, int border, Color color) {
    DrawRectangleLines(canvas, rect.x, rect.y, rect.width, rect.height, border, color);
}

pubfunc void DrawRectangleLinesRecF(Texture canvas, RectF rect, int border, Color color) {
    DrawRectangleLines(canvas, (int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, border, color);
}

pubfunc Texture InitTexture(int width, int height) {
    Texture tex;
    tex.width = width;
    tex.height = height;
    tex.pixels = (uint *)malloc(width * height * 4);
    return tex;
}

pubfunc void DisposeTexture(Texture texture) {
    if (texture.pixels) {
        free(texture.pixels);
    }
}

pubfunc Texture CopyTexture(Texture texture) {
    Texture copy = InitTexture(texture.width, texture.height);
    memcpy(copy.pixels, texture.pixels, texture.width * texture.height * 4);
    return copy;
}

pubfunc void ClearTexture(Texture canvas, Color color) {
    assert(canvas.pixels);
    for (int i = 0; i < canvas.width * canvas.height; i++) {
        canvas.pixels[i] = color;
    }
}

pubfunc void DrawTexture(Texture canvas, Texture texture, int posX, int posY) {
    Vec2 pos = {posX, posY};
    Rect src = {0, 0, texture.width, texture.height};
    DrawTextureEx(canvas, texture, pos, src);
}

pubfunc void DrawTextureV(Texture canvas, Texture texture, Vec2 pos) {
    Rect src = {0, 0, texture.width, texture.height};
    DrawTextureEx(canvas, texture, pos, src);
}

pubfunc void DrawTextureEx(Texture canvas, Texture texture, Vec2 pos, Rect src) {
    assert(canvas.pixels);
    Color *pixels = (Color *)canvas.pixels;

    // TODO: optimize
    // TODO: Bounds checking!
    for (int y = 0; y < src.height; y++) {
        for (int x = 0; x < src.width; x++) {
            int xx = pos.x + x;
            int yy = pos.y + y;
            int destIndex = yy * canvas.width + xx;
            int srcIndex = (src.y + y) * texture.width + (src.x + x);

            // drop pixel if out of bounds
            if (srcIndex < 0 || srcIndex >= texture.width*texture.height) {
                continue;
            }

            if ((texture.pixels[srcIndex] & 0xFF000000) == 0xFF000000){
                pixels[destIndex] = texture.pixels[srcIndex];
            }
        }
    }
}

// TODO: this entire function could be optimized
pubfunc void DrawTextureScaled(Texture canvas, Texture texture, Vec2 pos, float scale) {
    assert(canvas.pixels);
    assert(texture.pixels);

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

pubfunc void DrawWeirdTestGradient(Texture canvas) {
    assert(canvas.pixels);

    static int xOffset = 0;
    static int yOffset = 0;

    int i = 0;
    for (int y = 0; y < canvas.height; y++) {
        for (int x = 0; x < canvas.width; x++) {
            uchar red = 20;
            uchar green = x + xOffset;
            uchar blue = y + yOffset;
            canvas.pixels[i] = CreateColor(red,green,blue);
            i++;
        }
    }

    xOffset++;
    yOffset++;
}

pubfunc Color CreateColorA(uchar r, uchar g, uchar b, uchar a){
    return (a << 24) | (r << 16) | (g << 8) | b;
}

pubfunc Color CreateColor(uchar r, uchar g, uchar b){
    return CreateColorA(r, g, b, 255);
}
