#include "basalt_extra.h"

#include "../src/basalt_utils.c"
#include "../src/basalt_assets.c"
#include "../src/basalt_graphics.c"
#include "../src/basalt_extra_graphics.c"

#include "../src/assets_custom.dat.c"
#include "../src/temple_game.c"

Texture Canvas = {0};
uchar Pixels[WIDTH*HEIGHT*4];
static int FrameNumber = 0;

bool InitWASM() {
    Canvas = InitTexture(WIDTH, HEIGHT);
    return true;
}

int GetWASMCanvasWidth() {
    return Canvas.width;
}

int GetWASMCanvasHeight() {
    return Canvas.height;
}

uchar* GetWASMCanvas() {
    return Pixels;
}

int UpdateAndRenderWASM(float delta) {
    UpdateAndRenderGame(Canvas, delta);

    // force on transparancy or you won't see anything in the canvas
    uchar* pixels = (uchar*) Canvas.pixels;
    for (int i = 0; i < Canvas.width * Canvas.height; i ++){
        pixels[i*4 + 3] = 255;
    }

    for (int i = 0; i < Canvas.width * Canvas.height; i ++){
        Pixels[i*4+0] = 69;
        Pixels[i*4+1] = 96;
        Pixels[i*4+2] = 42;
        Pixels[i*4+3] = 42;
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

