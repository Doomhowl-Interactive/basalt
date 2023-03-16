#ifndef BASALT_H
#define BASALT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef int32_t int32;
typedef uint32_t uint;
typedef unsigned char uchar;

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

// Utilties (basalt_utils.c)
void* MemAlloc(uint size);
void* MemAllocEx(uint size, uint amount);
void MemFree(void* ptr);

#ifdef __linux__ 
#define LINUX
#elif defined(_WIN32) || defined(_WIN64)
#define WINDOWS
#else
#define UNKNOWN
#endif

#define INFO(...) printf("INFO: "__VA_ARGS__); printf("\n")
#define WARN(...) printf("WARN: "__VA_ARGS__); printf("\n")
#undef ERROR
#define ERROR(...) fprintf(stderr,"ERROR: "__VA_ARGS__); printf("\n")
#define FATAL(...) fprintf(stderr,"FATAL: "__VA_ARGS__); printf("\n"); exit(1);

#ifdef BASALT_DEBUG
#define Assert(X) assert(X)
#define Panic(X) panic(X)
#define DEBUG(...) printf("DEBUG: "__VA_ARGS__); printf("\n")
void UnitTest();
#else
#define Assert(X)
#define Panic(X)
#define DEBUG(...)
#endif

#define MAX(X,Y) (X > Y ? X:Y)
#define MIN(X,Y) (X < Y ? X:Y)

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
void DrawDot(Texture canvas, int posX, int posY, int radius, int32 color);
void DrawDotV(Texture canvas, Vec2 pos, int radius, int32 color);

Texture InitTexture(int width, int height);
void DisposeTexture(Texture texture);

void ClearTexture(Texture canvas, int32 color);
void BlitTexture(Texture canvas, Texture texture, int posX, int posY);
void BlitTextureV(Texture canvas, Texture texture, Vec2 pos);
void BlitTextureEx(Texture canvas, Texture texture, Vec2 pos, Rect src);
void BlitTextureScaled(Texture canvas, Texture texture, Vec2 pos, float scale);
void RenderWeirdTestGradient(Texture canvas);

// Main game methods (for example: temple_game.c)
void InitializeGame();
void DisposeGame();
void UpdateAndRenderGame(Texture canvas, float delta);

#endif
