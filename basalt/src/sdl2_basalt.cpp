#include <string>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
# include <SDL_timer.h>
# include <SDL_video.h>
#include <SDL_log.h>
#include <SDL_surface.h>
#else
# include <SDL2/SDL_timer.h>
# include <SDL2/SDL_video.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_surface.h>
#endif

#include "sdl2_plat.hpp"
#include "basalt_internal.hpp"

struct GameContext {
    bool shouldClose;
    usize frameIndex;
    double timeElapsed;

    Texture canvas;
};

GameConfig Game = { 0 };
GameContext Context = { 0 };

SDL_Window* Window = NULL;
SDL_Surface* ScreenSurface = NULL;
SDL_Surface* OverlaySurface = NULL;

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

    const char* copyright = FormatText("Copyright %s (2023) - %s", Game.company, Game.title);
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
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

    // initialize the Window
    Window = SDL_CreateWindow(Game.title,
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              Game.width,
                              Game.height,
                              SDL_WINDOW_ALWAYS_ON_TOP);
    if (Window == NULL) {
        SDL_LogError(0, "Could not create Window!");
        return EXIT_FAILURE;
    }

    Texture canvas = InitTexture(Game.width, Game.height);
    ScreenSurface = SDL_CreateRGBSurfaceWithFormatFrom(
        canvas.pixels, Game.width, Game.height, 32, Game.width * 4, SDL_PIXELFORMAT_ABGR32);
    OverlaySurface = SDL_CreateRGBSurface(
        0, Game.width, Game.height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);

    SetWindowTitle(Game.title);
    InitEngineFonts();

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
            if (!ProcessKeyboardInput(&event)) {
                Context.shouldClose = true;
                break;
            }
        }
        ProcessMouseInput();

        // ==== update and render main game ====
        if (UpdateAndRenderArchaeo(canvas)) {
            UpdateAndRenderGame(canvas, delta);
        }

        // === Render to the screen (software rendering)
        SDL_Surface* windowSurface = SDL_GetWindowSurface(Window);
        SDL_BlitSurface(ScreenSurface, NULL, windowSurface, NULL);
        SDL_BlitSurface(OverlaySurface, NULL, windowSurface, NULL);
        SDL_UpdateWindowSurface(Window);
        // =============================================

        // Clear the overlay surface
        SDL_FillRect(OverlaySurface, NULL, 0x00000000);

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

    SDL_FreeSurface(ScreenSurface);
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

// TODO: Implement SetTimeScale and make sure GetTimeElapsed  updates accordingly.
BASALT double GetTimeElapsed()
{
    return Context.timeElapsed;
}

PLAT_SDL2 SDL_Surface* GetScreenOverlaySurface()
{
    return OverlaySurface;
}

PLAT_SDL2 SDL_Color ConvertColor(Color color)
{
    // convert uint32 color to SDL_Color
    SDL_Color conv = {
        (Uint8)(color >> 24),
        (Uint8)(color >> 16),
        (Uint8)(color >> 8),
        (Uint8)(color >> 0),
    };
    return conv;
}
