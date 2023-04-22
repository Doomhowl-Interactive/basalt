#include "basalt.h"
#include "basalt_extra.h"
#define WASM

Texture Canvas = { 0 };
int FrameNumber = 0;

WASM bool InitWASM()
{
    Canvas.width = WIDTH;
    Canvas.height = HEIGHT;
    Canvas.pixels = malloc(WIDTH * HEIGHT * sizeof(int));
    return true;
}

WASM int GetWASMCanvasWidth()
{
    return Canvas.width;
}

WASM int GetWASMCanvasHeight()
{
    return Canvas.height;
}

WASM uchar* GetWASMCanvas()
{
    return (uchar*)Canvas.pixels;
}

WASM int UpdateAndRenderWASM(float delta)
{
    UpdateAndRenderGame(Canvas, delta);

    // set transparancy always to 255
    uchar* comps = (uchar*)Canvas.pixels;
    for (int i = 0; i < 4 * WIDTH * HEIGHT; i += 4) {
        comps[i + 3] = 255;
    }

    return FrameNumber++;
}

WASM float PollWASMMousePosition(float mouseX, float mouseY)
{
    return mouseX * mouseY;
}

WASM int LifeAndTheUniverse()
{
    int a = 42;
    return a;
}
