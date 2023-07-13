#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "basalt_keys.h"
#include "basalt_graphics.hpp"

// Ergonomic macros
#define V2(V) V.x, V.y
#define _V2(V) V->x, V->y
#define P2(P) P.x, P.y
#define _P2(P) P->x, P->y
#define R2(R) R.x, R.y, R.width, R.height
#define _R2(R) R.x, R.y, R.width, R.height

// Asset handling (basalt_assets.c)

Texture RequestTexture(const char* name);

bool IsMouseDown();
bool IsMousePressed();
bool IsMouseUp();

bool IsKeyDown(KeyCode code);
bool IsKeyUp(KeyCode code);

bool IsKeyPressed(KeyCode code);
bool IsKeyReleased(KeyCode code);

// Engine configuration (basalt_config.c)
typedef struct EngineConfig {
    bool hasArchaeo;
    bool hasHotloading;
    bool hasUnitTesting;
    bool hasConsole;
    bool useSoftware;
    bool isHeadless;
    bool unlockedFramerate;
    bool lowQuality;
} EngineConfig;
extern EngineConfig Config;

typedef struct GameConfig {
    const char* title;
    const char* company;
    unsigned int width;
    unsigned int height;
    unsigned char language;
} GameConfig;
extern GameConfig Game;

// Graphics drawing (basalt_graphics.c)
#define WHITE 0xFFFFFFFF
#define GRAY 0xAAAAAAFF
#define LIGHTGRAY 0xC8C8C8FF
#define DARKGRAY 0x505050FF
#define BLACK 0x000000FF
#define BLANK 0xFFFFFF00

#define RED 0xFF0000FF
#define GREEN 0x00FF00FF
#define DARKGREEN 0x00752CFF
#define BLUE 0x0000FFFF
#define DARKBLUE 0x00527FFF
#define YELLOW 0xFFFF00FF
#define PURPLE 0x00FFFFFF
#define BROWN 0x7F3F00FF
#define DARKBROWN 0x5A2E00FF
#define DARKRED 0x7F0000FF

extern void LoadFont(const char* name);
void LoadFontEx(const char* name, size_t size);

// TODO: Put in private header
void DisposeFonts();

// Main game methods (for example: temple_game.c)
DYNAMIC GameConfig ConfigureGame();
DYNAMIC void InitializeGame();
DYNAMIC void DisposeGame();
DYNAMIC void UpdateAndRenderGame(Texture canvas, float delta);

// basalt_tests.c
void UnitTest();

// sdl2_basalt.h
void OpenSystemConsole();
void CloseSystemConsole();

#ifdef __cplusplus
}
#endif
