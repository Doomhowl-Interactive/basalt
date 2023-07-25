#pragma once
#include <vector>

#include "basalt_colors.hpp"
#include "basalt_math.hpp"
#include "basalt_utils.hpp"
#include "basalt_graphics_fonts.hpp"

struct TextureSheet;
struct Texture {
    unsigned int width;
    unsigned int height;
    std::string name;
    std::shared_ptr<std::vector<Color>> pixels;

    Texture(unsigned int width, unsigned int height);

    static Texture GenerateNoise(unsigned int width,
                                 unsigned int height,
                                 Color bg,
                                 Color fg,
                                 double scale,
                                 int seed = GetRandomNumber());

    Texture Copy();
    void CopyInto(Texture& dest);

    void SwapChannels(unsigned char first,
                      unsigned char second,
                      unsigned char third,
                      unsigned char fourth);

    // Texture pixels are in ABGR (big endian), use this to convert to correct colors for XImage
    // NOTE: What is the correct format you might ask? I have no idea, found out after
    // trial-and-error.
    inline void MapToCorrectFormat()
    {
        SwapChannels(1, 2, 3, 0);
    }

    void Clear(Color tint);
    void Blit(Texture texture,
              int posX,
              int posY,
              Color tint = WHITE,
              int srcX = -1,
              int srcY = -1,
              int srcWidth = -1,
              int srcHeight = -1);
    inline void Blit(Texture texture, Point pos, Color tint = WHITE, Rect src = { -1, -1, -1, -1 })
    {
        Blit(texture, pos.x, pos.y, tint, src.x, src.y, src.width, src.height);
    }

    void BlitScaled(Texture texture,
                    int destX,
                    int destY,
                    int destWidth,
                    int destHeight,
                    Color tint = WHITE);
    inline void BlitScaled(Texture texture, Rect dest, Color tint = WHITE)
    {
        BlitScaled(texture, dest.x, dest.y, dest.width, dest.height, tint);
    }

    void BlitSheet(TextureSheet sheet, int frame, Vec2 pos, Color tint);

    void DrawDot(int posX, int posY, Color tint);
    inline void DrawDot(Point pos, Color tint)
    {
        DrawDot(pos.x, pos.y, tint);
    }

    void DrawLine(int startX, int startY, int endX, int endY, Color tint);
    inline void DrawLine(Point start, Point end, Color tint)
    {
        DrawLine(start.x, start.y, end.x, end.y, tint);
    }

    void DrawRectangle(int posX, int posY, int width, int height, Color tint);
    inline void DrawRectangle(Rect rect, Color tint)
    {
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, tint);
    }

    void DrawRectangleLines(int posX, int posY, int width, int height, int border, Color tint);
    inline void DrawRectangleLines(Rect rect, int border, Color tint)
    {
        DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, border, tint);
    }

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
    std::string name;  // description
    Texture texture;   // attached texture
    Vec2 origin;       // top left of sector
    Vec2 cellSize;     // size of each cell
    size_t count;      // number of cells in anim
    PlaybackMode mode;
    float fps;
};
