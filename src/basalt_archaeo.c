#include "basalt.h"

#define MAX_DRAW_CALLS 128

static bool IsRunning = false;
static Color* TargetTextureMemory = NULL;
static uint DrawCallIndex = 0;
static Texture DrawCallTextures[MAX_DRAW_CALLS] = { 0 };

pubfunc void DrawCallImpl(Texture canvas, const char* desc)
{
    if (TargetTextureMemory == NULL || TargetTextureMemory != canvas.pixels)
        return;

    if (DrawCallIndex < MAX_DRAW_CALLS)
    {
        int i = DrawCallIndex++;
        if (DrawCallTextures[i].pixels == NULL)
            DrawCallTextures[i] = InitTexture(canvas.width, canvas.height);

        CopyTextureInto(DrawCallTextures[i], canvas);
    }
    else
    {
        WARN("Too many draw calls to register!");
    }
}

func void ClearDrawCalls()
{
    DrawCallIndex = 0;
}

pubfunc bool UpdateAndRenderArchaeo(Texture canvas)
{
    if (TargetTextureMemory == NULL)
        TargetTextureMemory = canvas.pixels;

    // TODO: Use actual keybindings
    if (IsMouseDown())
    {
        IsRunning = true;
    }

    if (IsRunning)
    {
        return false;
    }

    ClearDrawCalls();

    return true;
}
