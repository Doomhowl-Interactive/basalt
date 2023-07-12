#pragma once

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

#define DYNAMIC

// NOTE: Returns true if engine should continue runnings
bool ParseLaunchArguments(int argc, char** argv);
const char* GetWorkingDirectory();

// Cross platform println function
// All text passed to this function will by stored in a buffer, allowing the possibility to render
// console output into your game or easily dump it to a file.
#define BasaltPrint(...) BasaltPrintColored(WHITE, __VA_ARGS__)
void BasaltPrintColored(ConsoleColor color, const char* format, ...);

String GetBasaltLog();

#define DEG2RAD(Y) ((Y)*PI / 180)
#define RAD2DEG(X) ((X)*180.0 / PI)
int Clamp(int value, int min, int max);
bool IsLittleEndian();

void SetRandomSeed(uint seed);
int GetRandomRange(int min, int max);
int GetRandomNumber();

extern Vec2 RectFOrigin(RectF rectf);
#define RectFOriginPoint(F) Vec2ToPoint(RectFOrigin(F))
extern Vec2 RectFCenter(RectF rectf);
#define RectFCenterPoint(F) Vec2ToPoint(RectFCenter(F))

extern Point RectOrigin(Rect rect);
#define RectOriginVec2(R) PointToVec2(RectOrigin(R))
extern Point RectCenter(Rect rect);
#define RectCenterVec2(R) PointToVec2(RectCenter(R))

extern Rect RectFToRect(RectF rectf);
extern RectF RectToRectF(Rect rect);
extern bool RectFOverlaps(RectF first, RectF second);
extern bool PointInRectF(Point point, RectF rectf);
extern bool PointInRect(Point point, Rect rect);

extern Point Vec2ToPoint(Vec2 v2);
extern Vec2 PointToVec2(Point point);

extern Vec2 Vec2Add(Vec2 src, Vec2 offset);
extern Vec2 Vec2Subtract(Vec2 src, Vec2 offset);
extern Vec2 Vec2Scale(Vec2 src, float scale);
extern Vec2 Vec2Normalize(Vec2 v2);
extern Vec2 Vec2Negate(Vec2 v2);
extern float Vec2Magnitude(Vec2 v2);

// NOTE: Get's normalized direction from src to dest
extern Vec2 Vec2Towards(Vec2 src, Vec2 dest);
extern float Vec2DistanceSquared(Vec2 first, Vec2 second);

// WARN: Taking square roots is expensive! Use Vec2DistanceSquared instead!
extern float Vec2Distance(Vec2 first, Vec2 second);

StringArray InitStringArray();
void StoreString(StringArray* arr, char* text);
void DisposeStringArray(StringArray* arr);

// WARN: Cached memory, copy for long usage!
const char* FormatText(const char* text, ...);
extern bool TextIsEqual(const char* text1, const char* text2);
extern bool TextIsEqualNoCase(const char* text1, const char* text2);
extern const char* AppendText(const char* src, const char* add);
extern char* CloneText(const char* text);  // WARN: free after use
extern usize TextLength(const char* text);
char* StripText(char* buffer);  // strip happens in-place!
int CopyText(char* dst, const char* src);

// TODO FIX: Inconsistent naming with InitStringArray
String MakeString();
void DisposeString(String* str);
String* AppendString(String* str, const char* add);
const char* ToUppercase(const char* str);
const char* ToLowercase(const char* str);
const char* PadStringRight(const char* text, char symbol, usize length);

extern bool FileHasExtension(const char* name, const char* ext);
bool FolderExists(const char* folder);

// WARN: Uses cached memory, result gets overwritten on future calls.
// NOTE: Get seconds since epoch a file was last changed.
// Returns 0 if file doesn't exists.
long GetFileModifiedTime(const char* filePath);
const char* GetFileName(const char* filePath);
const char* GetFileStem(const char* filePath);

// NOTE: Returns NULL when none exist,
// pass NULL ended array
const char* GetFirstExistingFolder(const char** folders);
StringArray GetFolderFiles(const char* folder, const char* ext);
const char* GetWorkingDirectory();

// Asset handling (basalt_assets.c)
typedef struct Texture {
    const char* name;
    int width;
    int height;
    Color* pixels;
} Texture;
extern const uchar SPR_PIXELFONT[];

Texture RequestTexture(const char* name);
void InitHotReloading();
void HotReloadTexture(Texture texture);

