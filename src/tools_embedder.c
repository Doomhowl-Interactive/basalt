#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#if defined(_WIN64) || defined(_WIN32)
#include "external/dirent.h"
#else
#include <dirent.h>
#endif

#define true 1
#define false 0

typedef int bool;

#undef assert
void assert(bool cond) {
#ifdef DEBUG
    if (!cond) {
        int* i = NULL;
        *i = 666;
    }
#endif
}

// malloc condom
void* s_malloc(size_t len){
    void* mem = malloc(len);
    if (mem == NULL){
        fprintf(stderr, "Failed to allocate memory!");
        exit(1);
    }
    return mem;
}

void WriteCode(char* outputPath, char* code) {
    FILE* file;
    file = fopen(outputPath, "a");
    if (file == NULL) {
        fprintf(stderr,"Could not open file for writing.\n");
        exit(1);
    }
    fprintf(file, "\n%s", code);
    fclose(file);
    printf("Wrote code into %s\n", outputPath);
    free(code);
}

char* GetAssetName(char* path) {
    // replace backslashes if any
    for (char* p = path; *p != '\0'; p++) {
        if (*p == '\\') {
            *p = '/';
        }
    }

    // get filestem
    char* stem = strrchr(path, '/');
    if (stem == NULL){
        stem = path;
    } else {
        stem++;
    }

    char* dot = strrchr(stem, '.');
    if (dot != NULL){
        *dot = '\0'; // cut of string at dot
    }

    stem = strupr(stem);

    return stem;
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

    unsigned char* buffer = (unsigned char*) s_malloc(*size);
    fread(buffer, *size, 1, file);
    fclose(file);

    return buffer;
}


//sprintf(hex, "0x%02X", bytes[i]); (replaces this slower call)
char HexLookupTable[] = "0123456789ABCDEF";
inline void UnsignedByteToHex(char* hex, unsigned char val){
    hex[0] = HexLookupTable[(val >> 4) & 0x0f];
    hex[1] = HexLookupTable[val & 0x0f];
    hex[2] = '\0'; 
    return hex[0];
}

void EmbedFile(char* file, char** genCode) {

    // read raw binary
    size_t size = 0;
    unsigned char* bytes = LoadFileBytes(file, &size);
    assert(size > 0);

    char* fileName = strdup(file);
    char* assetName = GetAssetName(file);
    printf("Embedding file %s named %s\n", fileName, assetName);
    free(fileName);

    char* code = s_malloc(size * 8 + 1024);

    clock_t startTime = clock();

    // write code header
    sprintf(code, "static unsigned int %s[%d] = {\n", assetName, size);

    // write each pixel after
    char hex[3];
    for (int i = 0; i < size; i++) {
        UnsignedByteToHex(hex,bytes[i]);
        strcat(code, "0x");
        strcat(code, hex);
        strcat(code, ",");
    }

    free(bytes);

    // end code block
    strcat(code, "\n};");

    *genCode = code;

    clock_t endTime = clock();
    double cpuTimeUsed = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;
    printf("%f secs...\n", cpuTimeUsed);
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

    char** files = NULL;
    GetFolderFiles(folder, ".png", &files);

    for (int i = 0; files[i] != NULL; i++) {
        char* code = NULL;
        EmbedFile(files[i], &code);
        WriteCode(outputFile, code);
        free(files[i]);
    }
    free(files);
}

#ifdef DEBUG
bool UnitTest() {
    assert(strcmp(GetAssetName("assets/spr_player_fox.png"), "SPR_PLAYER_FOX") == 0);
    assert(strcmp(GetAssetName("assets\\mus_overworld.ogg"), "MUS_OVERWORLD") == 0);

    printf("Completed unit testing...\n");
    return true;
}
#endif

int main(int argc, char** argv) {
#ifdef DEBUG
    assert(UnitTest());
#endif

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
        *files = (char**)s_malloc((MAX_FILE_COUNT + 1) * sizeof(char*));

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
