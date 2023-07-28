#include <spdlog/spdlog.h>
#include <vector>

#include "basalt_renderer.hpp"
#include "basalt_colors.hpp"
#include "basalt_images.hpp"

using namespace std;

Image::Image(int width, int height)
{
    if (width <= 0 || height <= 0) {
        spdlog::critical("Canvas width and height must be greater than 0!");
        return;
    }
    this->id = GetNextTextureID();
    this->width = width;
    this->height = height;
}

void Image::SetPixels(PixelArray& pixels)
{
    //
}

void Image::Clear(Color color)
{
    DrawRectangle(0, 0, width, height, color);
}

Image Image::Copy()
{
    Image copy(width, height);
    CopyInto(copy);
    return copy;
}

void Image::CopyInto(Image& dest)
{
    assert(dest.width == width && dest.height == height);
}

void Image::Blit(Image texture,
                 int posX,
                 int posY,
                 Color tint,
                 int srcX,
                 int srcY,
                 int srcWidth,
                 int srcHeight)
{
}

void Image::Blit(Image texture, Point pos, Color tint, Rect src)
{
    Blit(texture, pos.x, pos.y, tint, src.x, src.y, src.width, src.height);
}

void Image::BlitScaled(Image texture,
                       int destX,
                       int destY,
                       int destWidth,
                       int destHeight,
                       Color tint)
{
}

void Image::BlitScaled(Image texture, Rect dest, Color tint)
{
    BlitScaled(texture, dest.x, dest.y, dest.width, dest.height, tint);
}

void Image::DrawDot(int posX, int posY, Color tint)
{
}

void Image::DrawDot(Point pos, Color tint)
{
    DrawDot(pos.x, pos.y, tint);
}

void Image::DrawLine(int startX, int startY, int endX, int endY, Color tint)
{
}

void Image::DrawLine(Point start, Point end, Color tint)
{
    DrawLine(start.x, start.y, end.x, end.y, tint);
}

void Image::DrawRectangle(int posX, int posY, int width, int height, Color tint)
{
}

void Image::DrawRectangle(Rect rect, Color tint)
{
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, tint);
}

void Image::DrawRectangleLines(int posX, int posY, int width, int height, int border, Color color)
{
    DrawRectangle(posX, posY, width, border, color);   // top
    DrawRectangle(posX + width - border, posY, border, height,
                  color);                              // right
    DrawRectangle(posX, posY + height - border, width, border,
                  color);                              // bottom
    DrawRectangle(posX, posY, border, height, color);  // left
}

void Image::DrawRectangleLines(Rect rect, int border, Color tint)
{
    DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, border, tint);
}

PixelArray CreatePixelArray(int width, int height)
{
    return make_shared<vector<Color>>(width * height);
}