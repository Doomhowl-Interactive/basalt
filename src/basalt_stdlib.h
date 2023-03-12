// Reverse-Engineering of the C Standard Library 
// ==========================================
// Does not contain everything, just the things I need
// ==========================================

#ifdef __linux__ 
#define LINUX
typedef unsigned long size_t;

#elif defined(_WIN32) || defined(_WIN64)
#define WINDOWS

#include <windows.h>

#else
#define UNKNOWN
// TODO: not implemented
#endif

#ifndef NULL
#define NULL    0
#endif

#ifndef __cplusplus
typedef int bool;
#define true    1
#define false   0
#endif

#undef assert
void assert(bool cond);
void panic(char* msg);

void *memcpy(void *dest, const void * src, size_t n);
void *memset(void *str, int c, size_t n);

void* malloc(size_t size);
void free(void* ptr);
void* realloc(void* ptr, size_t size);

void exit(int status);

int abs(int x);

#ifdef BASALT_STDLIB_IMPLEMENTATION

void assert(bool cond) {
    if (!cond) {
        int* i = NULL;
        *i = 666;
    }
}

void panic(char* msg) {
    int* i = NULL;
    *i = 666;
}

void *memcpy(void *dest, const void* src, size_t n) {
    char* destChar = (char*) dest;
    char* srcChar = (char*) src;

    for (size_t i = 0; i < n; i++){
        destChar[i] = srcChar[i];
    }
    return dest;
}

void *memset(void *str, int c, size_t n) {
    unsigned char val = (unsigned char) c;
    char* data = (char*) str;
    for (size_t i = 0; i < n; i++){
        data[i] = val;
    }
    return str;
}

void* malloc(size_t size) {
#ifdef WINDOWS
    return VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE);
#endif
}

void free(void* ptr) {
#ifdef WINDOWS
    VirtualFree(ptr, 0, MEM_RELEASE);
#endif
}

void* realloc(void* ptr, size_t size) {
    void* copy = malloc(size);
    if (ptr != NULL){
        memcpy(copy, ptr, size);
        free(ptr); // free original memory
    }
    return copy;
}

#ifdef WINDOWS
void exit(int status) {
    ExitProcess(status);
}
#endif

int abs(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

#endif
