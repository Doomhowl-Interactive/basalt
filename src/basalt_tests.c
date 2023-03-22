#include "basalt.h"

#ifdef BASALT_DEBUG

func void CheckByteReading(){
    // spr block size example
    uchar result[4];
    uint value = 1770;
    result[3] = (uchar)(value >> 24);
    result[2] = (uchar)(value >> 16);
    result[1] = (uchar)(value >> 8);
    result[0] = (uchar)(value >> 0);
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
    Color alpha = 0x000000FF;
    assert(alpha == RGBA(0,0,0,255));

    Color red = 0xFF0000FF;
    assert(red == RGB(255,0,0));

    Color green = 0x00FF00FF;
    assert(green == RGB(0,255,0));

    Color blue = 0x0000FFFF;
    assert(blue == RGB(0,0,255));

    Color left = RGBA(255,50,50,255);
    assert((left & 0x000000FF) == 0x000000FF);

    Color purple = RGBA(51, 51, 153, 255);
    assert(purple == 0x333399FF);
}

pubfunc void UnitTest() {
    CheckByteReading();
    TestMath();
    TestColors();
    CheckAllocation();
}

#endif
