#pragma once
#include <string>
#include <memory>
#include <vector>

#include "basalt_graphics_fonts.hpp"
#include "basalt_math.hpp"

namespace basalt {

using namespace std;

typedef uint32_t Color;

struct Texture {
    unsigned int width;
    unsigned int height;
    string name;
    shared_ptr<vector<Color>> pixels;

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

    void BlitSheet(TextureSheet sheet, int frame, Vec2 pos, Color tint)
    {
    }

    void DrawDot(int posX, int posY, Color tint);
    void DrawLine(int startX, int startY, int endX, int endY, Color tint);
    void DrawRectangle(int posX, int posY, int width, int height, Color tint);
    void DrawRectangleLines(int posX, int posY, int width, int height, int border, Color tint);

    void DrawWeirdTestGradient();

    void DrawText(string text, int posX, int posY, Color color, Font font = Font::Default());
};

enum PlaybackMode {
    PLAY_LOOP,
    PLAY_PING_PONG,
    PLAY_ONCE,
};

struct TextureSheet {
    string name;      // description
    Texture texture;  // attached texture
    Vec2 origin;      // top left of sector
    Vec2 cellSize;    // size of each cell
    size_t count;     // number of cells in anim
    PlaybackMode mode;
    float fps;
};

extern const unsigned char SPR_PIXELFONT[];

constexpr Color WHITE = 0xFFFFFFFF;
constexpr Color GRAY = 0xAAAAAAFF;
constexpr Color LIGHTGRAY = 0xC8C8C8FF;
constexpr Color DARKGRAY = 0x505050FF;
constexpr Color BLACK = 0x000000FF;
constexpr Color BLANK = 0xFFFFFF00;

constexpr Color RED = 0xFF0000FF;
constexpr Color GREEN = 0x00FF00FF;
constexpr Color DARKGREEN = 0x00752CFF;
constexpr Color BLUE = 0x0000FFFF;
constexpr Color DARKBLUE = 0x00527FFF;
constexpr Color YELLOW = 0xFFFF00FF;
constexpr Color PURPLE = 0x00FFFFFF;
constexpr Color BROWN = 0x7F3F00FF;
constexpr Color DARKBROWN = 0x5A2E00FF;
constexpr Color DARKRED = 0x7F0000FF;

void InitHotReloading();
void HotReloadTexture(Texture texture);

// NOTE: Returns null ended array of loaded/cached textures
Texture* GetLoadedTextures();
void TakeScreenshot(Texture canvas);

// Platform dependent stuff
Point GetMousePosition();
void SetWindowTitle(const char* title);

// NOTE: Get number of frames elapsed since the start of the simulation.
usize GetFrameIndex();

// NOTE: Get time elapsed since the start of the simulation.
double GetTimeElapsed();
extern void DrawDot(Texture canvas, int posX, int posY, Color tint);
void DrawLine(Texture canvas, int startX, int startY, int endX, int endY, Color tint);
void DrawRectangle(Texture canvas, int posX, int posY, int width, int height, Color tint);
void DrawRectangleLines(Texture canvas,
                        int posX,
                        int posY,
                        int width,
                        int height,
                        int border,
                        Color tint);
void DrawWeirdTestGradient(Texture canvas);
extern void LoadFont(const char* name);
void LoadFontEx(const char* name, uint size);

void DrawBasaltText(Texture canvas, const char* text, int posX, int posY, Color color);
void DrawBasaltTextEx(const char* fontName,
                      Texture canvas,
                      const char* text,
                      int posX,
                      int posY,
                      Color color);

// TODO: Put in private header
void DisposeFonts();

// ==== COLOR UTILTIES ====
extern Color MakeRGB(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
extern Color ColorAlpha(Color col, float a);
extern Color MakeRGB(float r, float g, float b, float a = 1.f);
Color BlendColors(Color src, Color dest, unsigned char t);

}  // namespace basalt