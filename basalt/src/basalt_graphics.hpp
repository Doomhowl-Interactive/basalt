#pragma once
#include <string>
#include "basalt.h"

// Graphics drawing (basalt_graphics.c)
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

Texture InitTexture(int width, int height);
Texture CopyTexture(Texture source);
void CopyTextureInto(Texture dest, Texture source);
void DisposeTexture(Texture texture);

// Texture pixels are in ABGR (big endian), use this to convert to correct colors for XImage
// NOTE: What is the correct format you might ask? I have no idea, found out after trial-and-error.
extern void MapTextureToCorrectFormat(Texture dest, Texture src);
void SwapTextureChannels(Texture dest,
                                Texture src,
                                uchar first,
                                uchar second,
                                uchar third,
                                uchar fourth);

void ClearTexture(Texture canvas, Color tint);
void DrawTextureEx(Texture canvas,
                          Texture texture,
                          int posX,
                          int posY,
                          int srcX,
                          int srcY,
                          int srcWidth,
                          int srcHeight,
                          Color tint);

extern void DrawTexture(Texture canvas, Texture texture, int posX, int posY, Color tint);
void DrawTextureScaled(Texture canvas,
                              Texture texture,
                              int destX,
                              int destY,
                              int destWidth,
                              int destHeight,
                              Color tint);

void PrintASCIILogo(std::string suffix);

// ==== COLOR UTILTIES ====
extern Color MakeRGB(uchar r, uchar g, uchar b, uchar a = 255);
Color MakeRGB(float r, float g, float b, float a = 1.f);

Color BlendColors(Color src, Color dest, uchar t);
extern Color ColorAlpha(Color col, float a);
