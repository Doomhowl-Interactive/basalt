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

    DEBUG("Loaded texture %s of size %u with %d channels", name, size, channels);

    if (data == NULL) {
        ERR("Failed to load texture %s from memory! ( likely data corruption :( )", name);
        return texture;
    }

    if (channels != 4 && channels != 3) {
        ERR("Unexpected amount of channels in image (%d)!", channels);
        return texture;
    }
    texture.pixels = (Color*) data;

    if (!texture.pixels) {
        ERR("Failed to parse texture!");
        return texture;
    }
    return texture;
}

#endif
