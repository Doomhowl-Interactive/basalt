#pragma once
#include <vector>
#include <memory>

#include "basalt_colors.hpp"
#include "basalt_math.hpp"
#include "basalt_utils.hpp"
#include "basalt_images_fonts.hpp"
#include "sdl2_basalt.hpp"

struct TextureSheet;
struct Image {
    size_t id;
    int width;
    int height;
    SDL_Surface* surface;

    Image(int width, int height);
    Image(const Image& other);
    ~Image();

    void SetPixels(std::vector<Color>& pixels);
    const Color* GetPixels() const;

    void Clear(Color tint);
    void Blit(Image texture,
              int posX,
              int posY,
              Color tint = WHITE,
              int srcX = -1,
              int srcY = -1,
              int srcWidth = -1,
              int srcHeight = -1);
    void Blit(Image texture, Point pos, Color tint = WHITE, Rect src = { -1, -1, -1, -1 });

    void BlitScaled(Image texture,
                    int destX,
                    int destY,
                    int destWidth,
                    int destHeight,
                    int srcX = -1,
                    int srcY = -1,
                    int srcWidth = -1,
                    int srcHeight = -1,
                    Color tint = WHITE);
    void BlitScaled(Image texture, Rect dest, Rect src = { -1, -1, -1, -1 }, Color tint = WHITE);

    void BlitSheet(TextureSheet sheet, int frame, Vec2 pos, Color tint);

    // Expensive, do not use for rendering an entire image every frame.
    void DrawDot(int posX, int posY, Color tint, int thickness = 1);
    void DrawDot(Point pos, Color tint, int thickness = 1);

    void DrawLine(int startX, int startY, int endX, int endY, Color tint);
    void DrawLine(Point start, Point end, Color tint);

    void DrawRectangle(int posX, int posY, int width, int height, Color tint);
    void DrawRectangle(Rect rect, Color tint);

    void DrawRectangleLines(int posX, int posY, int width, int height, int border, Color tint);
    void DrawRectangleLines(Rect rect, int border, Color tint);

    // DrawText is already taken by windows.h
    // @deprecated Use overloaded method with TextStyle instead.
    void DrawBasaltText(std::string text,
                        int posX,
                        int posY,
                        Color color,
                        Font font = Font::Default(),
                        int size = 32,
                        int maxWidth = INT16_MAX,
                        bool centered = false);

    // DrawText is already taken by windows.h
    void DrawBasaltText(std::string text, int posX, int posY, FontStyle style = {});

    // DrawText is already taken by windows.h
    void DrawBasaltText(std::string text,
                        int posX,
                        int posY,
                        Font font = Font::Default(),
                        FontStyle style = {});

    // Draw text with a shadow
    void DrawBasaltTextShadow(std::string text,
                              int posX,
                              int posY,
                              Font font = Font::Default(),
                              FontStyle foreStyle = {},
                              FontStyle backStyle = {},
                              int spacingX = 2,
                              int spacingY = 2);
};

enum PlaybackMode {
    PLAY_LOOP,
    PLAY_PING_PONG,
    PLAY_ONCE,
};

struct TextureSheet {
    std::string name;                // description
    std::shared_ptr<Image> texture;  // attached texture
    Vec2 origin;                     // top left of sector
    Vec2 cellSize;                   // size of each cell
    size_t count;                    // number of cells in anim
    PlaybackMode mode;
    float fps;
};
