#ifndef BASALT_H
#define BASALT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define WIDTH 1280
#define HEIGHT 720

typedef unsigned int uint;
typedef unsigned char uchar;

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

// Graphics drawing (basalt_graphics.c)
typedef Texture Canvas;

void RenderSprite(Canvas canvas, Texture texture, int posX, int posY);
void RenderSpriteV(Canvas canvas, Texture texture, Vec2 pos);
void RenderWeirdTestGradient(Canvas canvas);

#endif
