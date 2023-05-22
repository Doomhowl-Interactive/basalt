#pragma once
#include "basalt.h"
#define PLATFORM

////////
// Platform specific methods
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

// basalt_utils.c
PLATFORM bool ParseLaunchArguments(int argc, char** argv);  // NOTE: Returns true if engine should continue running
PLATFORM const char* GetWorkingDirectory();
