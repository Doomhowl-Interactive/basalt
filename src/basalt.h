#ifndef BASALT_H
#define BASALT_H

#ifndef __cplusplus
typedef int bool;
#endif

typedef unsigned int uint;
typedef unsigned char uchar;

#ifndef NULL
#define NULL    0
#endif

#define true    1
#define false   0

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

#ifdef DEBUG
#define Assert(X) AssertImpl(X)
#define Panic(X) PanicImpl(X)
void AssertImpl(bool cond);
void PanicImpl(char* msg);
void UnitTest();
#else
#define Assert(X)
#define Panic(X)
#endif

bool IsLittleEndian();

// Asset handling (basalt_assets.c)
typedef struct {
    int width;
    int height;
    uint* pixels;
} Texture;

#ifndef BASALT_NO_ASSETS

Texture LoadTexture(unsigned char* data);

#endif

// Graphics drawing (basalt_graphics.c)
typedef Texture Canvas;

void RenderSprite(Canvas canvas, Texture texture, int posX, int posY);
void RenderSpriteV(Canvas canvas, Texture texture, Vec2 pos);
void RenderWeirdTestGradient(Canvas canvas);

#endif
