#ifndef BASALT_H
#define BASALT_H

#ifndef __cplusplus
typedef int bool;
#endif

typedef unsigned int uint;
typedef unsigned char uchar;

#ifndef NULL
#define NULL    0
#endif

#define true    1
#define false   0

#ifndef RELEASE
#define Assert(X) AssertImpl(X)
void AssertImpl(bool cond);
void CheckTypes();
#else
#define Assert(X)
#endif

bool IsLittleEndian();

typedef struct {
    int X;
    int Y;
    int Width;
    int Height;
} rect;

typedef struct {
    float X;
    float Y;
    float Width;
    float Height;
} rectf;

typedef struct {
    int X;
    int Y;
} vec2;

typedef struct {
    float X;
    float Y;
} vec2f;

#endif
