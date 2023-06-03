#include "basalt.h"
#define LAUNCHER

func float wave(float offset)
{
    return (sinf(offset) + 1.f) / 2.f;
}

LAUNCHER void DrawColoredGrid(Texture canvas)
{
    float perc = wave(GetTimeElapsed());
    uchar r = 255 - perc * 50;
    uchar b = 150 - perc * 50;

    const int gridCount = 10;
    const int gridsW = canvas.width / gridCount;
    const int gridsH = canvas.height / gridCount;
    for (int y = 0; y < gridCount; y++) {
        for (int x = 0; x < gridCount; x++) {
            int xx = x * gridsW;
            int yy = y * gridsH;
            if ((x + y) % 2 == 0) {
                DrawRectangle(canvas, xx, yy, gridsW, gridsH, RGBA(r, 100, b, 255));
            } else {
                DrawRectangle(canvas, xx, yy, gridsW, gridsH, RGBA(r, 150, b + 40, 255));
            }
        }
    }
}