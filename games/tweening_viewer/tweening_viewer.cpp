#include <memory>

#include "basalt.h"
#include "tweening_scroll.cpp"
#include "tweening_display.cpp"

using namespace std;

int RunGame(int argc, char** argv)
{
    GameConfig config = { 0 };
    config.title = "Basalt Tween Viewer";
    config.company = "Doomhowl Interactive";
    config.width = 900;
    config.height = 900;

    auto engine = Basalt(config, argc, argv);

    unique_ptr<TweeningDisplay> display = nullptr;
    while (!engine.ShouldClose()) {
        auto canvas = engine.BeginFrame();
        if (!display) {
            display = std::make_unique<TweeningDisplay>(*canvas);
        }
        canvas->Clear(LIGHTGRAY);
        static float scrollY = ScrollOnWindowEdges();
        // scrollY -= ScrollOnWindowEdges();
        // scrollY = Clamp(scrollY, -256.f, 0.f);
        scrollY = 0.f;
        display->UpdateAndRenderTweenings((float)GetDeltaTime(), scrollY);
        engine.EndFrame();
    }

    return engine.exitCode.value_or(0);
}

BasaltMain
