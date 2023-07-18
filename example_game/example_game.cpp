#include <basalt.h>

#include "example_game.h"

static bool Horizontal = false;
static Font font;

using namespace std;

bool RunGame(int argc, char** argv)
{
    GameConfig config = { 0 };
    config.title = "Basalt Test Game";
    config.company = "Doomhowl Interactive";
    config.width = 640;
    config.height = 480;

    auto engine = Basalt(config, argc, argv);

    font = LoadFont("font_sf_cartoonist_hand.ttf");

    while (!engine.ShouldClose()) {
        Texture canvas = engine.BeginFrame();
        UpdateAndRenderGame(canvas, GetDeltaTime());
        engine.EndFrame();
    }

    return engine.exitCode;
}

int main(int argc, char** argv)
{
#ifndef _DEBUG
    try {
#endif
        return RunGame(argc, argv);
#ifndef _DEBUG
    } catch (exception e) {
        HandleFatalException(e);
        return -1;
    }
#endif
}

void UpdateAndRenderGame(Texture canvas, float delta)
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
            canvas.DrawDot(x, y, color);
        }
    }

    canvas.DrawText("Hello Basalt!", 10, 10, WHITE, font);
    canvas.DrawText("Press SPACE to change rainbow direction", 10, 50, GREEN, font);
    canvas.DrawText("Press DELETE to segfault", 10, 80, RED, font);
    canvas.DrawText("Press BACKSPACE to throw c++ error", 10, 120, RED, font);

    if (IsKeyPressed(KEY_SPACE)) {
        Horizontal = !Horizontal;
    }

    // simulate crashes to demonstrate crash handler
    if (IsKeyPressed(KEY_DELETE)) {
        int* val = new int[5];
        delete[] val;
        val[5] = 4;
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        throw exception("I am a teapot.");
    }
}