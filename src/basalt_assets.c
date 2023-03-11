#include "basalt.h"

#ifndef BASALT_NO_ASSETS

#define STBI_NO_STDIO
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

Texture LoadTexture(uchar* pixels){
    uint len = 0;

    if (IsLittleEndian()) {
        len = ((uint)pixels[0] << 24) |
              ((uint)pixels[1] << 16) |
              ((uint)pixels[2] << 8)  |
              ((uint)pixels[3] << 0);
    } else {
        len = *(uint*)pixels;
    }

    const stbi_uc* data = (const stbi_uc*) &pixels[4];

    Texture texture;
    int channels = 0;

    uint* imgData = (uint*) stbi_load_from_memory(data, len, &texture.width, &texture.height, &channels, 4);
    if (imgData == NULL) {
        exit(1);
    }

    // TODO: deal with 3 channels
    // TODO: check if windows can draw RGBA so this doesn't need to be converted
    int pixelCount = texture.width * texture.height;

    // rearrange each pixel to be BGRA (for windows bitmaps)
    uint* pixel = imgData;
    for (int i = 0; i < pixelCount; i++){

        // Extract the red, green, blue, and alpha channels
        // TODO: find out why this is weird
        uchar green = (*pixel >> 0)   & 0xFF;
        uchar red =   (*pixel >> 8)   & 0xFF;
        uchar alpha = (*pixel >> 16)  & 0xFF;
        uchar blue =  (*pixel >> 24)  & 0xFF;
       
        // Create a new BGRA color value by swapping the red and blue channels
        uint bgraColor = (blue << 24) | (green << 16) | (red << 8) | alpha;

        // WARN: swap is done in place, so don't do stb operations after this!
        *pixel = bgraColor;
        pixel++;
    }

    texture.pixels = imgData;
    assert(channels == 4);

    return texture;
}

#endif
