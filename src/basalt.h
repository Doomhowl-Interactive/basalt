#pragma once

#ifndef PI
# define PI 3.14159265358979323846
#endif

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "basalt_keys.h"

typedef unsigned int uint;
typedef unsigned char uchar;
typedef size_t usize;

#ifdef _WIN32
typedef uint64_t ulong;
#endif

typedef uint32_t Color;

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

#ifdef __cplusplus
extern "C" {
#endif

#define BASALT

typedef enum ConsoleColor {
    CBLACK,
    CLGRAY,
    CDGRAY,
    CBLUE,
    CGREEN,
    CYAN,
    CRED,
    CPURPLE,
    CYELLOW,
    CWHITE,
    CORANGE,
    CPINK,
} ConsoleColor;

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

// TODO: If I switch to C++, I can finally ditch this.
typedef struct String {
    size_t size;
    size_t capacity;
    char* text;
} String;

// TODO: If I switch to C++, I can finally ditch this.
typedef struct StringArray {
    char** strings;
    usize count;
    usize capacity;
} StringArray;

#ifdef __cplusplus
# define INFO printf
# define WARN printf
# define ERR printf
# define FATAL printf
# define DEBUG printf
#else
# define INFO(...) BasaltPrintColored(CWHITE, "INFO  : "__VA_ARGS__)
# define WARN(...) BasaltPrintColored(CYELLOW, "WARN  : "__VA_ARGS__)
# define ERR(...) BasaltPrintColored(CRED, "ERROR : "__VA_ARGS__)
# define FATAL(...) BasaltPrintColored(CPURPLE, "FATAL : "__VA_ARGS__)
# define DEBUG(...) BasaltPrintColored(CLGRAY, "DEBUG : "__VA_ARGS__)
#endif

// Ergonomic macros
#define V2(V) V.x, V.y
#define _V2(V) V->x, V->y
#define P2(P) P.x, P.y
#define _P2(P) P->x, P->y
#define R2(R) R.x, R.y, R.width, R.height
#define _R2(R) R.x, R.y, R.width, R.height

// Utility functions (basalt_utils.c)

// NOTE: Returns true if engine should continue runnings
BASALT bool ParseLaunchArguments(int argc, char** argv);
BASALT const char* GetWorkingDirectory();

// Cross platform println function
// All text passed to this function will by stored in a buffer, allowing the possibility to render
// console output into your game or easily dump it to a file.
#define BasaltPrint(...) BasaltPrintColored(WHITE, __VA_ARGS__)
BASALT void BasaltPrintColored(ConsoleColor color, const char* format, ...);

BASALT String GetBasaltLog();

#define DEG2RAD(Y) ((Y)*PI / 180)
#define RAD2DEG(X) ((X)*180.0 / PI)
BASALT int Clamp(int value, int min, int max);
BASALT bool IsLittleEndian();

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
BASALT extern bool RectFOverlaps(RectF first, RectF second);

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

// WARN: Cached memory, copy for long usage!
BASALT const char* FormatText(const char* text, ...);
BASALT extern bool TextIsEqual(const char* text1, const char* text2);
BASALT extern bool TextIsEqualNoCase(const char* text1, const char* text2);
BASALT extern const char* AppendText(const char* src, const char* add);
BASALT extern char* CloneText(const char* text);  // WARN: free after use
BASALT extern usize TextLength(const char* text);
BASALT char* StripText(char* buffer);  // strip happens in-place!
BASALT int CopyText(char* dst, const char* src);

// TODO FIX: Inconsistent naming with InitStringArray
BASALT String MakeString();
BASALT void DisposeString(String* str);
BASALT String* AppendString(String* str, const char* add);
BASALT const char* ToUppercase(const char* str);
BASALT const char* ToLowercase(const char* str);
BASALT const char* PadStringRight(const char* text, char symbol, usize length);

BASALT extern bool FileHasExtension(const char* name, const char* ext);
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
BASALT StringArray GetFolderFiles(const char* folder, const char* ext);
BASALT const char* GetWorkingDirectory();

// Asset handling (basalt_assets.c)
typedef struct Texture {
    const char* name;
    int width;
    int height;
    Color* pixels;
} Texture;
extern const uchar SPR_PIXELFONT[];

BASALT Texture RequestTexture(const char* name);
BASALT void InitHotReloading();
BASALT void HotReloadTexture(Texture texture);

// NOTE: Returns null ended array of loaded/cached textures
BASALT Texture* GetLoadedTextures();
BASALT void TakeScreenshot(Texture canvas);

// Platform dependent stuff
BASALT Point GetMousePosition();
BASALT void SetWindowTitle(const char* title);

// NOTE: Get number of frames elapsed since the start of the simulation.
BASALT usize GetFrameIndex();

// NOTE: Get time elapsed since the start of the simulation.
BASALT double GetTimeElapsed();

BASALT bool IsMouseDown();
BASALT bool IsMouseUp();

BASALT bool IsKeyDown(KeyCode code);
BASALT bool IsKeyUp(KeyCode code);

BASALT bool IsKeyPressed(KeyCode code);
BASALT bool IsKeyReleased(KeyCode code);

// Engine configuration (basalt_config.c)
typedef struct EngineConfig {
    bool hasArchaeo;
    bool hasHotloading;
    bool hasUnitTesting;
    bool hasConsole;
    bool useSoftware;
    bool isHeadless;
    bool unlockedFramerate;
    bool lowQuality;
} EngineConfig;
extern EngineConfig Config;

typedef struct GameConfig {
    const char* title;
    const char* company;
    uint width;
    uint height;
    uint language;
} GameConfig;
extern GameConfig Game;

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

BASALT void PrintASCIILogo(const char* suffix);
BASALT extern void DrawDot(Texture canvas, int posX, int posY, Color tint);
BASALT void DrawLine(Texture canvas, int startX, int startY, int endX, int endY, Color tint);
BASALT void DrawRectangle(Texture canvas, int posX, int posY, int width, int height, Color tint);
BASALT void DrawRectangleLines(Texture canvas,
                               int posX,
                               int posY,
                               int width,
                               int height,
                               int border,
                               Color tint);
BASALT void DrawWeirdTestGradient(Texture canvas);

BASALT extern void LoadFont(const char* name);
BASALT void LoadFontEx(const char* name, uint size);
#undef DrawText  // *yells at Microsoft*
BASALT void DrawText(Texture canvas, const char* text, int posX, int posY, Color color);
BASALT void DrawTextWithFont(const char* fontName,
                             Texture canvas,
                             const char* text,
                             int posX,
                             int posY,
                             Color color);

// TODO: Put in private header
BASALT void DisposeFonts();

BASALT Texture InitTexture(int width, int height);
BASALT Texture CopyTexture(Texture source);
BASALT void CopyTextureInto(Texture dest, Texture source);
BASALT void DisposeTexture(Texture texture);

// Texture pixels are in ABGR (big endian), use this to convert to correct colors for XImage
// NOTE: What is the correct format you might ask? I have no idea, found out after trial-and-error.
BASALT extern void MapTextureToCorrectFormat(Texture dest, Texture src);
BASALT void SwapTextureChannels(Texture dest,
                                Texture src,
                                uchar first,
                                uchar second,
                                uchar third,
                                uchar fourth);

BASALT void ClearTexture(Texture canvas, Color tint);
BASALT void DrawTextureEx(Texture canvas,
                          Texture texture,
                          int posX,
                          int posY,
                          int srcX,
                          int srcY,
                          int srcWidth,
                          int srcHeight,
                          Color tint);

BASALT extern void DrawTexture(Texture canvas, Texture texture, int posX, int posY, Color tint);
BASALT void DrawTextureScaled(Texture canvas,
                              Texture texture,
                              int destX,
                              int destY,
                              int destWidth,
                              int destHeight,
                              Color tint);

#undef RGB  // *yells at Microsoft*
BASALT extern Color RGB(uchar r, uchar g, uchar b);
BASALT extern Color RGBA(uchar r, uchar g, uchar b, uchar a);
BASALT extern Color BlendColors(Color src, Color dest, uchar t);

// Main game methods (for example: temple_game.c)
DYNAMIC BASALT GameConfig ConfigureGame();
DYNAMIC BASALT void InitializeGame();
DYNAMIC BASALT void DisposeGame();
DYNAMIC BASALT void UpdateAndRenderGame(Texture canvas, float delta);

// basalt_tooling.c
BASALT bool UpdateAndRenderArchaeo(Texture canvas);
#define DRAWCALL(X, Y) DrawCallImpl(X, #Y)
BASALT void DrawCallImpl(Texture canvas, const char* desc);

// basalt_tests.c
BASALT void UnitTest();

// sdl2_basalt.h
BASALT void OpenSystemConsole();
BASALT void CloseSystemConsole();

#ifdef __cplusplus
}
#endif
