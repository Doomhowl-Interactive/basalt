#include <spdlog/spdlog.h>
#include <vector>

#include "basalt_colors.hpp"
#include "basalt_images.hpp"

using namespace std;

SDL_WrappedSurface::SDL_WrappedSurface(int width, int height, Uint32 format)
{
    surface = SDL_CreateSurface(width, height, format);
}

SDL_WrappedSurface::~SDL_WrappedSurface()
{
    SDL_DestroySurface(surface);
}

SDL_Surface* SDL_WrappedSurface::get() const
{
    assert(surface);
    assert(surface->pixels);
    return surface;
}

Image::Image(int width, int height)
{
    if (width <= 0 || height <= 0) {
        spdlog::critical("Canvas width and height must be greater than 0!");
        return;
    }

    this->width = width;
    this->height = height;
    this->surface = make_shared<SDL_WrappedSurface>(width, height, SDL_PIXELFORMAT_RGBA8888);
}

Image::Image(int width, int height, std::vector<Color>& colors) : Image(width, height)
{
    SetPixels(colors);
}

Image::Image(const Image& other)
{
    this->width = other.width;
    this->height = other.height;
    this->surface = other.surface;
}

void Image::SetPixels(vector<Color>& pixels) const
{
    assert(pixels.size() == width * height);
    SDL_LockSurface(surface->get());
    memcpy(surface->get()->pixels, pixels.data(), pixels.size() * sizeof(Color));
    SDL_UnlockSurface(surface->get());
}

const Color* Image::GetPixels() const
{
    return (Color*)this->surface->get()->pixels;
}

void Image::Clear(Color color) const
{
    SDL_FillSurfaceRect(surface->get(), nullptr, color);
}

void Image::Blit(const Image& texture,
                 int posX,
                 int posY,
                 Color tint,
                 int srcX,
                 int srcY,
                 int srcWidth,
                 int srcHeight) const
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
    SDL_BlitSurface(texture.surface->get(), &srcRect, surface->get(), &destRect);
}

void Image::Blit(const Image& texture, Point pos, Color tint, Rect src) const
{
    Blit(texture, pos.x, pos.y, tint, src.x, src.y, src.width, src.height);
}

void Image::BlitScaled(const Image& texture,
                       int destX,
                       int destY,
                       int destWidth,
                       int destHeight,
                       int srcX,
                       int srcY,
                       int srcWidth,
                       int srcHeight,
                       Color tint) const
{
    SDL_Rect destRect = { destX, destY, destWidth, destHeight };
    SDL_BlitSurfaceScaled(texture.surface->get(), nullptr, surface->get(), &destRect);
}

void Image::BlitScaled(const Image& texture, Rect dest, Rect src, Color tint)
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

void Image::DrawDot(int posX, int posY, Color tint, int thickness) const
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
    SDL_FillSurfaceRect(surface->get(), &rect, tint);
}

void Image::DrawDot(Point pos, Color tint, int thickness) const
{
    DrawDot(pos.x, pos.y, tint, thickness);
}

void Image::DrawLine(int startX, int startY, int endX, int endY, Color tint) const
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

void Image::DrawRectangle(int posX, int posY, int width, int height, Color tint) const
{
    SDL_Rect rect = { posX, posY, width, height };
    SDL_FillSurfaceRect(surface->get(), &rect, tint);
}

void Image::DrawRectangle(Rect rect, Color tint) const
{
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, tint);
}

void Image::DrawRectangleLines(int posX, int posY, int width, int height, int border, Color color)
    const
{
    DrawRectangle(posX, posY, width, border, color);  // top
    DrawRectangle(posX + width - border, posY, border, height,
                  color);  // right
    DrawRectangle(posX, posY + height - border, width, border,
                  color);                              // bottom
    DrawRectangle(posX, posY, border, height, color);  // left
}

void Image::DrawRectangleLines(Rect rect, int border, Color tint)
{
    DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, border, tint);
}