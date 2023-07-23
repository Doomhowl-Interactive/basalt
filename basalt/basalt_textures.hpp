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
              int srcX = 0,
              int srcY = -1,
              int srcWidth = -1,
              int srcHeight = -1);

    void BlitScaled(Texture texture,
                    int destX,
                    int destY,
                    int destWidth,
                    int destHeight,
                    Color tint = WHITE);

    void BlitSheet(TextureSheet sheet, int frame, Vec2 pos, Color tint);

    void DrawDot(int posX, int posY, Color tint);
    void DrawLine(int startX, int startY, int endX, int endY, Color tint);
    void DrawRectangle(int posX, int posY, int width, int height, Color tint);
    void DrawRectangleLines(int posX, int posY, int width, int height, int border, Color tint);

    void DrawWeirdTestGradient();

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
