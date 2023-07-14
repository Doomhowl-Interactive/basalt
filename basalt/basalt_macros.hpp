#pragma once

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

// Ergonomic macros
#define V2(V) V.x, V.y
#define _V2(V) V->x, V->y
#define P2(P) P.x, P.y
#define _P2(P) P->x, P->y
#define R2(R) R.x, R.y, R.width, R.height
#define _R2(R) R.x, R.y, R.width, R.height