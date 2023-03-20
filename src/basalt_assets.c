#ifndef BASALT_NO_ASSETS

#include "basalt.h"

extern unsigned char PALETTE_COLORS[];

func uint UnsignedCharsToInteger(unsigned char value[4]) {
    uint result = 0;
    result |= (value[0] << 24);
    result |= (value[1] << 16);
    result |= (value[2] << 8);
    result |= value[3];
    return result;
}

pubfunc Texture LoadTexture(uchar* pixels){
    Texture texture;

    // get texture size
    texture.width = UnsignedCharsToInteger(pixels);
    assert(texture.width > 0);
    texture.height = UnsignedCharsToInteger(&pixels[4]);
    assert(texture.height > 0);

    // allocate pixel memory
    texture.pixels = MemAlloc(texture.width*texture.height*sizeof(uint));

    // calculate every pixel's colors
    for (int i = 0; i < texture.width*texture.height; i++){

        // Color lookup from palette
        uchar index = pixels[i+4*2];
        uchar alpha = PALETTE_COLORS[index * 4 + 0];
        uchar red   = PALETTE_COLORS[index * 4 + 1];
        uchar green = PALETTE_COLORS[index * 4 + 2];
        uchar blue  = PALETTE_COLORS[index * 4 + 3];

        if (alpha == 0) {
            assert(red == 0);
            assert(green == 0);
            assert(blue == 0);
        } else {
            assert(alpha == 255);
        }

        // A RGB
        texture.pixels[i] = CreateColorA(red, green, blue, alpha);
    }
    return texture;
}

#endif
