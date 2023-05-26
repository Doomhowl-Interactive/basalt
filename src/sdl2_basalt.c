#include <SDL2/SDL.h>
#include <stdlib.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <time.h>
#include "basalt.h"

#define WIDTH 640
#define HEIGHT 480

GameConfig Game = { 0 };
GameInput Input = { 0 };
GameContext Context = { 0 };
SDL2Session SDL2 = { 0 };

int main(int argc, char** argv)
{
    if (!ParseLaunchArguments(argc, argv)) {
        return EXIT_SUCCESS;
    }
    Game = ConfigureGame();

    // initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // initialize the window
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;
    SDL_CreateWindowAndRenderer(Game.width,Game.height,SDL_WINDOW_ALWAYS_ON_TOP, &window,&renderer);
    SDL2.window = window;
    if (window == NULL) {
        ERR("Could not create window!");
        return EXIT_FAILURE;
    }

    // initialize the renderer

    SDL_Surface* surface = SDL_GetWindowSurface(window);
    Texture canvas = InitTexture(WIDTH, HEIGHT);

    PrintASCIILogo("Copyright Doomhowl Interactive (2023) - Guardians of the Holy Fire");

    if (Config.hasUnitTesting){
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
            switch (event.type){
                case SDL_QUIT: {
                    Context.shouldClose = true;
                    break;
                }
                case SDL_KEYDOWN:{
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
        SDL_SetRenderDrawColor(renderer, 150, 0, 150, 255);
        SDL_RenderClear(renderer);
        if (UpdateAndRenderArchaeo(canvas)) {
            UpdateAndRenderGame(canvas, (float)delta);
        }
        SDL_RenderPresent(renderer);
        Context.frameIndex++;
        SDL_Delay(1000 / maxFps);

        Uint64 ticksPassed = SDL_GetTicks64() - startTicks;
        delta = (float)ticksPassed / 1000.0f;
        Context.timeElapsed = SDL_GetTicks64() / 1000.0;
        Context.frameIndex++;

        // Set window title to fps and delta-time
        static double timer = 0.f;
        if (timer > 0.2) {
            // set window title to framerate
            float fps = 1.0f / delta;
            const char* title = FormatText("%s - %d FPS - %f delta", Game.title, (int)fps, delta);
            SetWindowTitle(title);
            timer = 0.0;
        }
        timer += delta;

        // clear keys
        memset(Input.pressedKeysOnce, 0, sizeof(Input.pressedKeysOnce));
    }

    DisposeTexture(canvas);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    INFO("Closed SDL2 window");

    return EXIT_SUCCESS;
}
