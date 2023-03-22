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

void StringToUppercase(char* str) {
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

    StringToUppercase(dest);
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

char* AppendString(char* dest, char* add){
    strcpy(dest, add);
    dest += strlen(add);
    return dest;
}

void EmbedFile(char* file, char** genCode) {

    // read raw binary
    size_t size = 0;
    unsigned char* bytes = LoadFileBytes(file, &size);
    assert(size > 0);

    char* fileName = strdup(file);
    char assetName[128]; GetAssetName(assetName, file);
    printf("Embedding file %s named %s ", fileName, assetName);
    free(fileName);

    size_t codeMaxSize = size * 8 + 1024;
    char* code = malloc(codeMaxSize);
    memset(code, '\0', codeMaxSize);
    char* marker = code;

    clock_t startTime = clock();

    // write code header
    char header[64];
    sprintf(header, "unsigned char %s[] = {\n", assetName, size);
    marker = AppendString(marker, header);

    // add size marker (4 bytes) == BIG ENDIAN
    char sizeText[64];
    unsigned char s[4];
    s[0] = (unsigned char)(size >> 24);
    s[1] = (unsigned char)(size >> 16);
    s[2] = (unsigned char)(size >>  8);
    s[3] = (unsigned char)(size >>  0);
    sprintf(sizeText,"0x%02X,0x%02X,0x%02X,0x%02X, ",s[0],s[1],s[2],s[3]);
    marker = AppendString(marker, sizeText);

    // write each pixel after
    for (int i = 0; i < size; i++) {
        char hexText[18];
        sprintf(hexText,"0x%02X,", bytes[i]);
        marker = AppendString(marker, hexText);
    }

    free(bytes);

    // end code block
    marker = AppendString(marker, "\n};\0");

    *genCode = code;

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

bool UnitTest() {
    char name1[128];
    GetAssetName(name1, "assets/spr_player_fox.png");
    assert(strcmp(name1, "SPR_PLAYER_FOX") == 0);

    char name2[128];
    GetAssetName(name2, "assets\\mus_overworld.ogg");
    assert(strcmp(name2, "MUS_OVERWORLD") == 0);

    char str[128];
    char* ptr = str;
    ptr = AppendString(ptr, "Hello");
    ptr = AppendString(ptr, " world");
    ptr = AppendString(ptr, "!");
    assert(strcmp(str,"Hello world!") == 0);

    char clone[16];
    strcpy(clone, "Hello");
    StringToUppercase(clone);
    assert(strcmp(clone,"HELLO") == 0);

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
