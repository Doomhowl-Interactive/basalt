#include "basalt_extra.h"

const uint WIDTH = 1280;
const uint HEIGHT = 720;

#define BG_COLOR 0x69B7E0

void InitializeGame() {}

void DisposeGame() { WARN("TODO: dispose game properly"); }

void UpdateAndRenderGame(Texture canvas, float delta) {
    ClearTexture(canvas, BG_COLOR);
}
