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

pubfunc Vec2 Vec2Normalize(Vec2 v2)
{
    float mag = Vec2Magnitude(v2);

    Vec2 norm;
    norm.x = v2.x / mag;
    norm.y = v2.y / mag;
    return norm;
}

pubfunc float Vec2Magnitude(Vec2 v2)
{
    // Pythagorean theorem
    return sqrt(v2.x*v2.x + v2.y*v2.y);
}
