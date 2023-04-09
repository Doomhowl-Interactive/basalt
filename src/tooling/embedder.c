#define BASALT_NO_ENGINE
#include "../basalt.h"
#include "../basalt_utils.c"

#include <ctype.h>
#include <time.h>

#if defined(_WIN64) || defined(_WIN32)
#include "../external/dirent.h"
#else
#include <dirent.h>
#endif

void WriteCode(char* outputPath, String code) {
    FILE* file;
    file = fopen(outputPath, "a");
    if (file == NULL) {
        fprintf(stderr,"Could not open file for writing.\n");
        exit(EXIT_FAILURE);
    }

    // append code to disk file
    fprintf(file, "\n%s", code.text);
    fclose(file);
    printf("Wrote code into %s\n", outputPath);

    UnloadString(&code);
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
        exit(EXIT_FAILURE);
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

    // TODO: Messy
    char prefixToPad[128];
    sprintf(prefixToPad, "Embedding file %s",file);

    char suffixToPad[128];
    sprintf(suffixToPad, "as %s",assetName);

    char prefixFormatted[256];
    strcpy(prefixFormatted, PadStringRight(prefixToPad,'.',80));

    printf("%s %s",prefixFormatted,PadStringRight(suffixToPad,'.',40));

    clock_t startTime = clock();

    // write code header
    {
        char header[64];
        sprintf(header, "unsigned char %s[] = {\n", assetName, size);
        AppendString(code, header);
    }

    // add size marker (4 bytes) == LITTLE ENDIAN
    {
        char sizeText[64];
        unsigned char s[4];
        s[0] = (unsigned char)(size >>  0);
        s[1] = (unsigned char)(size >>  8);
        s[2] = (unsigned char)(size >> 16);
        s[3] = (unsigned char)(size >> 24);
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
    printf(" %fs\n", cpuTimeUsed);
}

void ClearFile(char* outputFile) {
    FILE* file = fopen(outputFile, "w");
    if (file != NULL) {
        fputs("", file);
        fclose(file);
    }
}

void EncodeFolder(char* folder, char* outputFile) {
    ClearFile(outputFile);

    String code = MakeString();
    FilePathList files = GetFolderFiles(folder, ".png");

    for (int i = 0; i < files.count; i++) {
        AppendFileCode(&code, files.strings[i]);
    }

    WriteCode(outputFile, code);
    UnloadFilePathList(files);
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
