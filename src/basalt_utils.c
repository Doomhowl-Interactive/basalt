#include "basalt.h"

#ifdef WASM

// TODO: naive solution at the moment
// virtual heap (needed for WASM, but might work fine on other platforms)
static uchar HeapMemory[HEAP_SIZE] = { 0 };
static uint HeapLength = 0;

void* malloc(uint size){
    void* ptr = NULL;
    if (HeapLength + size < HEAP_SIZE){
        ptr = (void*) &HeapMemory[HeapLength];
        HeapLength += size;
    } else {
        ERROR("HEAP FULL: NO SIZE LEFT!");
    }
    return ptr;
}

void free(void* ptr){
     // TODO: not yet implemented, shhhh
}

void memcpy(void* dest, const void* src, uint size){
    char* destPtr = (char*) dest;
    const char* srcPtr = (char*) src;

    for (uint i = 0; i < size; i++) {
        destPtr[i] = srcPtr[i];
    }
}

#endif

// memory condoms
void* MemAlloc(uint size) {
    void* mem = malloc(size);
    Assert(mem);
    return mem;
}

void* MemAllocEx(uint size, uint amount) {
    uint totalSize = size * amount;
    return MemAlloc(totalSize);
}

void MemFree(void* ptr) {
    if (ptr != NULL) {
        free(ptr);
    }
}

bool IsLittleEndian(){
    volatile uint i = 0x01234567;
    bool littleEndian = *((uchar*)(&i)) == 0x67;
    return littleEndian;
}

void AssertImpl(bool cond){
#ifdef DEBUG
    if (!cond) {
        int* val = NULL;
        *val = 666;
    }
#endif
}

void Panic(const char* msg) {

}
