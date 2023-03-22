#include "basalt.h"
#include "basalt_extra.h"

#define WASM_STDLIB_IMPLEMENTATION
#include "../src/wasm_stdlib.h"

#include "../src/basalt_assets.c"
#include "../src/basalt_graphics.c"
#include "../src/basalt_extra_graphics.c"

#include "../src/assets_custom.dat.c"
#include "../src/temple_game.c"

Texture Canvas = {0};
int FrameNumber = 0;

wasmfunc bool InitWASM() {
    Canvas.width = WIDTH;
    Canvas.height = HEIGHT;
    Canvas.pixels = malloc(WIDTH*HEIGHT*sizeof(int));
    return true;
}

wasmfunc int GetWASMCanvasWidth() {
    return Canvas.width;
}

wasmfunc int GetWASMCanvasHeight() {
    return Canvas.height;
}

wasmfunc uchar* GetWASMCanvas() {
    return (uchar*) Canvas.pixels;
}

wasmfunc int UpdateAndRenderWASM(float delta) {
    UpdateAndRenderGame(Canvas, delta);

    // set transparancy always to 255
    uchar* comps = (uchar*) Canvas.pixels;
    for (int i = 0; i < 4*WIDTH*HEIGHT; i+=4) {
        comps[i+3] = 255;
    }

    return FrameNumber++;
}

wasmfunc float PollWASMMousePosition(float mouseX, float mouseY){
    return mouseX * mouseY;
}

wasmfunc int LifeAndTheUniverse(){
    int a = 42;
    return a;
}
