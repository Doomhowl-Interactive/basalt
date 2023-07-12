#include <string.h>

#include "sdl2_plat.hpp"
#include "basalt.h"
#include "basalt_archaeo.hpp"

using namespace std;

void Archaeo::RegisterDrawCallImpl(Texture canvas, string description)
{
    // Should this draw call be recorded?
    if (!Config.hasArchaeo || state != ARCHAEO_RECORDING || canvasPixels == NULL
        || canvasPixels != canvas.pixels)
        return;

    if (drawCallCount < MAX_DRAW_CALLS) {
        int i = drawCallCount++;
        DrawCall& drawCall = drawCalls[i];
        if (drawCall.texture.pixels == NULL) {
            drawCall.texture = InitTexture(canvas.width, canvas.height);
        }

        // Record this draw call
        drawCall.description = description;
        CopyTextureInto(drawCall.texture, canvas);
    }
}

bool Archaeo::UpdateAndRenderImpl(Texture canvas)
{
    if (!Config.hasArchaeo)
        return true;

    canvasPixels = canvas.pixels;

    switch (state) {
        case ARCHAEO_IDLE: {
            if (IsKeyPressed(KEY_p)) {
                drawCallCount = 0;
                state = ARCHAEO_RECORDING;

                // Change window title
                SetWindowTitle("ARCHAEO MODE - RECORDING...");
            }
            break;
        }
        case ARCHAEO_RECORDING: {
            state = ARCHAEO_OPENED;
            selectedIndex = drawCallCount - 1;
            break;
        }
        case ARCHAEO_OPENED: {
            const char* title = FormatText("ARCHAEO MODE - %d/%d", selectedIndex, drawCallCount - 1);
            SetWindowTitle(title);

            if (IsKeyPressed(KEY_p))
                state = ARCHAEO_IDLE;

            if (IsKeyPressed(KEY_k))
                selectedIndex++;
            if (IsKeyPressed(KEY_j))
                selectedIndex--;

            assert(drawCallCount > 0);
            if (selectedIndex < 0) {
                selectedIndex = drawCallCount - 1;
            }
            selectedIndex = selectedIndex % drawCallCount;

            DrawCall* drawCall = &drawCalls[selectedIndex];
            int bytesToCopy = canvas.width * canvas.height * sizeof(Color);
            memcpy(canvas.pixels, drawCall->texture.pixels, bytesToCopy);
            return false;
        }
        default:
            assert(0);
    }
    return true;
}

inline void Archaeo::RegisterDrawCall(Texture canvas, std::string description)
{
    if (!instance) {
        instance = std::make_shared<Archaeo>();
    }
    instance.get()->RegisterDrawCallImpl(canvas, description);
}
inline bool Archaeo::UpdateAndRender(Texture canvas)
{
    if (!instance) {
        instance = std::make_shared<Archaeo>();
    }
    instance.get()->UpdateAndRenderImpl(canvas);
}