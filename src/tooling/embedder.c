#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#if defined(_WIN64) || defined(_WIN32)
#include "external/dirent.h"
#else
#include <dirent.h>
#endif

typedef struct {
    size_t size;
    size_t capacity;
    char* text;
} String;

static String MakeString();
static void UnloadString(String* str);
static String* AppendString(String* str, const char* add);

void WriteCode(char* outputPath, String code) {
    FILE* file;
    file = fopen(outputPath, "a");
    if (file == NULL) {
        fprintf(stderr,"Could not open file for writing.\n");
        exit(1);
    }

    // append code to disk file
    fprintf(file, "\n%s", code.text);
    fclose(file);
    printf("Wrote code into %s\n", outputPath);

    UnloadString(&code);
}

void ToUppercase(char* str) {
    while (*str){
        *str = toupper(*str);
        str++;
    }
}

void GetAssetName(char* dest, const char* path) {
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

    ToUppercase(dest);
}

unsigned char* LoadFileBytes(char* filePath, size_t* size) {
    FILE *file = fopen(filePath,"rb");
    if (file == NULL){
        fprintf(stderr,"Error opening file %s\n", filePath);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    *size = (size_t) ftell(file);
    fseek(file, 0, SEEK_SET);

    assert(*size > 0);
    unsigned char* buffer = (unsigned char*) malloc(*size);
    fread(buffer, *size, 1, file);
    fclose(file);

    return buffer;
}

void AppendFileCode(String* code, char* file) {

    // read raw binary
    size_t size = 0;
    unsigned char* fileBytes = LoadFileBytes(file, &size);
    assert(size > 0);

    char assetName[128];
    GetAssetName(assetName, file);
    printf("Embedding file %s named %s ", file, assetName);

    clock_t startTime = clock();

    // write code header
    {
        char header[64];
        sprintf(header, "unsigned char %s[] = {\n", assetName, size);
        AppendString(code, header);
    }

    // add size marker (4 bytes) == BIG ENDIAN
    {
        char sizeText[64];
        unsigned char s[4];
        s[0] = (unsigned char)(size >> 24);
        s[1] = (unsigned char)(size >> 16);
        s[2] = (unsigned char)(size >>  8);
        s[3] = (unsigned char)(size >>  0);
        sprintf(sizeText,"0x%02X,0x%02X,0x%02X,0x%02X, ",s[0],s[1],s[2],s[3]);
        AppendString(code, sizeText);
    }

    // write each pixel after
    for (int i = 0; i < size; i++) {
        char hexText[18];
        sprintf(hexText,"0x%02X,", fileBytes[i]);
        AppendString(code, hexText);
    }

    free(fileBytes);

    // end code block
    AppendString(code, "\n};\n");

    clock_t endTime = clock();
    double cpuTimeUsed = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;
    printf(" - %f secs...\n", cpuTimeUsed);
}

void ClearFile(char* outputFile) {
    FILE* file = fopen(outputFile, "w");
    if (file != NULL) {
        fputs("", file);
        fclose(file);
    }
}

void GetFolderFiles(char* folder, char* ext, char*** files);

void EncodeFolder(char* folder, char* outputFile) {
    ClearFile(outputFile);

    String code = MakeString();

    // TODO: hide in struct
    char** files = NULL;
    GetFolderFiles(folder, ".png", &files);

    for (int i = 0; files[i] != NULL; i++) {
        AppendFileCode(&code, files[i]);
        free(files[i]);
    }

    WriteCode(outputFile, code);

    free(files);
}

void UnitTest() {

    // test appending assets
    {
        char name1[128];
        GetAssetName(name1, "assets/spr_player_fox.png");
        assert(strcmp(name1, "SPR_PLAYER_FOX") == 0);

        char name2[128];
        GetAssetName(name2, "assets\\mus_overworld.ogg");
        assert(strcmp(name2, "MUS_OVERWORLD") == 0);
    }

    // test text capitalization
    {
        char clone[16];
        strcpy(clone, "Hello");
        ToUppercase(clone);
        assert(strcmp(clone,"HELLO") == 0);
    }

    // test appending strings
    {
        String str = MakeString();
        AppendString(&str, "Hello");
        AppendString(&str, " world");
        AppendString(&str, "!");
        assert(strcmp(str.text,"Hello world!") == 0);
    }

    // printf("Completed unit testing...\n");
}

int main(int argc, char** argv) {
    UnitTest();

    if (argc != 3) {
        printf("Incorrect amount of arguments! Gave %d\n",argc);
        return 1;
    }

    char* inputFolder = argv[1];
    char* outputFile = argv[2];

    clock_t startTime = clock();

    EncodeFolder(inputFolder, outputFile);

    clock_t endTime = clock();
    double cpuTimeUsed = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;

    printf("Done in %f seconds\n", cpuTimeUsed);
}

#define MAX_PATH_LENGTH 128
#define MAX_FILE_COUNT 512
void GetFolderFiles(char* folder, char* ext, char*** files) {

    DIR* dir;
    struct dirent* ent;
    int extLen = strlen(ext);
    if ((dir = opendir(folder)) != NULL) {

        // list
        *files = (char**)malloc((MAX_FILE_COUNT + 1) * sizeof(char*));

        int index = 0;
        while ((ent = readdir(dir)) != NULL) {
            int nameLen = strlen(ent->d_name);
            if (nameLen > extLen && strcmp(ent->d_name + nameLen - extLen, ext) == 0) {
                char fullName[MAX_PATH_LENGTH];
                snprintf(fullName, MAX_PATH_LENGTH, "%s/%s", folder, ent->d_name);
                (*files)[index++] = strdup(fullName);
            }
        }
        closedir(dir);

        // append null to end list
        (*files)[index++] = NULL;
    }
    else {
        (*files) = NULL;
        fprintf(stderr,"Unable to open directory");
        exit(1);
    }
}

// string implementation
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

static void UnloadString(String* str) {
    str->size = 0;
    str->capacity = 100;
    if (str->text){
        free(str->text);
    }
}