// NOTE: Returns null ended array of loaded/cached textures
Texture* GetLoadedTextures();
void TakeScreenshot(Texture canvas);

// Platform dependent stuff
Point GetMousePosition();
void SetWindowTitle(const char* title);

// NOTE: Get number of frames elapsed since the start of the simulation.
usize GetFrameIndex();

// NOTE: Get time elapsed since the start of the simulation.
double GetTimeElapsed();

bool IsMouseDown();
bool IsMousePressed();
bool IsMouseUp();

bool IsKeyDown(KeyCode code);
bool IsKeyUp(KeyCode code);

bool IsKeyPressed(KeyCode code);
bool IsKeyReleased(KeyCode code);

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
#define GRAY 0xAAAAAAFF
#define LIGHTGRAY 0xC8C8C8FF
#define DARKGRAY 0x505050FF
#define BLACK 0x000000FF
#define BLANK 0xFFFFFF00

#define RED 0xFF0000FF
#define GREEN 0x00FF00FF
#define DARKGREEN 0x00752CFF
#define BLUE 0x0000FFFF
#define DARKBLUE 0x00527FFF
#define YELLOW 0xFFFF00FF
#define PURPLE 0x00FFFFFF
#define BROWN 0x7F3F00FF
#define DARKBROWN 0x5A2E00FF
#define DARKRED 0x7F0000FF

void PrintASCIILogo(const char* suffix);
extern void DrawDot(Texture canvas, int posX, int posY, Color tint);
void DrawLine(Texture canvas, int startX, int startY, int endX, int endY, Color tint);
void DrawRectangle(Texture canvas, int posX, int posY, int width, int height, Color tint);
void DrawRectangleLines(Texture canvas,
                        int posX,
                        int posY,
                        int width,
                        int height,
                        int border,
                        Color tint);
void DrawWeirdTestGradient(Texture canvas);

extern void LoadFont(const char* name);
void LoadFontEx(const char* name, uint size);
#undef DrawText  // *yells at Microsoft*
void DrawText(Texture canvas, const char* text, int posX, int posY, Color color);
void DrawTextWithFont(const char* fontName,
                      Texture canvas,
                      const char* text,
                      int posX,
                      int posY,
                      Color color);

// TODO: Put in private header
void DisposeFonts();

Texture InitTexture(int width, int height);
Texture CopyTexture(Texture source);
void CopyTextureInto(Texture dest, Texture source);
void DisposeTexture(Texture texture);

// Texture pixels are in ABGR (big endian), use this to convert to correct colors for XImage
// NOTE: What is the correct format you might ask? I have no idea, found out after trial-and-error.
extern void MapTextureToCorrectFormat(Texture dest, Texture src);
void SwapTextureChannels(Texture dest,
                         Texture src,
                         uchar first,
                         uchar second,
                         uchar third,
                         uchar fourth);

void ClearTexture(Texture canvas, Color tint);
void DrawTextureEx(Texture canvas,
                   Texture texture,
                   int posX,
                   int posY,
                   int srcX,
                   int srcY,
                   int srcWidth,
                   int srcHeight,
                   Color tint);

extern void DrawTexture(Texture canvas, Texture texture, int posX, int posY, Color tint);
void DrawTextureScaled(Texture canvas,
                       Texture texture,
                       int destX,
                       int destY,
                       int destWidth,
                       int destHeight,
                       Color tint);

#undef RGB  // *yells at Microsoft*
extern Color RGB(uchar r, uchar g, uchar b);
extern Color RGBA(uchar r, uchar g, uchar b, uchar a);
Color BlendColors(Color src, Color dest, uchar t);
extern Color ColorAlpha(Color col, float a);

// Main game methods (for example: temple_game.c)
DYNAMIC GameConfig ConfigureGame();
DYNAMIC void InitializeGame();
DYNAMIC void DisposeGame();
DYNAMIC void UpdateAndRenderGame(Texture canvas, float delta);

// basalt_tooling.c
bool UpdateAndRenderArchaeo(Texture canvas);
#define DRAWCALL(X, Y) Archaeo::DrawCallImpl(X, #Y)
void DrawCallImpl(Texture canvas, const char* desc);

// basalt_tests.c
void UnitTest();

// sdl2_basalt.h
void OpenSystemConsole();
void CloseSystemConsole();

#ifdef __cplusplus
}
#endif
