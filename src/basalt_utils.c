#include "basalt.h"

pubfunc bool IsLittleEndian() {
    volatile uint i = 0x01234567;
    bool littleEndian = *((uchar*)(&i)) == 0x67;
    return littleEndian;
}

pubfunc int Clamp(int value, int min, int max) {
    if (value < min) {
        return min;
    }

    if (value > max) {
        return max;
    }

    return value;
}
