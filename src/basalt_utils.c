#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include "basalt.h"

#if defined(_WIN64) || defined(_WIN32)
# include "external/dirent.h"
#else
# include <dirent.h>
#endif

static usize PrevRNGFrame = 0;
static usize RNGOffset = 0;

// NOTE: Random numbers aren't actually random, they're based on frame index
// in order to make reproducable tests
// If you want "real" random numbers, use GetRealRandomNumber()
BASALT int GetRandomNumber()
{
#ifndef BASALT_NO_ENGINE
    usize curFrame = GetFrameIndex();
    if (PrevRNGFrame == curFrame) {
        RNGOffset++;
    } else {
        RNGOffset = 0;
        PrevRNGFrame = curFrame;
    }

    int rng = ((curFrame + RNGOffset * 69696420) << 2);
    return rng;
#else
    return GetRealRandomNumber();
#endif
}

BASALT int GetRealRandomNumber()
{
    int rng = rand();
    return rng;
}

BASALT bool IsLittleEndian()
{
    volatile uint i = 0x01234567;
    bool littleEndian = *((uchar*)(&i)) == 0x67;
    return littleEndian;
}

BASALT int Clamp(int value, int min, int max)
{
    if (value < min) {
        return min;
    }

    if (value > max) {
        return max;
    }

    return value;
}

BASALT inline Vec2 RectFOrigin(RectF rectf)
{
    Vec2 origin = {
        rectf.x,
        rectf.y,
    };
    return origin;
}

BASALT inline Vec2 RectFCenter(RectF rectf)
{
    Vec2 res = {
        rectf.x + rectf.width * 0.5f,
        rectf.y + rectf.height * 0.5f,
    };
    return res;
}

BASALT inline Point RectOrigin(Rect rect)
{
    Point origin = {
        rect.x,
        rect.y,
    };
    return origin;
}

BASALT inline Point RectCenter(Rect rect)
{
    Point res = {
        rect.x + rect.width / 2,
        rect.y + rect.height / 2,
    };
    return res;
}

BASALT inline Rect RectFToRect(RectF rectf)
{
    Rect rect = {
        (int)rectf.x,
        (int)rectf.y,
        (int)rectf.width,
        (int)rectf.height,
    };
    return rect;
}

BASALT inline RectF RectToRectF(Rect rect)
{
    RectF rectf = {
        (float)rect.x,
        (float)rect.y,
        (float)rect.width,
        (float)rect.height,
    };
    return rectf;
}

BASALT inline Point Vec2ToPoint(Vec2 v2)
{
    Point p = { (int)v2.x, (int)v2.y };
    return p;
}

BASALT inline Vec2 PointToVec2(Point point)
{
    Vec2 v2 = { (float)point.x, (float)point.y };
    return v2;
}

BASALT inline Vec2 Vec2Add(Vec2 src, Vec2 offset)
{
    Vec2 v2 = {
        src.x + offset.x,
        src.y + offset.y,
    };
    return v2;
}

BASALT inline Vec2 Vec2Subtract(Vec2 src, Vec2 offset)
{
    Vec2 v2 = {
        src.x - offset.x,
        src.y - offset.y,
    };
    return v2;
}

BASALT inline Vec2 Vec2Scale(Vec2 src, float scale)
{
    Vec2 v2 = {
        src.x * scale,
        src.y * scale,
    };
    return v2;
}

BASALT inline Vec2 Vec2Normalize(Vec2 v2)
{
    float mag = Vec2Magnitude(v2);
    Vec2 norm;
    norm.x = v2.x / mag;
    norm.y = v2.y / mag;
    return norm;
}

BASALT inline float Vec2Magnitude(Vec2 v2)
{
    // Pythagorean theorem
#ifdef BASALT_NO_ENGINE
    assert(0);
    return 0.f;
#else
    return sqrt(v2.x * v2.x + v2.y * v2.y);
#endif
}

BASALT inline Vec2 Vec2Towards(Vec2 src, Vec2 dest)
{
    Vec2 diff = Vec2Subtract(dest, src);
    return Vec2Normalize(diff);
}

BASALT inline float Vec2DistanceSquared(Vec2 first, Vec2 second)
{
    float dist = ((second.x - first.x) * (second.x - first.x)) + ((second.y - first.y) * (second.y - first.y));
    return dist;
}

BASALT inline float Vec2Distance(Vec2 first, Vec2 second)
{
    float distRoot = sqrtf(Vec2DistanceSquared(first, second));
    return distRoot;
}

BASALT StringArray InitStringArray()
{
    StringArray s;
    s.strings = NULL;
    s.count = 0;
    s.capacity = 20;
    return s;
}

BASALT void StoreString(StringArray* arr, char* text)
{
    if (arr->strings == NULL)
        arr->strings = (char**)calloc(sizeof(char*), arr->capacity);

    if (arr->count == arr->capacity) {
        arr->capacity += 20;
        arr->strings = (char**)realloc(arr->strings, sizeof(char*) * arr->capacity);
    }
    arr->strings[arr->count++] = strdup(text);
}

BASALT void DisposeStringArray(StringArray* arr)
{
    if (arr->strings != NULL) {
        for (usize i = 0; i < arr->count; i++)
            free(arr->strings[i]);

        free(arr->strings);
    }
}

