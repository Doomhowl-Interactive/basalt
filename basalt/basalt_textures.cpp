#include <spdlog/spdlog.h>
#include <vector>

#include "basalt_renderer.hpp"
#include "basalt_colors.hpp"
#include "basalt_textures.hpp"

using namespace std;

Texture::Texture(int width, int height)
{
    if (width <= 0 || height <= 0) {
        spdlog::critical("Canvas width and height must be greater than 0!");
        return;
    }
    this->id = GetNextTextureID();
    this->width = width;
    this->height = height;
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
}

void Texture::Blit(Texture texture,
                   int posX,
                   int posY,
                   Color tint,
                   int srcX,
                   int srcY,
                   int srcWidth,
                   int srcHeight)
{
}

void Texture::Blit(Texture texture, Point pos, Color tint, Rect src)
{
    Blit(texture, pos.x, pos.y, tint, src.x, src.y, src.width, src.height);
}

void Texture::BlitScaled(Texture texture,
                         int destX,
                         int destY,
                         int destWidth,
                         int destHeight,
                         Color tint)
{
}

void Texture::BlitScaled(Texture texture, Rect dest, Color tint)
{
    BlitScaled(texture, dest.x, dest.y, dest.width, dest.height, tint);
}

void Texture::DrawDot(int posX, int posY, Color tint)
{
}

void Texture::DrawDot(Point pos, Color tint)
{
    DrawDot(pos.x, pos.y, tint);
}

void Texture::DrawLine(int startX, int startY, int endX, int endY, Color tint)
{
}

void Texture::DrawLine(Point start, Point end, Color tint)
{
    DrawLine(start.x, start.y, end.x, end.y, tint);
}

void Texture::DrawRectangle(int posX, int posY, int width, int height, Color tint)
{
}

void Texture::DrawRectangle(Rect rect, Color tint)
{
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, tint);
}

void Texture::DrawRectangleLines(int posX, int posY, int width, int height, int border, Color color)
{
    DrawRectangle(posX, posY, width, border, color);   // top
    DrawRectangle(posX + width - border, posY, border, height,
                  color);                              // right
    DrawRectangle(posX, posY + height - border, width, border,
                  color);                              // bottom
    DrawRectangle(posX, posY, border, height, color);  // left
}

void Texture::DrawRectangleLines(Rect rect, int border, Color tint)
{
    DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, border, tint);
}