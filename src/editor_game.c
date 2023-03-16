#include "basalt_extra.h"

#define BG_COLOR 0x69B7E0
#define GRID_COLOR 0x000000

const uint WIDTH = 1280;
const uint HEIGHT = 720;

typedef Vec2 Camera;

static struct {
    Camera camera;
    uint gridSize;
} Editor;

static void DrawGrid(Texture canvas, Camera camera, int32 color, int gridSize,
                     int thick) {
    Assert(gridSize > 0);

    int startX = (int)camera.x % gridSize;
    int startY = (int)camera.y % gridSize;

    int endX = canvas.width - (int)camera.x % gridSize;
    int endY = canvas.height - (int)camera.y % gridSize;

    Assert(startX < endX);
    Assert(startY < endY);

    for (int y = startY; y < endY; y += gridSize) {
        for (int x = startX; x < endX; x += gridSize) {
            DrawDot(canvas, x, y, thick, color);
        }
    }
}

void InitializeGame() {
    Editor.camera = (Camera) { 0, 0 };
    Editor.gridSize = 16;
}

void DisposeGame() { WARN("TODO: dispose game properly"); }

void UpdateAndRenderGame(Texture canvas, float delta) {
    ClearTexture(canvas, BG_COLOR);
    DrawGrid(canvas, Editor.camera, GRID_COLOR, Editor.gridSize, 1);
}
