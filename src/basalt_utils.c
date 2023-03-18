#include "basalt.h"

bool IsLittleEndian(){
    volatile uint i = 0x01234567;
    bool littleEndian = *((uchar*)(&i)) == 0x67;
    return littleEndian;
}
