#include <basalt.h>

using namespace std;

void UpdateAndRenderGame(Image& canvas, float delta)
{
    canvas.Clear(PURPLE);
    canvas.DrawBasaltText("Hello Basalt3D!", 10, 10, RED);
}

int RunGame(int argc, char** argv)
{
    GameConfig config = { 0 };
    config.title = "Basalt Test Game";
    config.company = "Doomhowl Interactive";
    config.width = 640;
    config.height = 480;

    auto engine = Basalt(config, argc, argv);
    while (!engine.ShouldClose()) {
        auto canvas = engine.BeginFrame();
        UpdateAndRenderGame(*canvas, (float)GetDeltaTime());
        engine.EndFrame();
    }

    return engine.exitCode.value_or(0);
}

BasaltMain