#pragma once

#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifdef WASM
#include "wasm_stdlib.h"
#else 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

typedef unsigned int uint;
typedef unsigned char uchar;
typedef size_t usize;

#ifdef _WIN32
typedef uint64_t ulong;
#endif

typedef uint32_t Color;
typedef uchar Key;

#endif

#define MAX_PATH_LENGTH 128

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
#define platfunc
#define wasmfunc

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

class(String) {
    size_t size;
    size_t capacity;
    char* text;
};

class(StringArray) {
    char** strings;
    usize count;
    usize capacity;
};
typedef StringArray FilePathList;

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

#define DEBUG(...) printf("DEBUG: "__VA_ARGS__); printf("\n")

// Utility functions (basalt_utils.c)
#define DEG2RAD(Y) ((Y) * PI / 180)
#define RAD2DEG(X) ((X) * 180.0 / PI)

pubfunc int Clamp(int value, int min, int max);
pubfunc int GetRandomNumber(); // WARN: read implementation 
pubfunc int GetRealRandomNumber();

pubfunc Vec2 Vec2Normalize(Vec2 v2);
pubfunc float Vec2Magnitude(Vec2 v2);

pubfunc StringArray InitStringArray();
pubfunc void StoreString(StringArray* arr, char* text);
pubfunc void DisposeStringArray(StringArray* arr);

pubfunc String MakeString();
pubfunc void UnloadString(String* str);
pubfunc String* AppendString(String* str, const char* add);
pubfunc void ToUppercase(char* str);
pubfunc const char* PadStringRight(const char* text, char symbol, usize length);

pubfunc bool FileHasExtension(const char* name, const char* ext);
pubfunc bool FolderExists(const char* folder);

// WARN: The following 2 functions use cached memory.
// The result gets overwritten on future calls.
pubfunc ulong GetFileModifiedTime(const char* filePath); // NOTE: Get seconds since epoch a file was last changed.
                                                         // Returns 0 if file doesn't exists.
pubfunc const char* GetFileName(const char* filePath);
pubfunc const char* GetFileStem(const char* filePath);
pubfunc const char* GetFirstExistingFolder(const char** folders); // NOTE: Returns NULL when none exist,
                                                                  // pass NULL ended array
pubfunc FilePathList GetFolderFiles(const char* folder, const char* ext);
pubfunc void UnloadFilePathList(FilePathList list);

// Asset handling (basalt_assets.c)
class(Texture) {
    const char* name;
    int width;
    int height;
    Color* pixels;
};

// required data for map editor
extern uchar* LIST_TEXTURES[];  // WARN: Terminate with NULL-pointer
extern uint LIST_TAGS[];        // WARN: Terminate with -1

#define LoadTexture(X) LoadTextureEx(#X,X)
pubfunc Texture LoadTextureEx(const char* name, uchar* pixels);

// Platform dependent stuff
pubfunc Point GetMousePosition();
pubfunc void SetWindowTitle(const char* title);

pubfunc usize GetFrameIndex();   // NOTE: Get number of frames elapsed since the start of the simulation.
pubfunc double GetTimeElapsed(); // NOTE: Get time elapsed since the start of the simulation.

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
    bool hasUnitTesting;
    bool hasConsole;
    bool unlockedFramerate; 
};

extern EngineConfig Config;

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

pubfunc extern Color RGB(uchar r, uchar g, uchar b);
pubfunc extern Color RGBA(uchar r, uchar g, uchar b, uchar a); // NOTE: Format RR GG BB AA
pubfunc extern Color BlendColors(Color src, Color dest, uchar t);

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
