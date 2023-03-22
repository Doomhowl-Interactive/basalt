#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>

#if defined(_WIN64) || defined(_WIN32)
#include "../external/dirent.h"
#else
#include <dirent.h>
#endif

#define MAX_PATH_LENGTH 128

typedef unsigned int uint;
typedef unsigned char uchar;

// TODO: memory cleanup
typedef struct {
    char name[256];
    size_t size;
    uint32_t* data;
} Asset;

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

// TODO: Write unit test for this

Asset LoadAsset(char* fileName) {
    Asset asset;
    GetAssetName(asset.name, fileName);

    FILE* file = fopen(fileName, "r");
    assert(file);
    fseek(file, 0L, SEEK_END);

    // determine size to allocate
    asset.size = ftell(file);
    asset.data = malloc(sizeof(uint32_t) + asset.size);
    rewind(file);

    printf("Embedding asset %s with size %u bytes\n", asset.name, asset.size);

    // write length header into buffer
    asset.data[0] = asset.size;

    // write data into buffer
    for (int i = 1; i < asset.size; i++) {
        char c = fgetc(file);
        ((char*)asset.data)[i++] = (char) c;
    }

    return asset;
}

static void GenerateAssetCode(String* code, Asset asset) {
    clock_t startTime = clock();

    // write code header
    char header[64];
    sprintf(header, "unsigned char %s[%d] = {\n", asset.name, asset.size + sizeof(uint32_t));
    AppendString(code, header);

    // write each asset byte
    for (size_t i = 0; i < asset.size; i++) {
        char hexString[12];
        sprintf(hexString,"0x%X",asset.data[i]);
        AppendString(code, hexString);
        AppendString(code, ", ");
    }

    // end code block
    AppendString(code, "\n};");
}

void EmbedFolder(char* folder, char* outputFile) {
    FilePathList list = GetFolderFiles(folder, ".png");
    Asset* textures = (Asset*) malloc(sizeof(Asset)*list.count);

    for (int i = 0; i < list.count; i++) {
        char* file = list.files[i];
        textures[i] = LoadAsset(file);
    }

    UnloadFilePathList(list);

    String code = MakeString();

    // write Asset code
    for (int i = 0; i < list.count; i++) {
        GenerateAssetCode(&code, textures[i]);
        AppendString(&code, "\n");
    }

    SaveFileText(outputFile, code.text);
}

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

    // printf("Completed unit testing...\n");
    return true;
}

int main(int argc, char** argv) {
    assert(UnitTest());

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

    int len = strlen(code);
    printf("Wrote %d chars into %s\n", len, outputPath);

    free(code);
}

static void UnloadFilePathList(FilePathList list) {
    for (size_t i = 0; i < list.count; i++) {
        free(list.files[i]);
    }
    free(list.files);
}
