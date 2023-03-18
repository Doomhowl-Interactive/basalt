#ifndef BASALT_STDLIB_H
#define BASALT_STDLIB_H

// Custom interface of the C Standard Library 
// ==========================================

#ifndef NULL
#define NULL    0
#endif

// ==========================================

#ifdef WASM
#define HEAP_SIZE 500*1000
#define MAX_ALLOCATIONS 500
#if !defined(__cplusplus)
typedef int bool;
#define true    1
#define false   0
#endif
#else
#include <stdbool.h>
#endif

// ==========================================

#define MAX(X,Y) (X > Y ? X:Y)
#define MIN(X,Y) (X < Y ? X:Y)

typedef unsigned long size_t;
typedef unsigned int uint;
typedef unsigned char uchar;

void Panic(char* msg, ...);

#define Assert(X) AssertImpl((bool)X)
void AssertImpl(bool cond);

void *MemCopy(void *dest, const void * src, size_t n);
void *MemSet(void *str, int c, size_t n);

void* MemAlloc(size_t size);
void* MemAllocEx(size_t size, size_t amount);
void* MemRealloc(void* ptr, size_t size);
void MemFree(void* ptr);

void PrintF(const char* format, ...);
void PrintLn(const char* format, ...);
void PrintE(const char* format, ...);
void PrintEln(const char* format, ...);

void Exit(int status);

inline int Absolute(int x){
    if (x < 0) {
        return -x;
    }
    return x;
}

#endif
