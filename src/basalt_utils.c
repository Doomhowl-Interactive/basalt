#include "basalt.h"

#ifdef DEBUG
void AssertImpl(bool cond) { // call with Assert() macro
    if (!cond) {
        int* i = NULL;
        *i = 666;
    }
}

void PanicImpl(char* msg) { // call with Panic() macro
    int* i = NULL;
    *i = 606;
}
#endif

bool IsLittleEndian(){
    volatile uint i = 0x01234567;
    bool littleEndian = *((uchar*)(&i)) == 0x67;
    return littleEndian;
}
