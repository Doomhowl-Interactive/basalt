#include "basalt.h"
#include <math.h>
#include <string.h>

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

void BlitTexture(Texture canvas, Texture texture, int posX, int posY) {
    uint *pixels = (uint*) canvas.pixels;

    // TODO: optimize
    int j = 0;
    for (int y = 0; y < texture.height; y++){
        for (int x = 0; x < texture.width; x++){
            int xx = posX + x;
            int yy = posY + y;
            int i = yy * canvas.width + xx;
            pixels[i] = texture.pixels[j++];
        }
    }
}

void BlitTextureV(Texture canvas, Texture texture, Vec2 pos) {
    BlitTextureEx(canvas, texture, pos, 1.f);
}

// TODO: this entire function could be optimized
void BlitTextureEx(Texture canvas, Texture texture, Vec2 pos, float scale) {
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
            int y =
                (int)((destY - originY) / (float)blitHeight * texture.height);
            int srcIndex = y * texture.width + x;

            assert(srcIndex >= 0 && srcIndex < texture.width * texture.height);
            dest[destIndex] = src[srcIndex];
        }
    }
}

void RenderWeirdTestGradient(Texture canvas) {
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
