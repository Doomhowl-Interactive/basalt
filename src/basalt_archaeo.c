#include <string.h>

#include "basalt.h"

#define MAX_DRAW_CALLS 128

typedef enum ArchaeoState { ARCHAEO_IDLE, ARCHAEO_RECORDING, ARCHAEO_OPENED } ArchaeoState;

static struct {
    ArchaeoState state;
    int selectedIndex;

    Color* canvasPixels;
    uint drawCallCount;
    Texture drawCallTextures[MAX_DRAW_CALLS];
} Archaeo = { 0 };

BASALT void DrawCallImpl(Texture canvas, const char* desc)
{
    // Should this draw call be recorded?
    if (!Config.hasArchaeo || Archaeo.state != ARCHAEO_RECORDING || Archaeo.canvasPixels == NULL
        || Archaeo.canvasPixels != canvas.pixels)
        return;

    if (Archaeo.drawCallCount < MAX_DRAW_CALLS) {
        int i = Archaeo.drawCallCount++;
        if (Archaeo.drawCallTextures[i].pixels == NULL)
            Archaeo.drawCallTextures[i] = InitTexture(canvas.width, canvas.height);

        // Record this draw call
        CopyTextureInto(Archaeo.drawCallTextures[i], canvas);
    }
}

BASALT bool UpdateAndRenderArchaeo(Texture canvas)
{
    if (!Config.hasArchaeo)
        return true;

    Archaeo.canvasPixels = canvas.pixels;

    switch (Archaeo.state) {
        case ARCHAEO_IDLE:
            if (IsKeyPressed(KEY_P)) {
                Archaeo.drawCallCount = 0;
                Archaeo.state = ARCHAEO_RECORDING;

                // Change window title
                char title[128];
                sprintf(title, "ARCHAEO MODE - RECORDING...");
                SetWindowTitle(title);
            }
            break;
        case ARCHAEO_RECORDING:
            Archaeo.state = ARCHAEO_OPENED;
            Archaeo.selectedIndex = Archaeo.drawCallCount - 1;
            break;
        case ARCHAEO_OPENED: {
            char title[128];
            sprintf(title, "ARCHAEO MODE - %d/%d", Archaeo.selectedIndex, Archaeo.drawCallCount - 1);
            SetWindowTitle(title);

            if (IsKeyDown(KEY_P))
                Archaeo.state = ARCHAEO_IDLE;

            if (IsKeyPressed(KEY_K))
                Archaeo.selectedIndex++;
            if (IsKeyPressed(KEY_J))
                Archaeo.selectedIndex--;

            if (Archaeo.selectedIndex < 0)
                Archaeo.selectedIndex = Archaeo.drawCallCount - 1;
            Archaeo.selectedIndex = Archaeo.selectedIndex % Archaeo.drawCallCount;

            int bytesToCopy = canvas.width * canvas.height * sizeof(Color);
            memcpy(canvas.pixels, Archaeo.drawCallTextures[Archaeo.selectedIndex].pixels, bytesToCopy);
            return false;
        } break;
        default:
            assert(0);
    }
    return true;
}
