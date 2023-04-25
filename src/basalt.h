#pragma once

#ifndef PI
# define PI 3.14159265358979323846
#endif

#ifdef WASM
# include "wasm_stdlib.h"
#else

# include <assert.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef unsigned int uint;
typedef unsigned char uchar;
typedef size_t usize;

# ifdef _WIN32
typedef uint64_t ulong;
# endif

typedef uint32_t Color;
typedef uchar Key;

#endif

#define MAX_ENTITIES 100000
#define MAX_PATH_LENGTH 128
#define DYNAMIC

#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

#define SWAP(T, a, b) \
 do { \
  T t = a; \
  a = b; \
  b = t; \
 } while (0)
#define SIGN(T, x) ((T)((x) > 0) - (T)((x) < 0))
#define ABS(T, x) (SIGN(T, x) * (x))

extern const uint WIDTH;
extern const uint HEIGHT;
extern const char* GAME_TITLE;

extern const uint TPS;

#define func static
#define BASALT

typedef struct Rect {
    int x;
    int y;
    int width;
    int height;
} Rect;

typedef struct RectF {
    float x;
    float y;
    float width;
    float height;
} RectF;

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Size {
    int width;
    int height;
} Size;

typedef struct Vec2 {
    float x;
    float y;
} Vec2;

typedef struct Vec3 {
    float x;
    float y;
    float z;
} Vec3;

typedef struct String {
    size_t size;
    size_t capacity;
    char* text;
} String;

typedef struct StringArray {
    char** strings;
    usize count;
    usize capacity;
} StringArray;

// TODO: Remove typedef
typedef StringArray FilePathList;

#ifdef WASM
# define INFO(...)
# define WARN(...)
# define ERR(...)
# define FATAL(...)
#else
# define INFO(...) \
  printf("INFO: "__VA_ARGS__); \
  printf("\n")
# define WARN(...) \
  printf("WARN: "__VA_ARGS__); \
  printf("\n")
# define ERR(...) \
  printf("ERROR: "__VA_ARGS__); \
  printf("\n")
# define FATAL(...) \
  printf("FATAL: "__VA_ARGS__); \
  printf("\n"); \
  exit(1)
#endif

#define DEBUG(...) \
 printf("DEBUG: "__VA_ARGS__); \
 printf("\n")

// Ergonomic macros
#define V2(V) V.x, V.y
#define _V2(V) V->x, V->y
#define P2(P) P.x, P.y
#define _P2(P) P->x, P->y
#define R2(R) R.x, R.y, R.width, R.height
#define _R2(R) R.x, R.y, R.width, R.height

// Utility functions (basalt_utils.c)
#define DEG2RAD(Y) ((Y)*PI / 180)
#define RAD2DEG(X) ((X)*180.0 / PI)

BASALT int Clamp(int value, int min, int max);

// WARN: read implementation
BASALT int GetRandomNumber();
BASALT int GetRealRandomNumber();

BASALT extern Vec2 RectFOrigin(RectF rectf);
#define RectFOriginPoint(F) Vec2ToPoint(RectFOrigin(F))
BASALT extern Vec2 RectFCenter(RectF rectf);
#define RectFCenterPoint(F) Vec2ToPoint(RectFCenter(F))

BASALT extern Point RectOrigin(Rect rect);
#define RectOriginVec2(R) PointToVec2(RectOrigin(R))
BASALT extern Point RectCenter(Rect rect);
#define RectCenterVec2(R) PointToVec2(RectCenter(R))

BASALT extern Rect RectFToRect(RectF rectf);
BASALT extern RectF RectToRectF(Rect rect);

BASALT extern Point Vec2ToPoint(Vec2 v2);
BASALT extern Vec2 PointToVec2(Point point);

BASALT extern Vec2 Vec2Add(Vec2 src, Vec2 offset);
BASALT extern Vec2 Vec2Subtract(Vec2 src, Vec2 offset);
BASALT extern Vec2 Vec2Scale(Vec2 src, float scale);
BASALT extern Vec2 Vec2Normalize(Vec2 v2);
BASALT extern Vec2 Vec2Negate(Vec2 v2);
BASALT extern float Vec2Magnitude(Vec2 v2);

// NOTE: Get's normalized direction from src to dest
BASALT extern Vec2 Vec2Towards(Vec2 src, Vec2 dest);

BASALT extern float Vec2DistanceSquared(Vec2 first, Vec2 second);

// WARN: Taking square roots is expensive! Use Vec2DistanceSquared instead!
BASALT extern float Vec2Distance(Vec2 first, Vec2 second);

BASALT StringArray InitStringArray();
BASALT void StoreString(StringArray* arr, char* text);
BASALT void DisposeStringArray(StringArray* arr);

BASALT String MakeString();
BASALT void UnloadString(String* str);
BASALT String* AppendString(String* str, const char* add);
BASALT void ToUppercase(char* str);
BASALT const char* PadStringRight(const char* text, char symbol, usize length);

BASALT bool FileHasExtension(const char* name, const char* ext);
BASALT bool FolderExists(const char* folder);

// WARN: Uses cached memory, result gets overwritten on future calls.
// NOTE: Get seconds since epoch a file was last changed.
// Returns 0 if file doesn't exists.
BASALT long GetFileModifiedTime(const char* filePath);
BASALT const char* GetFileName(const char* filePath);
BASALT const char* GetFileStem(const char* filePath);

// NOTE: Returns NULL when none exist,
// pass NULL ended array
BASALT const char* GetFirstExistingFolder(const char** folders);

BASALT FilePathList GetFolderFiles(const char* folder, const char* ext);
BASALT void UnloadFilePathList(FilePathList list);

