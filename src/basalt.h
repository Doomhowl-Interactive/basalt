#ifndef BASALT_H
#define BASALT_H

#include "basalt_stdlib.h"

extern const uint WIDTH;
extern const uint HEIGHT;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} Rect;

typedef struct {
    float x;
    float y;
    float width;
    float height;
} RectF;

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int width;
    int height;
} Size;

typedef struct {
    float x;
    float y;
} Vec2;

typedef struct {
    float x;
    float y;
    float z;
} Vec3;

#ifdef WASM
#define INFO(...)
#define WARN(...)
#define ERR(...)
#define FATAL(...)
#else
#define INFO(...) PrintLn("INFO: "__VA_ARGS__);
#define WARN(...) PrintLn("WARN: "__VA_ARGS__);
#define ERR(...) PrintEln("ERROR: "__VA_ARGS__);
#define FATAL(...) PrintEln("FATAL: "__VA_ARGS__); Exit(1);
#endif

#ifdef BASALT_DEBUG
#define DEBUG(...) PrintEln("DEBUG: "__VA_ARGS__);
void UnitTest();
#else
#define DEBUG(...)
#endif

// Utility functions (basalt_utils.c)
bool IsLittleEndian();

// Asset handling (basalt_assets.c)
typedef struct {
    int width;
    int height;
    uint* pixels;
} Texture;

// core assets
extern unsigned char SPR_RGBA[];
extern unsigned char SPR_PLAYER_FOX[];
extern unsigned char SPR_BLOCK[];
extern unsigned char TILE_BLOCK_SMALL[];

#ifndef BASALT_NO_ASSETS

Texture LoadTexture(unsigned char* data);

#endif

// Input handling (platform dependent)
Point GetMousePosition();

// Graphics drawing (basalt_graphics.c)
void DrawDot(Texture canvas, int posX, int posY, int radius, int color);
void DrawDotV(Texture canvas, Vec2 pos, int radius, int color);

void DrawRectangle(Texture canvas, int posX, int posY, int width, int height, int color);
void DrawRectangleRec(Texture canvas, Rect rect, int color);
void DrawRectangleRecF(Texture canvas, RectF rect, int color);

void DrawRectangleLines(Texture canvas, int posX, int posY, int width, int height, int border, int color);
void DrawRectangleLinesRec(Texture canvas, Rect rect, int border, int color);
void DrawRectangleLinesRecF(Texture canvas, RectF rect, int border, int color);

void DrawWeirdTestGradient(Texture canvas);

Texture InitTexture(int width, int height);
void DisposeTexture(Texture texture);

void ClearTexture(Texture canvas, int color);
void BlitTexture(Texture canvas, Texture texture, int posX, int posY);
void BlitTextureV(Texture canvas, Texture texture, Vec2 pos);
void BlitTextureEx(Texture canvas, Texture texture, Vec2 pos, Rect src);
void BlitTextureScaled(Texture canvas, Texture texture, Vec2 pos, float scale);

// Main game methods (for example: temple_game.c)
void InitializeGame();
void DisposeGame();
void UpdateAndRenderGame(Texture canvas, float delta);

#endif
