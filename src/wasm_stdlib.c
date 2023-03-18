#include "basalt_stdlib.h"

void AssertImpl(bool cond) {
#ifdef BASALT_DEBUG
    if (!cond) {
        int* i = NULL;
        *i = 666;
    }
#endif
}

void Panic(char* msg, ...) {
    PrintEln(msg);
#ifdef BASALT_DEBUG
    int* i = NULL;
    *i = 666;
#endif
}

void *MemCopy(void *dest, const void* src, usize n) {
    char* destChar = (char*) dest;
    char* srcChar = (char*) src;

    for (usize i = 0; i < n; i++){
        destChar[i] = srcChar[i];
    }
    return dest;
}

void *MemSet(void *str, int c, usize n) {
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

void* MemAlloc(usize size) {
    if(HEAP_SIZE - HeapCurrent >= size){
        HeapCurrent += size;
        return HeapMemory[HeapCurrent - size];
    }

    Panic("Virtual heap can't allocate %uz bytes!", size);
    return NULL;
}

void* MemAllocEx(usize size, usize amount) {
    return MemAlloc(size*amount);
}

void MemFree(void* ptr) {
    // TODO: free is not yet implemented, now pray you don't run out of RAM.
}

void* MemRealloc(void* ptr, usize size) {
    void* copy = MemAlloc(size);
    if (ptr != NULL){
        MemCopy(copy, ptr, size);
        MemFree(ptr); // free original memory
    }
    return copy;
}

void Exit(int status) {
    Panic("Exited with code %d!", status);
}
