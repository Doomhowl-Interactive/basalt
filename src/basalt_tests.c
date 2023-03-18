#include "basalt.h"

#ifdef BASALT_DEBUG

func void CheckTypes(){
    // check types
    Assert(sizeof(int)    == 4);
    Assert(sizeof(long)   == 4);
    Assert(sizeof(char)   == 1);
    Assert(sizeof(float)  == 4);
    Assert(sizeof(double) == 8);
}

func void CheckByteReading(){
    // spr block size example
    uchar result[4];
    uint value = 1770;
    if (IsLittleEndian()) {
        result[3] = (uchar)(value >> 24);
        result[2] = (uchar)(value >> 16);
        result[1] = (uchar)(value >> 8);
        result[0] = (uchar)(value >> 0);
    }
    else {
        result[0] = (uchar)(value >> 24);
        result[1] = (uchar)(value >> 16);
        result[2] = (uchar)(value >> 8);
        result[3] = (uchar)(value >> 0);
    }
    uint val = *((uint*)result);
    Assert(val == 1770);
}

func void CheckAllocation() {

}

func void TestMath(){
    Assert(MIN(5,3) == 3);
    Assert(MAX(5,3) == 5);
}

pubfunc void UnitTest() {
    CheckTypes();
    CheckByteReading();
    TestMath();
    CheckAllocation();
}

#endif