// adapted from Raylib
// WARN: Cached memory, copy for long usage!
BASALT const char* FormatText(const char* text, ...)
{
#define MAX_TEXT_LEN 1024
#define TEXT_BUFFER_COUNT 16
    assert(text);

    static char buffers[TEXT_BUFFER_COUNT][MAX_TEXT_LEN];
    static usize curBufferIndex = 0;

    char* currentBuffer = buffers[curBufferIndex++];
    curBufferIndex %= TEXT_BUFFER_COUNT;

    va_list args;
    va_start(args, text);
    vsnprintf(currentBuffer, MAX_TEXT_LEN, text, args);

    return currentBuffer;
}

BASALT inline bool TextIsEqual(const char* text1, const char* text2)
{
    assert(text1);
    assert(text2);
    return strcmp(text1, text2) == 0;
}

BASALT inline const char* AppendText(const char* src, const char* add)
{
    assert(src);
    assert(add);
    return FormatText("%s%s", src, add);
}

// from raylib
BASALT int CopyText(char* dst, const char* src)
{
    int bytes = 0;

    if ((src != NULL) && (dst != NULL)) {
        while (*src != '\0') {
            *dst = *src;
            dst++;
            src++;

            bytes++;
        }

        *dst = '\0';
    }
    return bytes;
}

BASALT inline usize TextLength(const char* text)
{
    return strlen(text);
}

// string implementation
BASALT String MakeString()
{
    String str = { 0 };
    str.capacity = 128;
    return str;
}

BASALT String* AppendString(String* str, const char* add)
{
    size_t addLen = TextLength(add);
    str->size += addLen;

    // allocate string
    if (str->text == NULL) {
        str->capacity = str->size + 1;  // +1 for null terminator
        str->text = (char*)malloc(str->capacity * sizeof(char));
    }

    // grow string
    if (str->size >= str->capacity) {
        str->capacity += 500;
        str->text = (char*)realloc(str->text, str->capacity * sizeof(char));
    }

    char* head = &str->text[str->size - addLen];  // calculate head position
    CopyText(head, add);

    return str;
}

BASALT void DisposeString(String* str)
{
    str->size = 0;
    str->capacity = 100;
    if (str->text) {
        free(str->text);
    }
}

BASALT void ToUppercase(char* str)
{
    while (*str) {
        *str = toupper(*str);
        str++;
    }
}

BASALT void ToLowercase(char* str)
{
    while (*str) {
        *str = tolower(*str);
        str++;
    }
}

// FIXME: Untested
#define MAX_PADDING_LENGTH 256
static char PaddingCache[MAX_PADDING_LENGTH];
BASALT const char* PadStringRight(const char* text, char symbol, usize length)
{
    memset(PaddingCache, symbol, length);
    PaddingCache[length] = '\0';

    int len = MIN(MAX_PADDING_LENGTH, MIN(TextLength(text), length));
    memcpy(PaddingCache, text, len);

    return PaddingCache;
}

BASALT const char* GetFirstExistingFolder(const char** folders)
{
    for (const char* folder = folders[0]; folder != NULL; folder++) {
        if (folder != NULL && FolderExists(folder))
            return folder;
    }
    return NULL;
}

BASALT bool FolderExists(const char* folder)
{
    DIR* dir = opendir(folder);
    if (dir != NULL) {
        closedir(dir);
        return true;
    }
    return false;
}

BASALT long GetFileModifiedTime(const char* filePath)
{
#ifndef BASALT_NO_ENGINE
    struct stat attr;
    if (stat(filePath, &attr) == -1) {
        WARN("Failed to determine modifiedTime of %s", filePath);
        return 0;
    }
    long modifiedTime = attr.st_mtime;
    return modifiedTime;
#else
    return 0;
#endif
}

// raylib.h (rcore.c)
BASALT const char* GetFileName(const char* filePath)
{
    const char* fileName = NULL;
    if (filePath != NULL) {
        fileName = strpbrk(filePath, "\\/");
    }

    if (!fileName) {
        return filePath;
    }

    return fileName + 1;
}

// raylib.h (rcore.c)
BASALT const char* GetFileStem(const char* filePath)
{
#define MAX_BUFFER_LEN 256

    static char fileName[MAX_BUFFER_LEN];

    if (filePath != NULL) {
        CopyText(fileName, GetFileName(filePath));  // Get filename with extension
    }

    int size = (int)TextLength(fileName);  // Get size in bytes

    for (int i = 0; (i < size) && (i < MAX_BUFFER_LEN); i++) {
        if (fileName[i] == '.') {
            // NOTE: We break on first '.' found
            fileName[i] = '\0';
            break;
        }
    }

    return fileName;
}

BASALT inline bool FileHasExtension(const char* name, const char* ext)
{
    const char* fileStem = GetFileStem(name);
    return TextIsEqual(fileStem, ext);
}

BASALT StringArray GetFolderFiles(const char* folder, const char* ext)
{
    StringArray list = { 0 };
    list.count = 0;
    list.capacity = 20;

    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(folder)) != NULL) {
        list.strings = (char**)malloc(list.capacity * sizeof(char*));
        while ((ent = readdir(dir)) != NULL) {
            if (FileHasExtension(ent->d_name, ext)) {
                // expand StringArray if needed
                if (list.count == list.capacity) {
                    list.capacity *= 2;
                    list.strings = (char**)realloc(list.strings, list.capacity * sizeof(char*));
                }

                char fullName[MAX_PATH_LENGTH + 300];
                snprintf(fullName, MAX_PATH_LENGTH + 300, "%s/%s", folder, ent->d_name);
                list.strings[list.count++] = strdup(fullName);
            }
        }
        closedir(dir);
    } else {
        fprintf(stderr, "Unable to open directory\n");
        exit(EXIT_FAILURE);
    }

    return list;
}
