#include <string.h>

#include "basalt.h"

#define MAX_DRAW_CALLS 128

typedef enum ArchaeoState { ARCHAEO_IDLE, ARCHAEO_RECORDING, ARCHAEO_OPENED } ArchaeoState;

typedef struct DrawCall {
    const char* desc;
    Texture texture;
} DrawCall;

typedef struct SArchaeo {
    ArchaeoState state;
    int selectedIndex;

    Color* canvasPixels;
    uint drawCallCount;
    DrawCall drawCalls[MAX_DRAW_CALLS];
} SArchaeo;
static SArchaeo Archaeo = { 0 };

BASALT void DrawCallImpl(Texture canvas, const char* desc)
{
    // Should this draw call be recorded?
    if (!Config.hasArchaeo || Archaeo.state != ARCHAEO_RECORDING || Archaeo.canvasPixels == NULL
        || Archaeo.canvasPixels != canvas.pixels)
        return;

    if (Archaeo.drawCallCount < MAX_DRAW_CALLS) {
        int i = Archaeo.drawCallCount++;
        DrawCall* drawCall = &Archaeo.drawCalls[i];
        if (drawCall->texture.pixels == NULL) {
            drawCall->texture = InitTexture(canvas.width, canvas.height);
        }

        // Record this draw call
        drawCall->desc = desc;
        CopyTextureInto(drawCall->texture, canvas);
    }
}

BASALT bool UpdateAndRenderArchaeo(Texture canvas)
{
    if (!Config.hasArchaeo)
        return true;

    Archaeo.canvasPixels = canvas.pixels;

    switch (Archaeo.state) {
        case ARCHAEO_IDLE: {
            if (IsKeyPressed(KEY_P)) {
                Archaeo.drawCallCount = 0;
                Archaeo.state = ARCHAEO_RECORDING;

                // Change window title
                SetWindowTitle("ARCHAEO MODE - RECORDING...");
            }
            break;
        }
        case ARCHAEO_RECORDING: {
            Archaeo.state = ARCHAEO_OPENED;
            Archaeo.selectedIndex = Archaeo.drawCallCount - 1;
            break;
        }
        case ARCHAEO_OPENED: {
            const char* title = FormatText("ARCHAEO MODE - %d/%d", Archaeo.selectedIndex, Archaeo.drawCallCount - 1);
            SetWindowTitle(title);

            if (IsKeyPressed(KEY_P))
                Archaeo.state = ARCHAEO_IDLE;

            if (IsKeyPressed(KEY_K))
                Archaeo.selectedIndex++;
            if (IsKeyPressed(KEY_J))
                Archaeo.selectedIndex--;

            assert(Archaeo.drawCallCount > 0);
            if (Archaeo.selectedIndex < 0) {
                Archaeo.selectedIndex = Archaeo.drawCallCount - 1;
            }
            Archaeo.selectedIndex = Archaeo.selectedIndex % Archaeo.drawCallCount;

            DrawCall* drawCall = &Archaeo.drawCalls[Archaeo.selectedIndex];
            int bytesToCopy = canvas.width * canvas.height * sizeof(Color);
            memcpy(canvas.pixels, drawCall->texture.pixels, bytesToCopy);
            return false;
        }
        default:
            assert(0);
    }
    return true;
}
