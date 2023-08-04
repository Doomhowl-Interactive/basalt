#include <memory>
#include <basalt.h>

#include "example_game.hpp"

using namespace std;

static Font font;
static FontStyle style = {};
static shared_ptr<Image> rainbowImage;

bool RunGame(int argc, char** argv)
{
    GameConfig config = { 0 };
    config.title = "Basalt Test Game";
    config.company = "Doomhowl Interactive";
    config.width = 640;
    config.height = 480;

    auto engine = Basalt(config, argc, argv);

    font = LoadFont("Coffee Terrace.ttf");
    rainbowImage = make_shared<Image>(BakeRainbowImage((int)config.width, (int)config.height));

    style.size = 36;

    while (!engine.ShouldClose()) {
        auto canvas = engine.BeginFrame();
        UpdateAndRenderGame(canvas, (float)GetDeltaTime());
        engine.EndFrame();
    }

    return engine.exitCode.value_or(EXIT_SUCCESS);
}

void UpdateAndRenderGame(shared_ptr<Image>& canvas, float delta)
{
    static float offsetY = 0;
    offsetY += delta * 100;
    if (offsetY > (float)canvas->height) {
        offsetY = 0;
    }

    canvas->Blit(*rainbowImage, 0, (int)offsetY - canvas->height, WHITE);
    canvas->Blit(*rainbowImage, 0, (int)offsetY, WHITE);

    DrawTrippyPattern(*canvas, 70, 100, 10);

    static FontStyle backStyle = style.wColor(BLACK);
    canvas->DrawBasaltTextShadow("Hello Basalt!",
                                 Game.width / 2,
                                 Game.height / 2,
                                 Font::Default(),
                                 style.center(),
                                 backStyle.center());

    static FontStyle debugStyle = style.wColor(YELLOW).wSize(18).center(false);
    canvas->DrawBasaltTextShadow("Press DELETE to segfault",
                                 10,
                                 80,
                                 Font::Default(),
                                 debugStyle,
                                 debugStyle.wColor(DARKRED));
    canvas->DrawBasaltTextShadow("Press BACKSPACE to throw c++ error",
                                 10,
                                 120,
                                 Font::Default(),
                                 debugStyle,
                                 debugStyle.wColor(DARKRED));

    // simulate crashes to demonstrate crash handler
    if (IsKeyPressed(KEY_DELETE)) {
        int* val = new int[5];
        delete[] val;
        val[5] = 4;
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        throw BasaltException("I am a teapot.");
    }
}

BasaltMain
