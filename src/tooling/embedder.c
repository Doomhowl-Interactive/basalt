#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

#if defined(_WIN64) || defined(_WIN32)
#include "external/dirent.h"
#else
#include <dirent.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#include "external/stb_image.h"

#define true 1
#define false 0

#define MAX_PATH_LENGTH 128

#ifndef __cplusplus
typedef int bool;
#endif

typedef unsigned int uint;
typedef unsigned char uchar;

#undef assert
void assert(bool cond) {
#ifdef BASALT_DEBUG
    if (!cond) {
        int* i = NULL;
        *i = 666;
    }
#endif
}

// TODO: memory cleanup

// === NEW DESIGN ===
// 1. Load all textures assets colors into memory
// 2. Extract unique colors from assets
// 3. Map pixels to table
// 4. Write palette array to file
// 5. Write pixel array to file
//
// PROS:
// - No stdb image required at runtime
// - Smaller executable size
// CONS:
// - Slower to render sprites due to frequent lookups (solution: caching)

typedef struct {
    char* name;
    int width;
    int height;
    int channels;
    uchar* colors;
    uchar* indices;
} Texture;

typedef struct {
    uint count;
    uint components;
    uchar* colors;
} Palette;

typedef struct {
    size_t count;
    size_t capacity;
    char** files;
} FilePathList;

typedef struct {
    size_t size;
    size_t capacity;
    char* text;
} String;

static String MakeString();
static String* AppendString(String* str, const char* add);
static void SaveFileText(char* outputPath, char* code);
static bool FileHasExtension(char* name, char* ext);
static void GetAssetName(char* dest, const char* path);
static FilePathList GetFolderFiles(char* folder, char* ext);
static void UnloadFilePathList(FilePathList list);

Texture LoadTexture(char* fileName) {
    Texture texture;

    // make asset name
    char assetName[256];
    GetAssetName(assetName, fileName);
    texture.name = strdup(assetName);

    // extract texture colors
    texture.colors = (uchar*) stbi_load(fileName, &texture.width, &texture.height, &texture.channels, 4);
    if (!texture.colors) {
        fprintf(stderr,"Could not load image colors %s.\n", fileName);
        exit(1);
    }

    // allocate space for indices array
    texture.indices = (uchar*) malloc(sizeof(uchar)*texture.width*texture.height);

    return texture;
}

uint RegisterPaletteColor(Palette* pal, uchar r, uchar g, uchar b, uchar a, bool* isNew) {
    // round alpha value
    if (a > 0) {
        a = 255;
    }

    // check if color already present
    for (uint i = 0; i < pal->count; i++) {
        if (pal->colors[i*pal->components + 0] == r &&
            pal->colors[i*pal->components + 1] == g &&
            pal->colors[i*pal->components + 2] == b &&
            pal->colors[i*pal->components + 3] == a){

            if (isNew) {
                *isNew = false;
            }
            return i;
        }
    }

    // add the new color
    if (pal->count == 0) {
        pal->colors = (uchar*)malloc(256 * pal->components);
    } else if (pal->count == 256) {
        fprintf(stderr,"\nYou are using too much colors! (max is 255)\n");
        exit(1);
    }

    int i = pal->count*pal->components;
    pal->colors[i + 0] = r;
    pal->colors[i + 1] = g;
    pal->colors[i + 2] = b;
    pal->colors[i + 3] = a;

    // printf("Registered color %d %d %d %d\n",r,g,b,a);

    if (isNew) {
        *isNew = true;
    }

    uint latest = pal->count++;
    return latest;
}

Palette LoadPalette(Texture* textures, size_t count) {
    Palette palette = { 0 };
    palette.components = 4;

    RegisterPaletteColor(&palette,255,0,255,0, NULL); // alpha

    for (int i = 0; i < count; i++) {
        Texture txt = textures[i];
        printf("Converting image %s ... ",txt.name);

        // read every pixel
        uchar* colors = txt.colors;
        uint unique = 0;
        for (int j = 0; j < txt.width*txt.height; j++) {
            uchar r = colors[j*txt.channels + 0];
            uchar g = colors[j*txt.channels + 1];
            uchar b = colors[j*txt.channels + 2];
            uchar a = txt.channels == 4 ? 255 : colors[j + 3];

            bool isNew;
            uchar index = RegisterPaletteColor(&palette,r,g,b,a,&isNew);
            txt.indices[j] = index;

            if (isNew){
                unique++;
            }
        }
        if (unique == 0){
            printf("(no new colors! :D )\n");
        } else {
            printf("(%d new colors)\n",unique);
        }
    }
    return palette;
}

void UnsignedIntegerToUnsignedChars(uint32_t value, unsigned char result[4]) {
    result[0] = (value >> 24) & 0xFF;
    result[1] = (value >> 16) & 0xFF;
    result[2] = (value >> 8) & 0xFF;
    result[3] = value & 0xFF;
}

static void GenerateTextureCode(String* code, Texture texture) {
    clock_t startTime = clock();

    // write code header
    char header[64];
    sprintf(header, "unsigned char %s[] = {\n", texture.name);
    AppendString(code, header);

    // write dimensions
    unsigned char width[4];
    UnsignedIntegerToUnsignedChars(texture.width, width);

    for (int i = 0; i < 4; i++) {
        char numChar[4];
        sprintf(numChar,"%d",width[i]);
        AppendString(code, numChar);
        AppendString(code, ", ");
    }

    unsigned char height[4];
    UnsignedIntegerToUnsignedChars(texture.height, height);

    for (int i = 0; i < 4; i++) {
        char numChar[4];
        sprintf(numChar,"%d",height[i]);
        AppendString(code, numChar);
        AppendString(code, ", ");
    }

    // write each pixel after
    for (int i = 0; i < texture.width*texture.height; i++) {
        uchar index = texture.indices[i];

        char numChar[4];
        sprintf(numChar,"%d",index);

        AppendString(code, numChar);
        AppendString(code, ", ");
    }
    // write terminator
    AppendString(code, "255");

    // end code block
    AppendString(code, "\n};");
}

