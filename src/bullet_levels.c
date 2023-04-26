#include "bullet_levels.h"
#include "basalt.h"

static const LevelLayout TrainingLevelLayout = {};

// TODO: Use indices instead
const LevelInfo Level1 = {
    0,
    "Training Ground",
    &TrainingLevelLayout,
    ScrollingPurpleNoise,
};

static double LastLevelChange = 0.0;
static const LevelInfo* CurrentLevel = NULL;

BULLET void SwitchLevel(const LevelInfo* level)
{
    CurrentLevel = level;
    LastLevelChange = GetTimeElapsed();
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

func void UpdateAndRenderBackground(Texture canvas, float delta)
{
    assert(CurrentLevel);
    if (CurrentLevel->background) {
        CurrentLevel->background(canvas, delta);
    }
}

func void UpdateAndRenderGUI(Texture canvas, float delta)
{
    assert(CurrentLevel);
    if (GetTimeElapsed() - LastLevelChange < 2.0) {
        DrawText(canvas, CurrentLevel->name, 150, 150, 0xFFFF00FF);
    }
}

BULLET bool UpdateAndRenderLevel(Texture canvas, Scene* scene, float delta)
{
    UpdateAndRenderBackground(canvas, delta);
    UpdateAndRenderScene(scene, canvas, delta);
    UpdateAndRenderEditor(scene, canvas, delta);
    UpdateAndRenderGUI(canvas, delta);
    return false;
}
