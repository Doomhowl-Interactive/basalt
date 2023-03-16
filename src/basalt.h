#ifndef BASALT_H
#define BASALT_H

#define HEAP_SIZE 500*1000
#define MAX_ALLOCATIONS 500

#define NULL 0

typedef int int32;
typedef unsigned int uint;
typedef unsigned char uchar;

#define stderr 1

#ifndef __cplusplus

typedef int bool;
#define false 0
#define true 1

#endif

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

// logging functions (basalt_utils.c)
#ifdef WASM
#define INFO(...)
#define WARN(...)
#define ERROR(...)
#define FATAL(...)
#else
#define INFO(...) PrintImpl("INFO: "__VA_ARGS__); PrintImpl("\n")
#define WARN(...) PrintImpl("WARN: "__VA_ARGS__); PrintImpl("\n")
#undef ERROR
#define ERROR(...) PrintImpl(stderr,"ERROR: "__VA_ARGS__); PrintImpl("\n")
#define FATAL(...) PrintImpl(stderr,"FATAL: "__VA_ARGS__); PrintImpl("\n"); exit(1);
#endif

#ifdef BASALT_DEBUG
#define DEBUG(...) printf("DEBUG: "__VA_ARGS__); printf("\n")
void UnitTest();
#else
#define DEBUG(...)
#endif

#define MAX(X,Y) (X > Y ? X:Y)
#define MIN(X,Y) (X < Y ? X:Y)

#define Assert(X) AssertImpl((bool)X)
void AssertImpl(bool cond);

void Panic(const char* msg);
bool IsLittleEndian();

// C standard library reinventing for WASM (basalt_utils.c)
#ifdef WASM
void* malloc(uint size);
void free(void* ptr);
void memcpy(void* dest, const void* src, uint size);
#endif

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

void DrawRectangle(Texture canvas, int posX, int posY, int width, int height, int32 color);
void DrawRectangleRec(Texture canvas, Rect rect, int32 color);
void DrawRectangleRecF(Texture canvas, RectF rect, int32 color);

void DrawRectangleLines(Texture canvas, int posX, int posY, int width, int height, int border, int32 color);
void DrawRectangleLinesRec(Texture canvas, Rect rect, int border, int32 color);
void DrawRectangleLinesRecF(Texture canvas, RectF rect, int border, int32 color);

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
