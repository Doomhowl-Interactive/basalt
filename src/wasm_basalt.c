#include "basalt.h"
#include "basalt_extra.h"

#include "../src/basalt_utils.c"
#include "../src/basalt_assets.c"
#include "../src/basalt_graphics.c"
#include "../src/basalt_extra_graphics.c"

#include "../src/assets_custom.dat.c"
#include "../src/temple_game.c"

static Texture Canvas = {0};
static int FrameNumber = 0;

static uint Pixels[WIDTH*HEIGHT];

bool InitWASM() {
    Canvas.width = WIDTH;
    Canvas.height = HEIGHT;
    Canvas.pixels = Pixels;
    return true;
}

int GetWASMCanvasWidth() {
    return Canvas.width;
}

int GetWASMCanvasHeight() {
    return Canvas.height;
}

uchar* GetWASMCanvas() {
    return (uchar*) Canvas.pixels;
}

int UpdateAndRenderWASM(float delta) {
    UpdateAndRenderGame(Canvas, delta);

    // set transparancy always to 255
    uchar* comps = (uchar*) Canvas.pixels;
    for (int i = 0; i < 4*WIDTH*HEIGHT; i+=4) {
        comps[i+3] = 255;
    }

    // test 
    for (int i = 0; i < 4*WIDTH*HEIGHT; i++) {
        // comps[i] = 40;
    }

    return FrameNumber++;
}

float PollWASMMousePosition(float mouseX, float mouseY){
    return mouseX * mouseY;
}

int LifeAndTheUniverse(){
    int a = 42;
    return a;
}

