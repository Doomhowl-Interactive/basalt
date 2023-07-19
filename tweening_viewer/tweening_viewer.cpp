#include <basalt.h>
#include <memory>
#include "tweening_display.cpp"

using namespace std;

bool RunGame(int argc, char** argv)
{
    GameConfig config = { 0 };
    config.title = "Basalt Test Game";
    config.company = "Doomhowl Interactive";
    config.width = 640;
    config.height = 480;

    auto engine = Basalt(config, argc, argv);

    unique_ptr<TweeningDisplay> display = nullptr;
    while (!engine.ShouldClose()) {
        Texture canvas = engine.BeginFrame();
        if (!display) {
            display = unique_ptr<TweeningDisplay>(new TweeningDisplay(canvas));
        }
        canvas.Clear(LIGHTGRAY);
        display->UpdateAndRenderTweenings((float)GetDeltaTime());
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
