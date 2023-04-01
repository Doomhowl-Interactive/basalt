#include "basalt.h"

#define MAX_DRAW_CALLS 128

static bool IsRunning = false;
static uint SelectedIndex = 0;

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

    if (IsKeyDown(KEY_P))
    {
        IsRunning = true;
    }

    if (IsRunning)
    {
        char title[128];
        sprintf(title, "ARCHAEO MODE - %d/%d",SelectedIndex, DrawCallIndex);
        SetWindowTitle(title);

        if (IsKeyDown(KEY_O))
            IsRunning = false;

        for (int i = 0; i < 10; i++){
            char key = '0'+i;
            if (IsKeyDown(key) && SelectedIndex != i && i < DrawCallIndex){
                SelectedIndex = i;
                DEBUG("Archaeo showing frame %d", i);
            }
        }

        int bytesToCopy = canvas.width * canvas.height * sizeof(Color);
        memcpy(canvas.pixels, DrawCallTextures[SelectedIndex].pixels, bytesToCopy);
        return false;
    }

    ClearDrawCalls();

    return true;
}
