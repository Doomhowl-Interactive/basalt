#include "basalt.h"
#include "bullet_common.h"
#define BACKGROUND

typedef void (*BackgroundRenderer)(Texture canvas, float delta);

typedef struct LevelLayout {
} LevelLayout;

typedef struct LevelInfo {
    int number;
    const char* name;
    const LevelLayout* layout;
    BackgroundRenderer background;
} LevelInfo;

extern const LevelInfo Level1;

BULLET void SwitchLevel(const LevelInfo* level);

BACKGROUND void ScrollingPurpleNoise(Texture canvas, float delta);
BULLET bool UpdateAndRenderLevel(Texture canvas, Scene* scene, float delta);
