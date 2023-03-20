#include "basalt.h"

#ifdef BASALT_DEBUG

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
    assert(val == 1770);
}

func void CheckAllocation() {

}

func void TestMath(){
    assert(MIN(5,3) == 3);
    assert(MAX(5,3) == 5);
}

// NOTE: I'm terrible at calculating hexes and binary,
// so this is a little sandbox to make sure I don't go insane.
func void TestColors(){
    Color alpha = 0xFF000000;
    assert(alpha == CreateColorA(0,0,0,255));

    Color red = 0xFFFF0000;
    assert(red == CreateColor(255,0,0));

    Color green = 0xFF00FF00;
    assert(green == CreateColor(0,255,0));

    Color blue = 0xFF0000FF;
    assert(blue == CreateColor(0,0,255));

    Color left = CreateColorA(255,50,50,255);
    assert((left & 0xFF000000) == 0xFF000000);
}

pubfunc void UnitTest() {
    CheckByteReading();
    TestMath();
    TestColors();
    CheckAllocation();
}

#endif