// Asset handling (basalt_assets.c)
typedef struct Texture {
    const char* name;
    int width;
    int height;
    Color* pixels;
} Texture;
extern const uchar SPR_PIXELFONT[];

// TODO: Change to retreive texture, that uses cache system
#define RequestTexture(X) RequestTextureEx(#X, X)
BASALT Texture RequestTextureEx(const char* name, const uchar* pixels);

// NOTE: Returns null ended array of loaded/cached textures
BASALT Texture* GetLoadedTextures();
BASALT void TakeScreenshot(Texture canvas);

BASALT void TakeScreenshot();
// Platform dependent stuff
BASALT Point GetMousePosition();
BASALT void SetWindowTitle(const char* title);

// NOTE: Get number of frames elapsed since the start of the simulation.
BASALT usize GetFrameIndex();

// NOTE: Get time elapsed since the start of the simulation.
BASALT double GetTimeElapsed();

BASALT bool IsMouseDown();
BASALT bool IsMouseUp();

BASALT bool IsKeyDown(Key code);
BASALT bool IsKeyUp(Key code);

BASALT bool IsKeyPressed(Key code);
BASALT bool IsKeyReleased(Key code);

// Engine configuration (basalt_config.c)
typedef struct EngineConfig {
    bool hasArchaeo;
    bool hasHotloading;
    bool hasUnitTesting;
    bool hasConsole;
    bool isHeadless;
    bool unlockedFramerate;
} EngineConfig;
extern EngineConfig Config;

// Graphics drawing (basalt_graphics.c)
#define WHITE 0xFFFFFFFF
#define GRAY 0xAAAAAAAA
#define BLACK 0x000000FF
#define BLANK 0xFFFFFF00

#define RED 0xFF0000FF
#define GREEN 0x00FF00FF
#define BLUE 0x0000FFFF
#define YELLOW 0xFFFF00FF
#define PURPLE 0x00FFFFFF

typedef struct BitmapFont {
    Texture texture;
    uint cols;
    uint rows;
    uint cellWidth;
    uint cellHeight;
    const char* symbols;
} BitmapFont;

BASALT void DrawDot(Texture canvas, int posX, int posY, Color tint);
BASALT void DrawLine(Texture canvas, int startX, int startY, int endX, int endY, Color tint);
BASALT void DrawRectangle(Texture canvas, int posX, int posY, int width, int height, Color tint);
BASALT void DrawRectangleLines(Texture canvas, int posX, int posY, int width, int height, int border, Color tint);
BASALT void DrawWeirdTestGradient(Texture canvas);

BASALT void DrawBitmapFontSymbol(BitmapFont font, int posX, int posY, char symbol, Color color);
BASALT void DrawText(Texture canvas, const char* text, int posX, int posY, Color color);
BASALT void DrawBitmapText(BitmapFont font, Texture canvas, const char* text, int posX, int posY, Color color);

BASALT Texture InitTexture(int width, int height);
BASALT Texture CopyTexture(Texture source);
BASALT void CopyTextureInto(Texture dest, Texture source);
BASALT void DisposeTexture(Texture texture);

// Texture pixels are in ABGR (big endian), use this to convert to correct colors for XImage
// NOTE: What is the correct format you might ask? I have no idea, found out after trial-and-error.
BASALT extern void MapTextureToCorrectFormat(Texture dest, Texture src);
BASALT void SwapTextureChannels(Texture dest, Texture src, uchar first, uchar second, uchar third, uchar fourth);

BASALT void ClearTexture(Texture canvas, Color tint);
BASALT void DrawTextureEx(Texture canvas, Texture texture, int posX, int posY, int srcX, int srcY, int srcWidth, int srcHeight, Color tint);

BASALT extern void DrawTexture(Texture canvas, Texture texture, int posX, int posY, Color tint);
BASALT void DrawTextureScaled(Texture canvas, Texture texture, int destX, int destY, int destWidth, int destHeight, Color tint);

BASALT extern Color RGB(uchar r, uchar g, uchar b);
BASALT extern Color RGBA(uchar r, uchar g, uchar b, uchar a);
BASALT extern Color BlendColors(Color src, Color dest, uchar t);

// Main game methods (for example: temple_game.c)
DYNAMIC BASALT void InitializeGame();
DYNAMIC BASALT void DisposeGame();
DYNAMIC BASALT void UpdateAndRenderGame(Texture canvas, float delta);

// Key definitions
// TODO: Find a way to translate other keyboard layouts
#define KEY_A 'A'
#define KEY_B 'B'
#define KEY_C 'C'
#define KEY_D 'D'
#define KEY_E 'E'
#define KEY_F 'F'
#define KEY_G 'G'
#define KEY_H 'H'
#define KEY_I 'I'
#define KEY_J 'J'
#define KEY_K 'K'
#define KEY_L 'L'
#define KEY_M 'M'
#define KEY_N 'N'
#define KEY_O 'O'
#define KEY_P 'P'
#define KEY_Q 'Q'
#define KEY_R 'R'
#define KEY_S 'S'
#define KEY_T 'T'
#define KEY_U 'U'
#define KEY_V 'V'
#define KEY_W 'W'
#define KEY_X 'X'
#define KEY_Y 'Y'
#define KEY_Z 'Z'

// FIXME: This won't work on AZERTY keyboards!
#define KEY_0 '0'
#define KEY_1 '1'
#define KEY_2 '2'
#define KEY_3 '3'
#define KEY_4 '4'
#define KEY_5 '5'
#define KEY_6 '6'
#define KEY_7 '7'
#define KEY_8 '8'
#define KEY_9 '9'
