#ifndef BASALT_NO_ASSETS

#include "basalt.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#include "external/stb_image.h"

pubfunc Texture LoadTextureEx(const char* name, uchar* pixels) {

    int width, height;
    int channels = 0;

    uint* pixels32Bit = (uint*) pixels;

    int size = pixels32Bit[0];
    uchar* data = (uchar*) stbi_load_from_memory((stbi_uc*) &pixels32Bit[1], size,
                                                 &width, &height,
                                                 &channels, 4);

    DEBUG("Loaded texture %s of size %u with %d channels", name, size, channels);

    Texture texture = InitTexture(width, height);

    if (data == NULL) {
        ERR("Failed to load texture %s from memory! ( likely data corruption :( )", name);
        return texture;
    }

    if (channels != 4 && channels != 3) {
        ERR("Unexpected amount of channels in image (%d)!", channels);
        return texture;
    }

    // HACK: Copy the texture into the correct color order
    // (by trial and error lol)
    uchar* comps = (uchar*) texture.pixels;
    for (int i = 0; i < width * height; i++) {
        ((uchar*)texture.pixels)[i*4+0] = data[i*4+3];
        ((uchar*)texture.pixels)[i*4+1] = data[i*4+2];
        ((uchar*)texture.pixels)[i*4+2] = data[i*4+1];
        ((uchar*)texture.pixels)[i*4+3] = data[i*4+0];
    }

    stbi_image_free(data);

    if (!texture.pixels) {
        ERR("Failed to parse texture!");
        return texture;
    }
    return texture;
}

#endif
