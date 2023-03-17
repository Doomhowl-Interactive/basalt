#include "basalt_stdlib.h"

void AssertImpl(bool cond) {
    if (!cond) {
        int* i = NULL;
        *i = 666;
    }
}

void Panic(char* msg, ...) {
    int* i = NULL;
    *i = 666;
}

void *MemCopy(void *dest, const void* src, size_t n) {
    char* destChar = (char*) dest;
    char* srcChar = (char*) src;

    for (size_t i = 0; i < n; i++){
        destChar[i] = srcChar[i];
    }
    return dest;
}

void *MemSet(void *str, int c, size_t n) {
    unsigned char val = (unsigned char) c;
    char* data = (char*) str;
    for (size_t i = 0; i < n; i++){
        data[i] = val;
    }
    return str;
}

// Abuse the stack to create a 'virtual heap'
// Ment for the WASM platform, but might work just fine on other platforms.
// WARN: Do not declare these two variables as static! WASM will not find them.
uchar* HeapMemory[HEAP_SIZE];
size_t HeapCurrent = 0;

void* MemAlloc(size_t size) {
    if(HEAP_SIZE - HeapCurrent >= size){
        HeapCurrent += size;
        return HeapMemory[HeapCurrent - size];
    }

    Panic("Virtual heap can't allocate %uz bytes!", size);
    return NULL;
}

void* MemAllocEx(size_t size, size_t amount) {
    return MemAlloc(size*amount);
}

void MemFree(void* ptr) {
    // TODO: free is not yet implemented, now pray you don't run out of RAM.
}

void* MemRealloc(void* ptr, size_t size) {
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

int Absolute(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

bool IsLittleEndian(){
    volatile uint i = 0x01234567;
    bool littleEndian = *((uchar*)(&i)) == 0x67;
    return littleEndian;
}
