#include "basalt_extra.h"
#include "bullet_common.h"

static bool IsOpened = false;
static uint SelectedTabIndex = 0;

typedef void (*EditorTabFunc)(Scene* activeScene, Texture canvas, float delta);

class(EditorTab)
{
    const char* name;
    EditorTabFunc function;
};

func void DrawMainTab(Scene* activeScene, Texture canvas, float delta);
func void DrawPatternsTab(Scene* activeScene, Texture canvas, float delta);
static const EditorTab EditorTabs[] = {
    {"Main", DrawMainTab },
    {"Patterns", DrawPatternsTab },
    { NULL }
};

func void DrawMainTab(Scene* activeScene, Texture canvas, float delta)
{
    DrawText(canvas, "Hello editor!", 50,50, YELLOW);
}

func void DrawPatternsTab(Scene* activeScene, Texture canvas, float delta)
{
    DrawText(canvas, "Bullet patterns", 50,50, PURPLE);
}

func void DrawEditorTabs(Scene* activeScene, Texture canvas, float delta, const EditorTab* tabs)
{
    static Color bgColor = 0x111111FF;
    static Color fgColor = 0xAAAAAAFF;
    static uint tabWidth = 0;
    static uint tabHeight= 20;
    if (tabWidth == 0)
    {
        uint tabCount = 0;
        for (const EditorTab* tab = tabs; tab->name != NULL; tab++)
            tabCount++;

        assert(tabCount > 0);
        tabWidth = WIDTH / tabCount;
    }

    uint i = 0;
    for (const EditorTab* tab = tabs; tab->name != NULL; tab++)
    {
        bool isSelected = SelectedTabIndex == i;

        Color bg = isSelected ? BlendColors(bgColor,GREEN,50):bgColor;
        Color fg = isSelected ? BlendColors(fgColor,GREEN,50):fgColor;

        uint x = tabWidth * i;
        DrawRectangle(canvas, x, 0, tabWidth, tabHeight, bg);
        DrawRectangleLines(canvas, x, 0, tabWidth, tabHeight, 2, fg);
        DrawText(canvas, tab->name, x + 5, 5, WHITE);

        // Draw tab content
        if (isSelected && tab->function != NULL)
            (*tab->function)(activeScene, canvas, delta);

        // Change if hovered over
        Point mouse = GetMousePosition();
        if (mouse.x >= x && mouse.y >= 0 && mouse.x <= x + tabWidth && mouse.y <= tabHeight)
            SelectedTabIndex = i;

        i++;
    }
}

BULLET void UpdateAndRenderEditor(Scene* activeScene, Texture canvas, float delta)
{
    if (IsKeyPressed(KEY_Y))
    {
        IsOpened = !IsOpened;
        INFO("%s editor", IsOpened ? "Opened":"Closed");
    }

    if (!IsOpened)
        return; 

    DrawEditorTabs(activeScene, canvas, delta, EditorTabs);
}

BULLET bool IsEditorOpen()
{
    return IsOpened;
}
