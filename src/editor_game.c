#include "basalt_extra.h"

#define BG_COLOR 0x69B7E0
#define GRID_COLOR 0x000000
#define GRID_CELL_COLOR 0x202000

const uint WIDTH = 1280;
const uint HEIGHT = 720;

typedef Vec2 Camera;

static struct {
    Camera camera;
    uint gridSize;
} Editor;

func void DrawSelectedGridCell(Texture canvas, Camera camera, int32 color, int gridSize, int thick) {
    Point mouse = GetMousePosition();
    int x = (int)((int)camera.x % gridSize + mouse.x / gridSize) * gridSize;
    int y = (int)((int)camera.y % gridSize + mouse.y / gridSize) * gridSize;
    DrawRectangleLines(canvas, x, y, gridSize, gridSize, thick, color);
}

func void DrawGrid(Texture canvas, Camera camera, int32 color, int gridSize, int thick) {
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

pubfunc void InitializeGame() {
    Editor.camera = (Camera){0, 0};
    Editor.gridSize = 16;
}

pubfunc void DisposeGame() {
    WARN("TODO: dispose game properly");
}

pubfunc void UpdateAndRenderGame(Texture canvas, float delta) {
    ClearTexture(canvas, BG_COLOR);
    DrawGrid(canvas, Editor.camera, GRID_COLOR, Editor.gridSize, 1);
    DrawSelectedGridCell(canvas, Editor.camera, GRID_CELL_COLOR, Editor.gridSize, 2);
}
