#include "bullet_levels.h"
#include "basalt.h"

// TODO: Use indices instead
const LevelInfo Level1 = {
    0,
    "Training Ground",
    TrainingLevelLayout,
    ScrollingPurpleNoise,
};

typedef struct LevelContext {
    double lastLevelChange;
    const LevelInfo* currentLevel;
} LevelContext;

static LevelContext Context = { 0 };

func void UpdateAndRenderBackground(Texture canvas, float delta)
{
    assert(Context.currentLevel);
    if (Context.currentLevel->backgroundFunc) {
        Context.currentLevel->backgroundFunc(canvas, delta);
    }
}

func void UpdateAndRenderGUI(Texture canvas, float delta)
{
    const LevelInfo* level = Context.currentLevel;

    assert(level);
    if (GetTimeElapsed() - Context.lastLevelChange < 2.0) {
        DrawText(canvas, level->name, 150, 150, 0xFFFF00FF);
    }
}

BULLET void SwitchLevel(const LevelInfo* level)
{
    Context.currentLevel = level;
    Context.lastLevelChange = GetTimeElapsed();
}

BULLET bool UpdateAndRenderLevel(Texture canvas, Scene* scene, float delta)
{
    UpdateAndRenderBackground(canvas, delta);
    UpdateAndRenderScene(scene, canvas, delta);
    UpdateAndRenderEditor(scene, canvas, delta);
    UpdateAndRenderGUI(canvas, delta);
    return false;
}

BACKGROUND void ScrollingPurpleNoise(Texture canvas, float delta)
{
    // TODO: blend at runtime 0x4B486EFF, 0x07060FFF
    const float backgroundScrollSpeed = 100.f;
    Texture noiseTexture = RequestTexture(SPR_BACKGROUND_NOISE_COL);
    int offsetY = (int)(GetTimeElapsed() * backgroundScrollSpeed) % HEIGHT;
    DrawTexture(canvas, noiseTexture, 0.f, -offsetY, WHITE);
    DrawTexture(canvas, noiseTexture, 0.f, -offsetY + HEIGHT, WHITE);
}

SCHEDULE void TrainingLevelLayout(int difficulty)
{
}
