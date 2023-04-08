#include "basalt.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <ctype.h>

#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#if defined(_WIN64) || defined(_WIN32)
#include "external/dirent.h"
#else
#include <dirent.h>
#endif

static usize PrevRNGFrame = 0;
static usize RNGOffset = 0;

// NOTE: Random numbers aren't actually random, they're based on frame index
// in order to make reproducable tests 
// If you want "real" random numbers, use GetRealRandomNumber()
pubfunc int GetRandomNumber()
{
#ifndef BASALT_NO_ENGINE
    usize curFrame = GetFrameIndex();
    if (PrevRNGFrame == curFrame)
    {
        RNGOffset++;
    }
    else
    {
        RNGOffset += 0;
        PrevRNGFrame = curFrame;
    }

    // TODO: This could be better
    int rng = ((curFrame * 69696420) << 2) * (525+RNGOffset);
    return rng;
#else
    return GetRealRandomNumber();
#endif
}

pubfunc int GetRealRandomNumber()
{
    int rng = rand();
    return rng;
}

pubfunc bool IsLittleEndian() {
    volatile uint i = 0x01234567;
    bool littleEndian = *((uchar*)(&i)) == 0x67;
    return littleEndian;
}

pubfunc int Clamp(int value, int min, int max) {
    if (value < min) {
        return min;
    }

    if (value > max) {
        return max;
    }

    return value;
}

pubfunc Vec2 Vec2Normalize(Vec2 v2)
{
    float mag = Vec2Magnitude(v2);

    Vec2 norm;
    norm.x = v2.x / mag;
    norm.y = v2.y / mag;
    return norm;
}

pubfunc float Vec2Magnitude(Vec2 v2)
{
    // Pythagorean theorem
#ifdef BASALT_NO_ENGINE
    assert(0);
#else
    return sqrt(v2.x*v2.x + v2.y*v2.y);
#endif
}

pubfunc StringArray InitStringArray()
{
    StringArray s;
    s.strings = NULL;
    s.count = 0;
    s.capacity = 20;
    return s;
}

pubfunc void StoreString(StringArray* arr, char* text)
{
    if (arr->strings == NULL)
        arr->strings = calloc(sizeof(char*),arr->capacity);

    if (arr->count == arr->capacity)
    {
        arr->capacity += 20;
        arr->strings = realloc(arr->strings, sizeof(char*) * arr->capacity);
    }
    arr->strings[arr->count++] = strdup(text);
}

pubfunc void DisposeStringArray(StringArray* arr)
{
    if (arr->strings != NULL)
    {
        for (usize i = 0; i < arr->count; i++)
            free(arr->strings[i]);

        free(arr->strings);
    }
}

// string implementation
pubfunc String MakeString(){
    String str = { 0 };
    str.capacity = 128;
    return str;
}

pubfunc String* AppendString(String* str, const char* add) {
    size_t addLen = strlen(add);
    str->size += addLen;

    // allocate string
    if (str->text == NULL) {
        str->capacity = str->size + 1; // +1 for null terminator
        str->text = (char*)malloc(str->capacity * sizeof(char));
    }

    // grow string
    if (str->size >= str->capacity) {
        str->capacity += 500;
        str->text = (char*)realloc(str->text, str->capacity * sizeof(char));
    }

    char* head = &str->text[str->size - addLen]; // calculate head position
    strcpy(head, add);

    return str;
}

pubfunc void UnloadString(String* str) {
    str->size = 0;
    str->capacity = 100;
    if (str->text){
        free(str->text);
    }
}

pubfunc void ToUppercase(char* str) {
    while (*str){
        *str = toupper(*str);
        str++;
    }
}

// FIXME: Untested
#define MAX_PADDING_LENGTH 256
static char PaddingCache[MAX_PADDING_LENGTH]; 
pubfunc const char* PadStringRight(const char* text, char symbol, usize length)
{
    memset(PaddingCache, symbol, length);
    PaddingCache[length] = '\0';

    int len = MIN(MAX_PADDING_LENGTH, MIN(strlen(text),length));
    memcpy(PaddingCache, text, len);

    return PaddingCache;
}

pubfunc const char* GetFirstExistingFolder(const char** folders)
{
    for (const char* folder = folders[0]; folder != NULL; folder++)
    {
        if (folder != NULL && FolderExists(folder))
            return folder;
    }
    return NULL;
}

pubfunc bool FolderExists(const char* folder)
{
    DIR* dir;
    if ((dir = opendir(folder)) != NULL) {
        closedir(dir);
        return true;
    }
    return false;
}

pubfunc ulong GetFileModifiedTime(const char* filePath)
{
    struct stat attr;
    stat(filePath, &attr);
    ulong modifiedTime = attr.st_mtime;
    return modifiedTime;
}

static char FileNameCache[MAX_PATH_LENGTH]; 
pubfunc const char* GetFileName(const char* folder)
{
    strcpy(FileNameCache,folder);
    return basename(FileNameCache);
}

pubfunc const char* GetFileStem(const char* folder)
{
    const char* fileName = GetFileName(folder);

    char* stem = strrchr(fileName, '.');
    if (stem != NULL)
        *stem = '\0';

    return fileName;
}

pubfunc bool FileHasExtension(const char* name, const char* ext) {
    int cmp = strcmp(name + strlen(name) - strlen(ext), ext);
    return cmp == 0;
}

pubfunc FilePathList GetFolderFiles(const char* folder, const char* ext) {

    FilePathList list = { 0 };
    list.count = 0;
    list.capacity = 20;

    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(folder)) != NULL) {
        list.strings = (char**) malloc(list.capacity * sizeof(char*));
        while ((ent = readdir(dir)) != NULL) {
            if (FileHasExtension(ent->d_name, ext)) {

                // expand FilePathList if needed
                if (list.count == list.capacity) {
                    list.capacity *= 2;
                    list.strings = (char**) realloc(list.strings,list.capacity * sizeof(char*));
                }

                char fullName[MAX_PATH_LENGTH];
                snprintf(fullName, MAX_PATH_LENGTH, "%s/%s", folder, ent->d_name);
                list.strings[list.count++] = strdup(fullName);
            }
        }
        closedir(dir);
    }
    else {
        fprintf(stderr,"Unable to open directory\n");
        exit(1);
    }

    return list;
}

pubfunc void UnloadFilePathList(FilePathList list) {
    for (size_t i = 0; i < list.count; i++) {
        free(list.strings[i]);
    }
    free(list.strings);
}
