#ifndef BASALT_H
#define BASALT_H

// TODO: ditch custom stdlib as I don't want to deal with it's #ifdef mess anymore
#include "basalt_stdlib.h"

#include <assert.h>

extern const uint WIDTH;
extern const uint HEIGHT;

// NOTE: Blursed macros that will make people mad,
// but it allows easy grepping/search
#define class(X) typedef struct X X; \
                 struct X

#define enumdef(X) typedef enum X X; \
                 enum X

#define func static
#define pubfunc
#define wasmfunc

#include <stdint.h>
typedef uint32_t Color;

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
#define INFO(...) PrintLn("INFO: "__VA_ARGS__);
#define WARN(...) PrintLn("WARN: "__VA_ARGS__);
#define ERR(...) PrintEln("ERROR: "__VA_ARGS__);
#define FATAL(...) PrintEln("FATAL: "__VA_ARGS__); Exit(1);
#endif

#ifdef BASALT_DEBUG
#define DEBUG(...) PrintEln("DEBUG: "__VA_ARGS__);
pubfunc void UnitTest();
#else
#define DEBUG(...)
#endif

// Utility functions (basalt_utils.c)
pubfunc bool IsLittleEndian();

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
pubfunc Texture LoadTexture(unsigned char* data);
#endif

// Input handling (platform dependent)
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
pubfunc void DisposeTexture(Texture texture);

pubfunc void ClearTexture(Texture canvas, Color color);
pubfunc void DrawTexture(Texture canvas, Texture texture, int posX, int posY);
pubfunc void DrawTextureV(Texture canvas, Texture texture, Vec2 pos);
pubfunc void DrawTextureEx(Texture canvas, Texture texture, Vec2 pos, Rect src);
pubfunc void DrawTextureScaled(Texture canvas, Texture texture, Vec2 pos, float scale);

pubfunc Color CreateColor(uchar r, uchar g, uchar b);
pubfunc Color CreateColorA(uchar r, uchar g, uchar b, uchar a); // NOTE: Format AA RR GG BB

// Main game methods (for example: temple_game.c)
pubfunc void InitializeGame();
pubfunc void DisposeGame();
pubfunc void UpdateAndRenderGame(Texture canvas, float delta);

#endif
