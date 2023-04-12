#include "basalt_extra.h"
#include "bullet_common.h"

static bool IsOpened = true;

BULLET void UpdateAndRenderEditor(Scene* activeScene, Texture canvas, float delta)
{
    if (IsKeyPressed(KEY_Y))
    {
        IsOpened = !IsOpened;
        INFO("%s editor", IsOpened ? "Opened":"Closed");
    }

    if (!IsOpened)
        return; 
}
