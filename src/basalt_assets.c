#include "basalt.h"
#ifndef BASALT_NO_ASSETS

extern unsigned char PALETTE_COLORS[];

uint UnsignedCharsToInteger(unsigned char value[4]) {
    uint result = 0;
    result |= (value[0] << 24);
    result |= (value[1] << 16);
    result |= (value[2] << 8);
    result |= value[3];
    return result;
}

Texture LoadTexture(uchar* pixels){

    Texture texture;

    // get texture size
    texture.width = UnsignedCharsToInteger(pixels);
    Assert(texture.width > 0);
    texture.height = UnsignedCharsToInteger(&pixels[4]);
    Assert(texture.height > 0);

    // allocate pixel memory
    texture.pixels = malloc(texture.width*texture.height*sizeof(uint));

    // calculate every pixel's colors
    for (int i = 0; i < texture.width*texture.height; i++){

        // Color lookup from palette
        uchar index = pixels[i+4*2];
        Assert(index < 256 * 4);
        uchar red   = PALETTE_COLORS[index * 4 + 0];
        uchar green = PALETTE_COLORS[index * 4 + 1];
        uchar blue  = PALETTE_COLORS[index * 4 + 2];
        uchar alpha = PALETTE_COLORS[index * 4 + 3];
        Assert(alpha == 0 || alpha == 255);

        uint argbColor = (alpha << 24) | (red << 16) | (green << 8) | blue;
        texture.pixels[i] = argbColor;
    }
    return texture;
}

#endif
