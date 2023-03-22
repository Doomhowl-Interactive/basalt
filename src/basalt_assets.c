#ifndef BASALT_NO_ASSETS

#include "basalt.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#include "external/stb_image.h"

pubfunc Texture LoadTextureEx(const char* name, uchar* pixels) {
    Texture texture;

    int channels = 0;
    int32* pixels32Bit = (int32*) pixels;

    int size = pixels32Bit[0];
    uchar* data = (uchar*) stbi_load_from_memory((stbi_uc*) &pixels32Bit[1], size,
                                                 &texture.width, &texture.height,
                                                 &channels, 4);
    if (data == NULL) {
        ERR("Failed to load texture %s from memory! ( likely data corruption :( )", name);
        return texture;
    }

    // If there are only 3 channels in the image, add an alpha channel
    if(channels == 3) {
        int count = texture.width*texture.height;
        Color* newPixels = malloc(count*sizeof(Color));
        for (uint i = 0; i < count; i++){
            uchar r = data[i+0];
            uchar g = data[i+1];
            uchar b = data[i+2];
            uchar a = data[i+3];
            newPixels[i] = CreateColorA(r,g,b,a);
        }

        stbi_image_free(texture.pixels);
        texture.pixels = newPixels;
    } else if (channels == 4){
        texture.pixels = (Color*) data;
    } else {
        ERR("Unexpected amount of channels in image (%d)!", channels);
        return texture;
    }

    if (!texture.pixels) {
        ERR("Failed to parse texture!");
        return texture;
    }

    DEBUG("Loaded texture %s of size %d with %d channels...", name, size, channels);
    return texture;
}

#endif
