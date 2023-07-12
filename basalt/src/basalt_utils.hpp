#pragma once
#include "basalt.h"

// NOTE: Returns true if engine should continue runnings
bool ParseLaunchArguments(int argc, char** argv);
const char* GetWorkingDirectory();

// Cross platform println function
// All text passed to this function will by stored in a buffer, allowing the possibility to render
// console output into your game or easily dump it to a file.
#define BasaltPrint(...) BasaltPrintColored(WHITE, __VA_ARGS__)
void BasaltPrintColored(ConsoleColor color, const char* format, ...);

String GetBasaltLog();

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
