#include <stdio.h>
#include "basalt.h"
#include "basalt_extra.h"
#include "bullet_common.h"

const Color tabBGColor = 0x111111FF;
const Color tabFGColor = 0xAAAAAAFF;
static int tabWidth = 0;
static int tabHeight = 20;

static bool IsOpened = false;
static uint SelectedTabIndex = 0;

typedef void (*EditorTabFunc)(Scene* activeScene, Texture canvas, float delta);

typedef struct EditorTab {
    const char* name;
    EditorTabFunc function;
} EditorTab;

func void DrawMainTab(Scene* activeScene, Texture canvas, float delta);
func void DrawAssetTab(Scene* activeScene, Texture canvas, float delta);
func void DrawPatternsTab(Scene* activeScene, Texture canvas, float delta);
static const EditorTab EditorTabs[] = { { "Main", DrawMainTab }, { "Assets", DrawAssetTab }, { "Patterns", DrawPatternsTab }, { NULL } };

func void DrawMainTab(Scene* activeScene, Texture canvas, float delta)
{
    const char* text = FormatText("Active entities %lu", GetEntityCount());
    DrawText(canvas, text, 50, 50, YELLOW);
}

func void DrawAssetTab(Scene* activeScene, Texture canvas, float delta)
{
    int x = 50;
    int y = 50;
    uint count = 0;
    Texture* textures = GetLoadedTextures();
    for (Texture* texture = textures; texture->name != NULL; texture++) {
        DrawText(canvas, texture->name, x, y, WHITE);
        y += 30;
        count++;
    }
    const char* countText = FormatText("Texture count %u", count);
    DrawText(canvas, countText, x, y, GRAY);
}

func void DrawPatternsTab(Scene* activeScene, Texture canvas, float delta)
{
    DrawText(canvas, "Bullet patterns", 50, 50, PURPLE);
    UpdateAndRenderPatternEditor(canvas, delta);
}

func void DrawEditorTabs(Scene* activeScene, Texture canvas, float delta, const EditorTab* tabs)
{
    if (tabWidth == 0) {
        uint tabCount = 0;
        for (const EditorTab* tab = tabs; tab->name != NULL; tab++) {
            tabCount++;
        }

        assert(tabCount > 0);
        tabWidth = WIDTH / tabCount;
    }

    uint i = 0;
    for (const EditorTab* tab = tabs; tab->name != NULL; tab++) {
        bool isSelected = SelectedTabIndex == i;

        Color bg = isSelected ? BlendColors(tabBGColor, GREEN, 50) : tabBGColor;
        Color fg = isSelected ? BlendColors(tabFGColor, GREEN, 50) : tabFGColor;

        uint x = tabWidth * i;
        DrawRectangle(canvas, x, 0, tabWidth, tabHeight, bg);
        DrawRectangleLines(canvas, x, 0, tabWidth, tabHeight, 2, fg);
        DrawText(canvas, tab->name, x + 5, 5, WHITE);

        // Draw tab content
        if (isSelected && tab->function != NULL) {
            (*tab->function)(activeScene, canvas, delta);
        }

        // Change if hovered over
        Point mouse = GetMousePosition();
        if (mouse.x >= x && mouse.y >= 0 && mouse.x <= x + tabWidth && mouse.y <= tabHeight) {
            SelectedTabIndex = i;
        }

        i++;
    }
}

BULLET void UpdateAndRenderEditor(Scene* activeScene, Texture canvas, float delta)
{
    if (IsKeyPressed(SDLK_y)) {
        IsOpened = !IsOpened;
        INFO("%s editor", IsOpened ? "Opened" : "Closed");
    }

    if (!IsOpened) {
        return;
    }

    DrawEditorTabs(activeScene, canvas, delta, EditorTabs);
}

BULLET Rect GetEditorTabContentRegion()
{
    return (Rect){ 0, tabHeight, WIDTH, HEIGHT - tabHeight };
}

BULLET bool IsEditorOpen()
{
    return IsOpened;
}
