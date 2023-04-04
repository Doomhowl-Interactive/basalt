#include "basalt.h"

#ifdef ARCHAEO

#define MAX_DRAW_CALLS 128

static bool IsRunning = false;
static int SelectedIndex = 0;

static Color* TargetTextureMemory = NULL;
static uint DrawCallCount = 0;
static Texture DrawCallTextures[MAX_DRAW_CALLS] = { 0 };

#endif

pubfunc void DrawCallImpl(Texture canvas, const char* desc)
{
#ifdef ARCHAEO
    if (TargetTextureMemory == NULL || TargetTextureMemory != canvas.pixels)
        return;

    if (DrawCallCount < MAX_DRAW_CALLS)
    {
        int i = DrawCallCount++;
        if (DrawCallTextures[i].pixels == NULL)
            DrawCallTextures[i] = InitTexture(canvas.width, canvas.height);

        CopyTextureInto(DrawCallTextures[i], canvas);
    }
#endif
}

pubfunc bool UpdateAndRenderArchaeo(Texture canvas)
{
#ifdef ARCHAEO
    if (TargetTextureMemory == NULL)
        TargetTextureMemory = canvas.pixels;

    if (IsKeyPressed(KEY_P))
    {
        IsRunning = !IsRunning;
        if (IsRunning)
        {
            SelectedIndex = DrawCallCount-1;
        }
    }

    if (IsRunning)
    {
        char title[128];
        sprintf(title, "ARCHAEO MODE - %d/%d",SelectedIndex, DrawCallCount-1);
        SetWindowTitle(title);

        if (IsKeyDown(KEY_O))
            IsRunning = false;

        if (IsKeyPressed(KEY_K))
            SelectedIndex++;
        if (IsKeyPressed(KEY_J))
            SelectedIndex--;

        if (SelectedIndex < 0) SelectedIndex = DrawCallCount-1;
        SelectedIndex = SelectedIndex % DrawCallCount;

        int bytesToCopy = canvas.width * canvas.height * sizeof(Color);
        memcpy(canvas.pixels, DrawCallTextures[SelectedIndex].pixels, bytesToCopy);
        return false;
    }

    // clear drawcalls
    DrawCallCount = 0;
#endif
    return true;
}
