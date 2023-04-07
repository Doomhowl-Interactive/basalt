#ifndef BASALT_STDLIB_H
#define BASALT_STDLIB_H

#ifndef NULL
#define NULL    0
#endif

#define HEAP_SIZE 500*1000
#define MAX_ALLOCATIONS 500

typedef int bool;
#define true    1
#define false   0

// ==========================================

typedef int int32;
typedef unsigned long usize;
typedef unsigned int uint;
typedef unsigned char uchar;

void assert(bool cond);

void *memcpy(void *dest, const void * src, usize n);
void *memset(void *str, int c, usize n);

void* malloc(usize size);
void* realloc(void* ptr, usize size);
void free(void* ptr);

void printf(const char* format, ...);
void fprintf(const char* format, ...);

void exit(int status);

#endif

#ifdef WASM_STDLIB_IMPLEMENTATION

void assert(bool cond) {
    if (!cond) {
        int* i = NULL;
        *i = 666;
    }
}

void *memcpy(void *dest, const void* src, usize n) {
    char* destChar = (char*) dest;
    char* srcChar = (char*) src;

    for (usize i = 0; i < n; i++){
        destChar[i] = srcChar[i];
    }
    return dest;
}

void *memset(void *str, int c, usize n) {
    unsigned char val = (unsigned char) c;
    char* data = (char*) str;
    for (usize i = 0; i < n; i++){
        data[i] = val;
    }
    return str;
}

// Abuse the stack to create a 'virtual heap'
// Ment for the WASM platform, but might work just fine on other platforms.
// WARN: Do not declare these two variables as static! WASM will not find them.
uchar* HeapMemory[HEAP_SIZE];
usize HeapCurrent = 0;

void* malloc(usize size) {
    if(HEAP_SIZE - HeapCurrent >= size){
        HeapCurrent += size;
        return HeapMemory[HeapCurrent - size];
    }

    Panic("Virtual heap can't allocate %uz bytes!", size);
    return NULL;
}

void free(void* ptr) {
    // TODO: free is not yet implemented, now pray you don't run out of RAM.
}

void* realloc(void* ptr, usize size) {
    void* copy = MemAlloc(size);
    if (ptr != NULL){
        MemCopy(copy, ptr, size);
        MemFree(ptr); // free original memory
    }
    return copy;
}

void exit(int status) {
    Panic("Exited with code %d!", status);
}

#endif
