#include <spdlog/spdlog.h>
#include <vector>

#include "basalt_colors.hpp"
#include "basalt_images.hpp"

using namespace std;

Image::Image(int width, int height)
{
    if (width <= 0 || height <= 0) {
        spdlog::critical("Canvas width and height must be greater than 0!");
        return;
    }

    this->width = width;
    this->height = height;
    this->surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA8888);
}

Image::Image(const Image& other)
{
    this->width = other.width;
    this->height = other.height;
    this->surface = SDL_DuplicateSurface(other.surface);
}

Image::~Image()
{
    assert(surface);
    SDL_DestroySurface(surface);
    surface = nullptr;
}

void Image::SetPixels(vector<Color>& pixels)
{
    assert(pixels.size() == width * height);
    SDL_LockSurface(surface);
    memcpy(surface->pixels, pixels.data(), pixels.size() * sizeof(Color));
    SDL_UnlockSurface(surface);
}

const Color* Image::GetPixels() const
{
    return (Color*)this->surface->pixels;
}

void Image::Clear(Color color)
{
    DrawRectangle(0, 0, width, height, color);
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
    if (srcX == -1) {
        srcX = 0;
    }
    if (srcY == -1) {
        srcY = 0;
    }
    if (srcWidth == -1) {
        srcWidth = texture.width;
    }
    if (srcHeight == -1) {
        srcHeight = texture.height;
    }

    SDL_Rect srcRect = { srcX, srcY, srcWidth, srcHeight };
    SDL_Rect destRect = { posX, posY, srcWidth, srcHeight };
    SDL_BlitSurface(texture.surface, &srcRect, surface, &destRect);
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
                       int srcX,
                       int srcY,
                       int srcWidth,
                       int srcHeight,
                       Color tint)
{
    SDL_Rect destRect = { destX, destY, destWidth, destHeight };
    SDL_BlitSurfaceScaled(texture.surface, nullptr, surface, &destRect);
}

void Image::BlitScaled(Image texture, Rect dest, Rect src, Color tint)
{
    BlitScaled(texture,
               dest.x,
               dest.y,
               dest.width,
               dest.height,
               src.x,
               src.y,
               src.width,
               src.height,
               tint);
}

void Image::DrawDot(int posX, int posY, Color tint, int thickness)
{
    if (thickness < 0) {
        thickness = 0;
        return;
    }
    int halfThickness = thickness / 2;

    int x = clamp(posX - halfThickness, 0, width - 1);
    int y = clamp(posY - halfThickness, 0, height - 1);
    int w = clamp(thickness, 0, width - x);
    int h = clamp(thickness, 0, height - y);

    SDL_Rect rect = { x, y, w, h };
    SDL_FillSurfaceRect(surface, &rect, tint);
}

void Image::DrawDot(Point pos, Color tint, int thickness)
{
    DrawDot(pos.x, pos.y, tint, thickness);
}

void Image::DrawLine(int startX, int startY, int endX, int endY, Color tint)
{
    int dx = abs(endX - startX);
    int dy = abs(endY - startY);
    int sx = (startX < endX) ? 1 : -1;
    int sy = (startY < endY) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        DrawDot(startX, startY, tint);
        if (startX == endX && startY == endY) {
            break;
        }
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            startX += sx;
        }
        if (e2 < dx) {
            err += dx;
            startY += sy;
        }
    }
}

void Image::DrawLine(Point start, Point end, Color tint)
{
    DrawLine(start.x, start.y, end.x, end.y, tint);
}

void Image::DrawRectangle(int posX, int posY, int width, int height, Color tint)
{
    SDL_Rect rect = { posX, posY, width, height };
    SDL_FillSurfaceRect(surface, &rect, tint);
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