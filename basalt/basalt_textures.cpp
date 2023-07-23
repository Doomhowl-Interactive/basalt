#include <spdlog/spdlog.h>
#include <vector>

#include "basalt_colors.hpp"
#include "basalt_textures.hpp"

using namespace std;

Texture::Texture(unsigned int width, unsigned int height)
{
    if (width == 0 || height == 0) {
        spdlog::critical("Canvas width and height must be greater than 0!");
        return;
    }

    this->width = width;
    this->height = height;
    this->pixels = make_shared<vector<Color>>(vector<Color>(width * height));
}

void Texture::Clear(Color color)
{
    DrawRectangle(0, 0, width, height, color);
}

Texture Texture::Copy()
{
    Texture copy(width, height);
    CopyInto(copy);
    return copy;
}

void Texture::CopyInto(Texture& dest)
{
    assert(dest.width == width && dest.height == height);
    dest.pixels = pixels;
}

void Texture::SwapChannels(unsigned char first,
                           unsigned char second,
                           unsigned char third,
                           unsigned char fourth)
{
    assert(first < 4);
    assert(second < 4);
    assert(third < 4);
    assert(fourth < 4);

    // Make copy before swapping
    Texture src = Copy();
    auto& sPixels = *src.pixels.get();
    auto& dPixels = *pixels.get();

    for (int i = 0; i < src.width * src.height; i++) {
        dPixels[i * 4 + 0] = sPixels[i * 4 + first];
        dPixels[i * 4 + 1] = sPixels[i * 4 + second];
        dPixels[i * 4 + 2] = sPixels[i * 4 + third];
        dPixels[i * 4 + 3] = sPixels[i * 4 + fourth];
    }
}