static void GeneratePaletteCode(String* code, Palette palette) {
    clock_t startTime = clock();

    // write code header
    AppendString(code, "unsigned char PALETTE_COLORS[] = {\n");

    // write each pixel after
    for (int i = 0; i < palette.count; i++) {
        for (int j = 0; j < palette.components; j++){
            int index = i * palette.components + j;
            uchar v = palette.colors[index];

            char numChar[4];
            sprintf(numChar,"%d",v);

            AppendString(code, numChar);
            AppendString(code, ", ");
        }
    }

    // end code block
    AppendString(code, "\n};");
}

void EmbedFolder(char* folder, char* outputFile) {
    FilePathList list = GetFolderFiles(folder, ".png");
    Texture* textures = (Texture*) malloc(sizeof(Texture)*list.count);

    for (int i = 0; i < list.count; i++) {
        char* file = list.files[i];
        textures[i] = LoadTexture(file);
    }

    UnloadFilePathList(list);

    // load palette
    Palette palette = LoadPalette(textures, list.count);
    printf("Extracted %d colors from all textures\n", palette.count);

    String code = MakeString();

    // write palette header
    GeneratePaletteCode(&code, palette); 
    AppendString(&code, "\n");

    // write texture code
    for (int i = 0; i < list.count; i++) {
        GenerateTextureCode(&code, textures[i]);
        AppendString(&code, "\n");
    }

    SaveFileText(outputFile, code.text);
}

#ifdef BASALT_DEBUG
bool UnitTest() {
    char name1[128];
    GetAssetName(name1, "assets/spr_player_fox.png");
    assert(strcmp(name1, "SPR_PLAYER_FOX") == 0);

    char name2[128];
    GetAssetName(name2, "assets\\mus_overworld.ogg");
    assert(strcmp(name2, "MUS_OVERWORLD") == 0);

    String str = MakeString();
    AppendString(&str, "Hello");
    AppendString(&str, " world");
    AppendString(&str, "!");
    assert(strcmp(str.text,"Hello world!") == 0);

    printf("Completed unit testing...\n");
    return true;
}
#endif

int main(int argc, char** argv) {
#ifdef BASALT_DEBUG
    assert(UnitTest());
#endif

    if (argc != 3) {
        printf("Incorrect amount of arguments! Gave %d\n",argc);
        return 1;
    }

    char* inputFolder = argv[1];
    char* outputFile = argv[2];

    clock_t startTime = clock();

    EmbedFolder(inputFolder, outputFile);

    clock_t endTime = clock();
    double cpuTimeUsed = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;

    printf("Done in %f seconds\n", cpuTimeUsed);
}

// Utilities

static bool FileHasExtension(char* name, char* ext) {
    int cmp = strcmp(name + strlen(name) - strlen(ext), ext);
    return cmp == 0;
}

static FilePathList GetFolderFiles(char* folder, char* ext) {

    FilePathList list = { 0 };
    list.count = 0;
    list.capacity = 100;

    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(folder)) != NULL) {

        list.files = (char**) malloc(list.capacity * sizeof(char*));

        while ((ent = readdir(dir)) != NULL) {
            if (FileHasExtension(ent->d_name, ext)) {

                // expand FilePathList if needed
                if (list.count == list.capacity) {
                    list.capacity *= 2;
                    list.files = (char**) realloc(list.files,list.capacity * sizeof(char*));
                }

                char fullName[MAX_PATH_LENGTH];
                snprintf(fullName, MAX_PATH_LENGTH, "%s/%s", folder, ent->d_name);
                list.files[list.count++] = strdup(fullName);
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

static void GetAssetName(char* dest, const char* path) {
    char* name = strdup(path);

    // replace backslashes if any
    for (char* p = name; *p != '\0'; p++) {
        if (*p == '\\') {
            *p = '/';
        }
    }

    // only get filestem
    char* stem = strrchr(name, '/');
    if (stem == NULL){
        strcpy(dest, name);
    }else{
        strcpy(dest, stem+1);
    }
    //free(name);

    // remove extension
    char* dot = strrchr(dest, '.');
    if (dot != NULL) {
        *dot = '\0'; // cut off string at dot
    }

    // string to uppercase
    for (char* v = dest; *v != '\0'; v++){
        *v = toupper(*v);
    }
}

static String MakeString(){
    String str = { 0 };
    str.capacity = 128;
    return str;
}

static String* AppendString(String* str, const char* add) {
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

static void UnloadString(String* string) {
    string->size = 0;
    string->capacity = 100;
    if (string->text){
        free(string->text);
    }
}

static void SaveFileText(char* outputPath, char* code) {
    FILE* file;
    file = fopen(outputPath, "w");
    if (file == NULL) {
        fprintf(stderr,"Could not open file for writing.\n");
        exit(1);
    }
    fputs(code, file);
    fclose(file);
    printf("Wrote %d chars into %s\n", strlen(code), outputPath);
    free(code);
}

static void UnloadFilePathList(FilePathList list) {
    for (size_t i = 0; i < list.count; i++) {
        free(list.files[i]);
    }
    free(list.files);
}
