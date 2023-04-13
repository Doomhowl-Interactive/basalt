#pragma once
#include "basalt.h"

////////
// Methods that games shouldn't need, so engine only stuff.
////////

#if defined(_WIN32) || defined(_WIN64)
# define COLTEXT(C, T) T
#else
# define COLTEXT(C, T) "\e[1;" # C "m" T "\e[0m"
#endif

// engine
class(GameInput)
{
    bool pressedKeys[256];
    bool pressedKeysOnce[256];
    bool isMouseDown;
    Point mousePos;
};

class(GameContext)
{
    bool shouldClose;
    usize frameIndex;
    double timeElapsed;

    Texture canvas;
};

extern GameInput Input;
extern GameContext Context;

// basalt_tooling.c
platfunc bool UpdateAndRenderArchaeo();
#define DRAWCALL(X, Y) DrawCallImpl(X, #Y)
platfunc void DrawCallImpl(Texture canvas, const char* desc);

// basalt_utils.c
platfunc bool ParseLaunchArguments(int argc, char** argv);  // NOTE: Returns true if engine should continue running
platfunc bool IsLittleEndian();

// basalt_tests.c
platfunc void UnitTest();

// basalt_assets.c
platfunc void InitHotReloading();
platfunc void HotReloadTexture(Texture texture);
