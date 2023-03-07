#include <stdio.h>
#include <string.h>
#include <stdint.h>

#if defined(_WIN64) || defined(_WIN32)
#include "external/dirent.h"
#else
#include <dirent.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

#define true 1
#define false 0

typedef uint32_t uint32;
typedef int bool;

typedef uint8_t ColorComp;
typedef uint32_t Color;

bool WriteCode(const char* outputPath, char* code){
    FILE* file;
    file = fopen(outputPath, "a");
    if (file == NULL){
        printf("Could not open file for writing.\n");
        return false;
    }
    fprintf(file, "\n%s", code);
    fclose(file);
    printf("Wrote code into %s.\n", outputPath);
}

bool EncodeImage(const char* file, char** genCode) {
    printf("Encoding image %s\n", file);

    Color* colors = NULL;
    int width = 0;
    int height = 0;
    if (!GetImageColors(file, &colors, &width, &height)){
        printf("Could not determine image colors\n");
        return false;
    }
    int pixelCount = width * height;

    char* assetName = "SPR_PLAYER_FOX";

    int totalIntegers = pixelCount+2;
    char* code = malloc(sizeof(char) * (totalIntegers * 12 + 1024));

    // write code header
    sprintf(code,"static unsigned int %s[%d] = {\n",assetName,totalIntegers);

    // write width and height first
    char hex[12];
    sprintf(hex, "0x%08X", width);
    strcat(code, hex);
    strcat(code, ",");
    sprintf(hex, "0x%08X", height);
    strcat(code, hex);
    strcat(code, ",");

    // write each pixel after
    for (int i = 0; i < pixelCount; i++){
        sprintf(hex, "0x%08X", colors[i]);
        strcat(code, hex);
        strcat(code, ",");
    }

    // end code block
    strcat(code, "\n};");

    *genCode = code;
    return true;
}

bool GetImageColors(const char* file, Color** pixels, int* width, int* height) {

    int channels = 0;
    int w, h;
    ColorComp* comps = stbi_load(file, &w, &h, &channels, STBI_rgb_alpha);
    if (comps == NULL){
        printf("Failed to encode image %s!", file);
        return false;
    }

    *pixels = (Color*) comps;
    *width = w;
    *height = h;

    // TODO: call later stbi_image_free(comps);
    return true;
}

void EncodeFolder(const char* folder, const char* outputFile) {
    char** files = NULL;
    if (!GetFolderFiles(folder,".png", &files)) {
        printf("Failed to read folder %s", folder);
    }

    for (int i = 0; files[i] != NULL; i++) {
        char** code = NULL;
        EncodeImage(files[i],&code);
        WriteCode(outputFile, code);
    }
}

int main(int argc, char** argv){
    EncodeFolder("../src","../src/assets_custom.dat.c");
    printf("Done");
}

#define MAX_PATH_LENGTH 128
#define MAX_FILE_COUNT 512
static bool GetFolderFiles(const char* folder, const char* ext, char*** files){

    DIR *dir;
    struct dirent *ent;
    int extLen = strlen(ext);
    if ((dir = opendir(folder)) != NULL) {

        // list
        *files = (char**) malloc((MAX_FILE_COUNT+1) * sizeof(char*));

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
    } else {
        perror("Unable to open directory");
        (*files) = NULL;
        return false;
    }
    return true;
}
