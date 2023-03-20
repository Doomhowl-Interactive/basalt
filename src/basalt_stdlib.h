// TODO: Find a way to ditch this for WASM.
// It will be way easier to just rely on POSIX goodness.

#ifndef BASALT_STDLIB_H
#define BASALT_STDLIB_H

// Easy defines to check operating system
#ifdef __linux__ 
#define LINUX
#elif defined(_WIN32) || defined(_WIN64)
#define WINDOWS
#else
#define UNKNOWN
#endif

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

typedef unsigned long usize;
typedef unsigned int uint;
typedef unsigned char uchar;

void Panic(char* msg, ...);

void *MemCopy(void *dest, const void * src, usize n);
void *MemSet(void *str, int c, usize n);

void* MemAlloc(usize size);
void* MemAllocEx(usize size, usize amount);
void* MemRealloc(void* ptr, usize size);
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
