#include "basalt.h"
#include "example_game.h"

static bool Horizontal = false;

BASALT GameConfig ConfigureGame()
{
    GameConfig config = { 0 };
    config.title = "Basalt Test Game";
    config.company = "Your company";
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
    int surface = canvas.width * canvas.height;
    for (int y = 0; y < canvas.height; y++) {
        for (int x = 0; x < canvas.width; x++) {
            float perc;
            if (Horizontal) {
                perc = 1.0f - (float)(x * canvas.height + y) / (float)surface;
            } else {
                perc = (float)(y * canvas.width + x) / (float)surface;
            }
            Color color = InterpolateHue(perc + GetTimeElapsed());
            DrawDot(canvas, x, y, color);
        }
    }

    if (IsKeyPressed(SDLK_SPACE)) {
        Horizontal = !Horizontal;
    }
}