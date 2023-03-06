#ifndef BASALT_H
#define BASALT_H

#include <stdint.h>

typedef int bool;
#define true    1
#define false   0

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float f32;
typedef double f64;

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
