#include <SDL2/SDL.h>
#include <stdlib.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <time.h>
#include "basalt.h"

GameConfig Game = { 0 };
GameInput Input = { 0 };
GameContext Context = { 0 };

SDL_Window *Window = NULL;

BASALT void SetWindowTitle(const char* title)
{
    if (Window != NULL) {
        // check if changed
        if (!TextIsEqual(SDL_GetWindowTitle(Window), title)) {
            SDL_SetWindowTitle(Window, title);
        }
    } else {
        ERR("Failed to set change window title!\n");
    }
}

int main(int argc, char** argv)
{
    if (!ParseLaunchArguments(argc, argv)) {
        return EXIT_SUCCESS;
    }
    Game = ConfigureGame();

    if (Config.hasConsole)
    {
        // open console (windows only)
        OpenSystemConsole();
    }

    // initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // initialize the Window and renderer
    Window = SDL_CreateWindow(Game.title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Game.width, Game.height, SDL_WINDOW_ALWAYS_ON_TOP);
    if (Window == NULL) {
        ERR("Could not create Window!");
        return EXIT_FAILURE;
    }

    Texture canvas = InitTexture(Game.width, Game.height);
    SDL_Surface* canvasSurface
        = SDL_CreateRGBSurfaceWithFormatFrom(canvas.pixels, Game.width, Game.height, 32, Game.width * 4, SDL_PIXELFORMAT_ABGR32);

    char text[1024];
    sprintf(text, "Copyright Doomhowl Interactive (2023) - %s", Game.title);
    PrintASCIILogo(text);

    if (Config.hasUnitTesting) {
        UnitTest();
    }

    SetWindowTitle(Game.title);

    srand((unsigned int)time(NULL));
    InitializeGame();
    InitHotReloading();

    Uint32 maxFps = Config.unlockedFramerate ? 10000 : 60;

    float delta = 0.f;
    while (!Context.shouldClose) {
        Uint64 startTicks = SDL_GetTicks64();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    Context.shouldClose = true;
                    break;
                }
                case SDL_KEYDOWN: {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        Context.shouldClose = true;
                        break;
                    }
                }
                default: {
                    break;
                }
            }
        }

        // poll the mouse

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

        // clear keys
        memset(Input.pressedKeysOnce, 0, sizeof(Input.pressedKeysOnce));
    }

    SDL_FreeSurface(canvasSurface);
    DisposeTexture(canvas);
    SDL_DestroyWindow(Window);
    CloseSystemConsole();
    INFO("Closed SDL2 Window");

    return EXIT_SUCCESS;
}
