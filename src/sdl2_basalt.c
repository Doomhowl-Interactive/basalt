#include <SDL2/SDL.h>
#include <stdlib.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include "basalt.h"
#include "basalt_plat.h"

#define WIDTH 640
#define HEIGHT 480

GameInput Input = { 0 };
GameContext Context = { 0 };

int main(int argc, char** argv)
{
    if (!ParseLaunchArguments(argc, argv)) {
        return EXIT_SUCCESS;
    }

    SDL_Init(SDL_INIT_VIDEO);
    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    SDL_Window* window
        = SDL_CreateWindow("Basalt Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, flags);

    Texture canvas = InitTexture(WIDTH, HEIGHT);

    Uint32 maxFps = Config.unlockedFramerate ? 10000 : 60;

    while (!Context.shouldClose) {
        Uint64 startTicks = SDL_GetTicks64();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                exit(0);
        }

        Uint64 ticksPassed = SDL_GetTicks64() - startTicks;
        Context.timeElapsed = SDL_GetTicks64() / 1000.0;
        Context.frameIndex++;
    }

    UnitTest();

    return EXIT_SUCCESS;
}
