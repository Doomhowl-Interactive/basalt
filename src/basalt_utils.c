#include "basalt.h"

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
