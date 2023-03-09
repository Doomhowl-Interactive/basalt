#include "basalt.h"

#ifndef RELEASE
void AssertImpl(bool cond) { // call with Assert() macro
    if (!cond) {
        int* i = NULL;
        *i = 666;
    }
}

void CheckTypes() {
    Assert(sizeof(int)    == 4);
    Assert(sizeof(long)   == 4);
    Assert(sizeof(char)   == 1);
    Assert(sizeof(float)  == 4);
    Assert(sizeof(double) == 8);
}
#endif

bool IsLittleEndian(){
    volatile uint i = 0x01234567;
    bool littleEndian = *((uchar*)(&i)) == 0x67;
    return littleEndian;
}
