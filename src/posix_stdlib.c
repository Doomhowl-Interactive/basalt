#include "basalt_stdlib.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void AssertImpl(bool cond) {
#ifdef BASALT_DEBUG
    assert(cond);
#endif
}

void Panic(char* msg, ...) {
    PrintEln(msg);
#ifdef BASALT_DEBUG
    Exit(1);
#endif
}

void *MemCopy(void *dest, const void* src, size_t n) {
    return memcpy(dest, src, n);
}

void *MemSet(void *str, int c, size_t n) {
    memset(str,c,n);
}

void* MemAlloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL){
        Panic("Failed to allocate memory!");
    }
    return ptr;
}

void* MemAllocEx(size_t size, size_t amount) {
    return MemAlloc(size*amount);
}

void MemFree(void* ptr) {
    if (ptr != NULL) {
        free(ptr);
    }
}

void* MemRealloc(void* ptr, size_t size) {
    void* moved = realloc(ptr, size);
    if (moved == NULL){
        Panic("Failed to reallocate memory!");
    }
    return moved;
}

void PrintF(const char* format, ...) {
    va_list argPtr;
    va_start(argPtr, format);
    vfprintf(stdout,format,argPtr);
    va_end(argPtr);
}

void PrintLn(const char* format, ...) {
    va_list argPtr;
    va_start(argPtr, format);
    vfprintf(stdout,format,argPtr);
    va_end(argPtr);

    fprintf(stdout, "\n");
}

void PrintE(const char* format, ...) {
    va_list argPtr;
    va_start(argPtr, format);
    vfprintf(stderr,format,argPtr);
    va_end(argPtr);
}

void PrintEln(const char* format, ...) {
    va_list argPtr;
    va_start(argPtr, format);
    vfprintf(stderr,format,argPtr);
    va_end(argPtr);

    fprintf(stderr, "\n");
}

void Exit(int status) {
    DEBUG("Exited with code %d!", status);
    exit(status);
}
