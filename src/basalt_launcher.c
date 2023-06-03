#include "basalt.h"

#define LAUNCHER
extern LAUNCHER void DrawColoredGrid(Texture canvas);

BASALT GameConfig ConfigureGame()
{
    GameConfig config = { 0 };
    config.title = "Insert game cartridge";
    config.company = "Doomhowl Interactive";
    config.width = 640;
    config.height = 480;
    return config;
}

BASALT void InitializeGame()
{
}

BASALT void DisposeGame()
{
}

BASALT void UpdateAndRenderGame(Texture canvas, float delta)
{
    DrawColoredGrid(canvas);
}