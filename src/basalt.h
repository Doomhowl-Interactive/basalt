#pragma once

#ifdef WASM
#include "wasm_stdlib.h"
#else 

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "string.h"

typedef size_t usize;
typedef int32_t int32;
typedef uint32_t uint;
typedef uint8_t uchar;
typedef uchar Key;

#endif

#define MAX(X,Y) (X > Y ? X:Y)
#define MIN(X,Y) (X < Y ? X:Y)

#define SWAP(T, a, b) do { T t = a; a = b; b = t; } while (0)
#define SIGN(T, x) ((T)((x) > 0) - (T)((x) < 0))
#define ABS(T, x) (SIGN(T, x)*(x))

extern const uint WIDTH;
extern const uint HEIGHT;
extern const char* GAME_TITLE;

extern const uint TPS;
extern const uint MAX_ENTITIES;

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

class(StringArray) {
    char** strings;
    usize count;
    usize capacity;
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
#define DEG2RAD(Y) ((Y) * M_PI / 180)
#define RAD2DEG(X) ((X) * 180.0 / M_PI)

pubfunc bool IsLittleEndian();
pubfunc int Clamp(int value, int min, int max);
pubfunc int GetRandomNumber(); // WARN: read implementation 
pubfunc int GetRealRandomNumber();

pubfunc Vec2 Vec2Normalize(Vec2 v2);
pubfunc float Vec2Magnitude(Vec2 v2);

pubfunc StringArray InitStrings();
pubfunc void AppendString(StringArray* arr, char* text);
pubfunc void DisposeStrings(StringArray* arr);

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
pubfunc Point GetMousePosition();
pubfunc void SetWindowTitle(const char* title);

pubfunc usize GetFrameIndex();

pubfunc bool IsMouseDown();
pubfunc bool IsMouseUp();

pubfunc bool IsKeyDown(Key code);
pubfunc bool IsKeyUp(Key code);

pubfunc bool IsKeyPressed(Key code);
pubfunc bool IsKeyReleased(Key code);

// Engine configuration (basalt_config.c)
class(EngineConfig) {
    bool hasArchaeo;
    bool hasHotloading;
    bool unlockedFramerate; 
};

extern EngineConfig Config;

pubfunc char* ListEnabledFeatures(int* count);
pubfunc bool ParseLaunchArguments(int argc, char** argv); // NOTE: Returns true if engine should continue running

// Tooling stuff 
// (basalt_tooling.h)
pubfunc bool UpdateAndRenderArchaeo();
#define DRAWCALL(X,Y) DrawCallImpl(X,#Y)
pubfunc void DrawCallImpl(Texture canvas, const char* desc);

// Graphics drawing (basalt_graphics.c)
pubfunc void DrawDot(Texture canvas, int posX, int posY, Color color);
pubfunc void DrawDotV(Texture canvas, Vec2 pos, Color color);

pubfunc void DrawLine(Texture canvas, int startX, int startY, int endX, int endY, Color color);
pubfunc void DrawLineV(Texture canvas, Vec2 start, Vec2 end, Color color);

pubfunc void DrawRectangle(Texture canvas, int posX, int posY, int width, int height, Color color);
pubfunc void DrawRectangleRec(Texture canvas, Rect rect, Color color);

pubfunc void DrawRectangleLines(Texture canvas, int posX, int posY, int width, int height, int border, Color color);
pubfunc void DrawRectangleLinesRec(Texture canvas, Rect rect, int border, Color color);

pubfunc void DrawWeirdTestGradient(Texture canvas);

pubfunc Texture InitTexture(int width, int height);
pubfunc Texture CopyTexture(Texture source);
pubfunc void CopyTextureInto(Texture dest, Texture source);
pubfunc void DisposeTexture(Texture texture);

// Texture pixels are in ABGR (big endian), use this to convert to correct colors for XImage
// NOTE: What is the correct format you might ask? I have no idea, found out after trial-and-error.
pubfunc void MapTextureToCorrectFormat(Texture dest, Texture src);

pubfunc void ClearTexture(Texture canvas, Color color);
pubfunc void DrawTexture(Texture canvas, Texture texture, int posX, int posY);
pubfunc void DrawTextureV(Texture canvas, Texture texture, Vec2 pos);
pubfunc void DrawTextureEx(Texture canvas, Texture texture, Vec2 pos, Rect src);
pubfunc void DrawTextureScaled(Texture canvas, Texture texture, Rect region);

pubfunc Color RGB(uchar r, uchar g, uchar b);
pubfunc Color RGBA(uchar r, uchar g, uchar b, uchar a); // NOTE: Format RR GG BB AA
pubfunc Color BlendColors(Color color1, Color color2, float percent);

// Main game methods (for example: temple_game.c)
pubfunc void InitializeGame();
pubfunc void DisposeGame();
pubfunc void UpdateAndRenderGame(Texture canvas, float delta);

// Key definitions
#define KEY_A               'A'
#define KEY_B               'B'
#define KEY_C               'C'
#define KEY_D               'D'
#define KEY_E               'E'
#define KEY_F               'F'
#define KEY_G               'G'
#define KEY_H               'H'
#define KEY_I               'I'
#define KEY_J               'J'
#define KEY_K               'K'
#define KEY_L               'L'
#define KEY_M               'M'
#define KEY_N               'N'
#define KEY_O               'O'
#define KEY_P               'P'
#define KEY_Q               'Q'
#define KEY_R               'R'
#define KEY_S               'S'
#define KEY_T               'T'
#define KEY_U               'U'
#define KEY_V               'V'
#define KEY_W               'W'
#define KEY_X               'X'
#define KEY_Y               'Y'
#define KEY_Z               'Z'

// FIXME: This won't work on AZERTY keyboards!
#define KEY_0               '0'
#define KEY_1               '1'
#define KEY_2               '2'
#define KEY_3               '3'
#define KEY_4               '4'
#define KEY_5               '5'
#define KEY_6               '6'
#define KEY_7               '7'
#define KEY_8               '8'
#define KEY_9               '9'
