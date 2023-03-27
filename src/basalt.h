#ifndef BASALT_H
#define BASALT_H

#ifdef WASM
#include "wasm_stdlib.h"
#else 

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

typedef size_t usize;
typedef int32_t int32;
typedef uint32_t uint;
typedef uint8_t uchar;

#endif

#define MAX(X,Y) (X > Y ? X:Y)
#define MIN(X,Y) (X < Y ? X:Y)

#define SWAP(T, a, b) do { T t = a; a = b; b = t; } while (0)
#define SIGN(T, x) ((T)((x) > 0) - (T)((x) < 0))
#define ABS(T, x) (SIGN(T, x)*(x))

extern const uint WIDTH;
extern const uint HEIGHT;
extern const const char* GAME_TITLE;

// NOTE: Blursed macros that will make people mad,
// but it allows easy grepping/search
#define class(X) typedef struct X X; \
                 struct X

#define enumdef(X) typedef enum X X; \
                   enum X

#define MAX(X,Y) (X > Y ? X:Y)
#define MIN(X,Y) (X < Y ? X:Y)

#define func static
#define pubfunc
#define wasmfunc

typedef uint Color;

class(Rect)
{
    int x;
    int y;
    int width;
    int height;
};

class(RectF) {
    float x;
    float y;
    float width;
    float height;
};

class(Point) {
    int x;
    int y;
};

class(Size) {
    int width;
    int height;
};

class(Vec2) {
    float x;
    float y;
};

class(Vec3) {
    float x;
    float y;
    float z;
};

#ifdef WASM
#define INFO(...)
#define WARN(...)
#define ERR(...)
#define FATAL(...)
#else
#define INFO(...)   printf("INFO: "__VA_ARGS__); printf("\n")
#define WARN(...)   printf("WARN: "__VA_ARGS__); printf("\n")
#define ERR(...)    printf("ERROR: "__VA_ARGS__); printf("\n")
#define FATAL(...)  printf("FATAL: "__VA_ARGS__); printf("\n"); exit(1)
#endif

#ifdef BASALT_DEBUG
#define DEBUG(...) printf("DEBUG: "__VA_ARGS__); printf("\n")
pubfunc void UnitTest();
#else
#define DEBUG(...)
#endif

// Utility functions (basalt_utils.c)
pubfunc bool IsLittleEndian();
pubfunc int Clamp(int value, int min, int max);

// Asset handling (basalt_assets.c)
class(Texture) {
    int width;
    int height;
    Color* pixels;
};

// required data for map editor
extern uchar* LIST_TEXTURES[];  // WARN: Terminate with NULL-pointer
extern uint LIST_TAGS[];        // WARN: Terminate with -1

#ifndef BASALT_NO_ASSETS
#define LoadTexture(X) LoadTextureEx(#X,X)
pubfunc Texture LoadTextureEx(const char* name, uchar* pixels);
#endif

// Platform dependent stuff
pubfunc void SetWindowTitle(const char* title);

pubfunc Point GetMousePosition();
pubfunc bool IsMouseDown();
pubfunc bool IsMouseUp();

// Graphics drawing (basalt_graphics.c)
pubfunc void DrawDot(Texture canvas, int posX, int posY, int radius, Color color);
pubfunc void DrawDotV(Texture canvas, Vec2 pos, int radius, Color color);

pubfunc void DrawRectangle(Texture canvas, int posX, int posY, int width, int height, Color color);
pubfunc void DrawRectangleRec(Texture canvas, Rect rect, Color color);
pubfunc void DrawRectangleRecF(Texture canvas, RectF rect, Color color);

pubfunc void DrawRectangleLines(Texture canvas, int posX, int posY, int width, int height, int border, Color color);
pubfunc void DrawRectangleLinesRec(Texture canvas, Rect rect, int border, Color color);
pubfunc void DrawRectangleLinesRecF(Texture canvas, RectF rect, int border, Color color);

pubfunc void DrawWeirdTestGradient(Texture canvas);
pubfunc Texture InitTexture(int width, int height);
pubfunc Texture CopyTexture(Texture source);
pubfunc void CopyTextureInto(Texture dest, Texture source);
pubfunc void DisposeTexture(Texture texture);

// Texture pixels are in ABGR (big endian), use this to convert to correct colors for XImage
// NOTE: What is the correct format you might ask? I have no idea, found out after trial-and-error.
pubfunc void MapTextureToCorrectFormat(Texture texture);

pubfunc void ClearTexture(Texture canvas, Color color);
pubfunc void DrawTexture(Texture canvas, Texture texture, int posX, int posY);
pubfunc void DrawTextureV(Texture canvas, Texture texture, Vec2 pos);
pubfunc void DrawTextureEx(Texture canvas, Texture texture, Vec2 pos, Rect src);
pubfunc void DrawTextureScaled(Texture canvas, Texture texture, Rect region);

pubfunc Color RGB(uchar r, uchar g, uchar b);
pubfunc Color RGBA(uchar r, uchar g, uchar b, uchar a); // NOTE: Format RR GG BB AA

// Main game methods (for example: temple_game.c)
pubfunc void InitializeGame();
pubfunc void DisposeGame();
pubfunc void UpdateAndRenderGame(Texture canvas, float delta);

#endif
