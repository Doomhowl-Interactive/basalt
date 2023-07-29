#include <memory>
#include <basalt.h>

#include "example_game.hpp"

static bool Horizontal = false;
static Font font;

using namespace std;

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

    style.color = WHITE;
    style.size = 32;
    style.centered = true;

    while (!engine.ShouldClose()) {
        auto canvas = engine.BeginFrame();
        UpdateAndRenderGame(canvas, (float)GetDeltaTime());
        engine.EndFrame();
    }

    return engine.exitCode;
}

// TODO: Abstract this away!
#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
# ifndef _DEBUG
    try {
# endif
        return RunGame(__argc, __argv);
# ifndef _DEBUG
    } catch (exception e) {
        HandleFatalException(e);
        return -1;
    }
# endif
}
#endif
#ifdef __APPLE__
int main(int argc, char** argv)
{
    return RunGame(argc, argv);
}
#endif

void UpdateAndRenderGame(shared_ptr<Image>& canvas, float delta)
{
    static float offsetY = 0;
    offsetY += delta * 100;
    if (offsetY > (float)canvas->height) {
        offsetY = 0;
    }

    canvas->Blit(*rainbowImage, 0, (int)offsetY - canvas->height, WHITE);
    canvas->Blit(*rainbowImage, 0, (int)offsetY, WHITE);

    canvas->DrawRectangle(20, 50, 100, 100, RED);
    canvas->DrawRectangle(30, 10, 100, 100, BLUE);
    canvas->DrawLine(100, 100, 200, 200, GREEN);

    canvas->DrawBasaltText("Hello Basalt!", Game.width / 2, Game.height / 2, style);
    canvas->DrawBasaltText("Press SPACE to change rainbow direction", 10, 50, GREEN, font);
    canvas->DrawBasaltText("Press DELETE to segfault", 10, 80, RED, font);
    canvas->DrawBasaltText("Press BACKSPACE to throw c++ error", 10, 120, RED, font);

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
        throw BasaltException("I am a teapot.");
    }
}