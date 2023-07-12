#include <iostream>
#include <cassert>
#include <cstdarg>

#include "basalt_math.hpp"

// adapted from Raylib
// WARN: Cached memory, copy for long usage!
const char* FormatText(const char* text, ...)
{
    constexpr int MAX_TEXT_LEN = 1024;
    constexpr int TEXT_BUFFER_COUNT = 16;

    assert(text);

    static char buffers[TEXT_BUFFER_COUNT][MAX_TEXT_LEN];
    static size_t curBufferIndex = 0;

    char* currentBuffer = buffers[curBufferIndex++];
    curBufferIndex %= TEXT_BUFFER_COUNT;

    va_list args;
    va_start(args, text);
    vsnprintf(currentBuffer, MAX_TEXT_LEN, text, args);

    return currentBuffer;
}

inline bool TextIsEqual(const char* text1, const char* text2)
{
    assert(text1);
    assert(text2);
    return strcmp(text1, text2) == 0;
}

inline bool TextIsEqualNoCase(const char* text1, const char* text2)
{
    char cache[1024];  // HACK TODO: ToLowercase can only remember one string!
    strcpy(cache, ToLowercase(text1));
    return TextIsEqual(cache, ToLowercase(text2));
}

inline const char* AppendText(const char* src, const char* add)
{
    assert(src);
    assert(add);
    return FormatText("%s%s", src, add);
}

char* StripText(char* buffer)
{
    // Strip leading spaces
    char* start = buffer;
    while (isspace(*start)) {
        ++start;
    }

    // Strip trailing spaces
    char* end = buffer + strlen(buffer) - 1;
    while (end > start && isspace(*end)) {
        --end;
    }

    // Null-terminate the modified string
    *(end + 1) = '\0';

    // Shift the remaining characters to the beginning of the string
    memmove(buffer, start, end - start + 2);
    return buffer;
}

// from raylib
int CopyText(char* dst, const char* src)
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

inline char* CloneText(const char* text)
{
    return _strdup(text);
}

inline usize TextLength(const char* text)
{
    return strlen(text);
}

// string implementation
String MakeString()
{
    String str = { 0 };
    str.capacity = 128;
    return str;
}

String* AppendString(String* str, const char* add)
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

void DisposeString(String* str)
{
    str->size = 0;
    str->capacity = 100;
    if (str->text) {
        free(str->text);
    }
}

#define MAX_CASE_LEN 1024
static char CaseBuffer[MAX_CASE_LEN];
const char* ToUppercase(const char* str)
{
    char* dst = CaseBuffer;
    if ((str != NULL) && (dst != NULL)) {
        while (*str != '\0') {
            *dst = toupper(*str);
            dst++;
            str++;
        }

        *dst = '\0';
    }
    return CaseBuffer;
}

const char* ToLowercase(const char* str)
{
    char* dst = CaseBuffer;
    if ((str != NULL) && (dst != NULL)) {
        while (*str != '\0') {
            *dst = tolower(*str);
            dst++;
            str++;
        }
        *dst = '\0';
    }
    return CaseBuffer;
}

// FIXME: Untested
#define MAX_PADDING_LENGTH 256
static char PaddingCache[MAX_PADDING_LENGTH];
const char* PadStringRight(const char* text, char symbol, usize length)
{
    memset(PaddingCache, symbol, length);
    PaddingCache[length] = '\0';

    int len = MIN(MAX_PADDING_LENGTH, MIN(TextLength(text), length));
    memcpy(PaddingCache, text, len);

    return PaddingCache;
}

const char* GetFirstExistingFolder(const char** folders)
{
    while (*folders) {
        if (FolderExists(*folders)) {
            return *folders;
        }
        folders++;
    }
    return NULL;
}

bool FolderExists(const char* folder)
{
    DIR* dir = opendir(folder);
    if (dir != NULL) {
        closedir(dir);
        return true;
    }
    return false;
}

long GetFileModifiedTime(const char* filePath)
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
const char* GetFileName(const char* filePath)
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
const char* GetFileStem(const char* filePath)
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

inline bool FileHasExtension(const char* name, const char* ext)
{
    const char* fileStem = GetFileStem(name);
    return TextIsEqual(fileStem, ext);
}

StringArray GetFolderFiles(const char* folder, const char* ext)
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

                char fullName[MAX_PATH + 300];
                snprintf(fullName, MAX_PATH + 300, "%s/%s", folder, ent->d_name);
                list.strings[list.count++] = _strdup(fullName);
            }
        }
        closedir(dir);
    } else {
        fprintf(stderr, "Unable to open directory\n");
        exit(EXIT_FAILURE);
    }

    return list;
}
