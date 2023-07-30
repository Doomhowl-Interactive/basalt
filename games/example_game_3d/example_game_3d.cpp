#include <basalt.h>
#include <windows.h>

using namespace std;

void UpdateAndRenderGame(Texture canvas, float delta)
{
    canvas.Clear(PURPLE);
    canvas.DrawBasaltText("Hello Basalt3D!", 10, 10, RED);
}

bool RunGame(int argc, char** argv)
{
    GameConfig config = { 0 };
    config.title = "Basalt Test Game";
    config.company = "Doomhowl Interactive";
    config.width = 640;
    config.height = 480;

    auto engine = Basalt(config, argc, argv);
    while (!engine.ShouldClose()) {
        Texture canvas = engine.BeginFrame();

        UpdateAndRenderGame(canvas, GetDeltaTime());
        engine.EndFrame();
    }

    return engine.exitCode;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifndef _DEBUG
    try {
#endif
        return RunGame(__argc, __argv);
#ifndef _DEBUG
    } catch (exception e) {
        HandleFatalException(e);
        return -1;
    }
#endif
}