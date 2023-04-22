#pragma once
#include "basalt.h"
#define PLATFORM

////////
// Methods that games shouldn't need, so engine only stuff.
////////

#if defined(_WIN32) || defined(_WIN64)
# define COLTEXT(C, T) T
#else
# define COLTEXT(C, T) "\e[1;" # C "m" T "\e[0m"
#endif

// engine
typedef struct GameInput {
    bool pressedKeys[256];
    bool pressedKeysOnce[256];
    bool isMouseDown;
    Point mousePos;
} GameInput;

typedef struct GameContext {
    bool shouldClose;
    usize frameIndex;
    double timeElapsed;

    Texture canvas;
} GameContext;

extern GameInput Input;
extern GameContext Context;

// basalt_tooling.c
PLATFORM bool UpdateAndRenderArchaeo(Texture canvas);
#define DRAWCALL(X, Y) DrawCallImpl(X, #Y)
PLATFORM void DrawCallImpl(Texture canvas, const char* desc);

// basalt_utils.c
PLATFORM bool ParseLaunchArguments(int argc, char** argv);  // NOTE: Returns true if engine should continue running
PLATFORM bool IsLittleEndian();

// basalt_tests.c
PLATFORM void UnitTest();

// basalt_assets.c
PLATFORM void InitHotReloading();
PLATFORM void HotReloadTexture(Texture texture);
