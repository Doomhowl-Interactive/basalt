#include "basalt.h"
#include "basalt_extra.h"

static Texture Canvas;

void InitWASM() {
    Canvas = InitTexture(WIDTH, HEIGHT);
}

int GetWASMCanvasWidth() {
    return Canvas.width;
}

int GetWASMCanvasHeight() {
    return Canvas.height;
}

void* GetWASMCanvas() {
    return (void*) &Canvas;
}

void UpdateAndRenderWASM(float delta){

}

float PollWASMMousePosition(float mouseX, float mouseY){
    return mouseX * mouseY;
}

int LifeAndTheUniverse(){
    int a = 42;
    return a;
}

