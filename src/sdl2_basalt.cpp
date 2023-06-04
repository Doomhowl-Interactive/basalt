#include <string>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <SDL_timer.h>
#include <SDL_video.h>
#else
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#endif

#include "sdl2_plat.hpp"

struct GameContext {
    bool shouldClose;
    usize frameIndex;
    double timeElapsed;

    Texture canvas;
};

GameConfig Game = { 0 };
GameContext Context = { 0 };

SDL_Window* Window = NULL;

BASALT void SetWindowTitle(const char* title)
{
    if (Window != NULL) {
        // check if changed
        std::string oldTitle = SDL_GetWindowTitle(Window);
        if (oldTitle != title) {
            SDL_SetWindowTitle(Window, title);
        }
    } else {
        SDL_LogError(0, "Failed to set change window title!");
    }
}

int main(int argc, char** argv)
{
    if (!ParseLaunchArguments(argc, argv)) {
        return EXIT_SUCCESS;
    }

    Game = ConfigureGame();

    const char* copyright = FormatText("Copyright %s (2023) - %s", Game.company,Game.title);
    PrintASCIILogo(copyright);

    if (Config.hasUnitTesting) {
        UnitTest();
        exit(EXIT_SUCCESS);
    }

    if (Config.hasConsole) {
        // open console (windows only)
        OpenSystemConsole();
    }

    // initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // initialize the Window and renderer
    Window
        = SDL_CreateWindow(Game.title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Game.width, Game.height, SDL_WINDOW_ALWAYS_ON_TOP);
    if (Window == NULL) {
        SDL_LogError(0, "Could not create Window!");
        return EXIT_FAILURE;
    }

    Texture canvas = InitTexture(Game.width, Game.height);
    SDL_Surface* canvasSurface
        = SDL_CreateRGBSurfaceWithFormatFrom(canvas.pixels, Game.width, Game.height, 32, Game.width * 4, SDL_PIXELFORMAT_ABGR32);

    SetWindowTitle(Game.title);

    srand((unsigned int)time(NULL));
    InitializeGame();
    InitHotReloading();

    Uint32 maxFps = Config.unlockedFramerate ? 10000 : 60;

    float delta = 0.f;
    while (!Context.shouldClose) {
        Uint64 startTicks = SDL_GetTicks64();

        // Process window and keyboard input
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    Context.shouldClose = true;
                    break;
                }
                default: {
                    break;
                }
            }
            if (!ProcessKeyboardInput(event)) {
                Context.shouldClose = true;
                break;
            }
        }
        ProcessMouseInput();

        // ==== update and render main game ====
        if (UpdateAndRenderArchaeo(canvas)) {
            UpdateAndRenderGame(canvas, delta);
        }
        SDL_BlitSurface(canvasSurface, NULL, SDL_GetWindowSurface(Window), NULL);
        SDL_UpdateWindowSurface(Window);
        Context.frameIndex++;
        SDL_Delay(1000 / maxFps);

        Uint64 ticksPassed = SDL_GetTicks64() - startTicks;
        delta = (float)ticksPassed / 1000.0f;
        Context.timeElapsed = (double)SDL_GetTicks64() / 1000.0;
        Context.frameIndex++;

        // Set Window title to fps and delta-time
        static double timer = 0.f;
        if (timer > 0.2) {
            // set Window title to framerate
            float fps = 1.0f / delta;
            const char* title = FormatText("%s - %d FPS - %f delta", Game.title, (int)fps, delta);
            SetWindowTitle(title);
            timer = 0.0;
        }
        timer += delta;

        ClearKeyboardInput();
    }

    SDL_FreeSurface(canvasSurface);
    DisposeTexture(canvas);
    SDL_DestroyWindow(Window);
    CloseSystemConsole();
    SDL_Log("Closed SDL2 Window");

    return EXIT_SUCCESS;
}

BASALT usize GetFrameIndex()
{
    return Context.frameIndex;
}

BASALT double GetTimeElapsed()
{
    return Context.timeElapsed;
}
