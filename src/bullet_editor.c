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

    DrawText(canvas, "Hello editor!", 50,50, 0xFF00FFFF);
}
