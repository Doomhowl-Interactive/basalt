#include <string>
#include <stdlib.h>
#include <time.h>
#include <spdlog/spdlog.h>
#include <memory.h>

#include "basalt_config.hpp"
#include "basalt_testing.hpp"
#include "sdl2_plat.hpp"
#include "basalt_profiler.hpp"

using namespace std;

static Basalt* Instance = nullptr;

GameConfig Game = { 0 };
EngineConfig Config = { 0 };

SDL_Window* Window = NULL;
SDL_Surface* ScreenSurface = NULL;
SDL_Surface* OverlaySurface = NULL;

static double timeElapsed = 0;
static double delta = 0;
static Uint64 startTicks = 0;
static ulong frameIndex = 0;

static std::unique_ptr<Texture> canvas;

void SetWindowTitle(string title)
{
    if (Window != NULL) {
        // check if changed
        std::string oldTitle = SDL_GetWindowTitle(Window);
        if (oldTitle != title) {
            SDL_SetWindowTitle(Window, title.c_str());
        }
    } else {
        spdlog::error("Failed to set change window title!");
    }
}

static void Close(Basalt& b, int code)
{
    b.exitCode = code;
}

// TODO: use a pointer so it doesn't need be initialized like this
Basalt::Basalt(GameConfig config, int argc, char** argv)
{
    Instance = this;
    this->exitCode = EXIT_SUCCESS;

    EngineConfig Config = { 0 };
    if (!ParseLaunchArguments(&Config, argc, argv)) {
        Close(*this, EXIT_SUCCESS);
        return;
    }

    Game = config;

    auto copyright = FormatText("Copyright %s (2023) - %s", Game.company, Game.title);
    PrintASCIILogo(copyright);

    if (Config.hasUnitTesting) {
        RunUnitTests();
        Close(*this, EXIT_SUCCESS);
        return;
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
                              NULL);
    if (Window == NULL) {
        spdlog::critical("Could not create Window!");
        Close(*this, EXIT_FAILURE);
        return;
    }

    // initialize the main texture
    canvas = std::make_unique<Texture>(Texture(Game.width, Game.height));

    // NOTE: If this breaks go back to using raw pointers instead of vector
    const auto& canvasPixels = *canvas->pixels.get();
    ScreenSurface = SDL_CreateRGBSurfaceWithFormatFrom((void*)canvasPixels.data(),
                                                       Game.width,
                                                       Game.height,
                                                       32,
                                                       Game.width * 4,
                                                       SDL_PIXELFORMAT_ABGR32);
    OverlaySurface = SDL_CreateRGBSurface(
        0, Game.width, Game.height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);

    SetWindowTitle(Game.title);

    srand((unsigned int)time(NULL));
};

bool Basalt::ShouldClose()
{
    static Uint32 maxFps = Config.unlockedFramerate ? 10000 : 60;
    static float delta = 0.f;

    static Uint64 startTicks = SDL_GetTicks64();

    if (exitCode == EXIT_FAILURE) {
        return true;
    }

    // Process window and keyboard input
    static SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                return true;
            }
            default: {
                break;
            }
        }
        if (!ProcessKeyboardInput(&event)) {
            return true;
        }
    }
    return false;
}

Texture Basalt::BeginFrame()
{
    if (canvas) {
        startTicks = SDL_GetTicks64();
        ProcessMouseInput();
        return *canvas;
    } else {
        spdlog::critical("No basalt canvas!");
    }
}

void Basalt::EndFrame()
{
    double maxFps = Config.unlockedFramerate ? 10000 : 200;

// Draw profiler overlay
#ifndef NO_PROFILER
    if (true || Config.hasProfiler) {
        DrawProfiler(*canvas);
    }
#endif

    // === Render to the screen (software rendering)
    SDL_Surface* windowSurface = SDL_GetWindowSurface(Window);
    SDL_BlitSurface(ScreenSurface, NULL, windowSurface, NULL);
    SDL_BlitSurface(OverlaySurface, NULL, windowSurface, NULL);
    SDL_UpdateWindowSurface(Window);
    // =============================================

    // Clear the overlay gui surface, (it needs to be transparent!)
    SDL_FillRect(OverlaySurface, NULL, 0x00000000);

    frameIndex++;

    // FIXME TODO: This waits longer than it should!
    Uint32 timeToWait = (Uint32)(1000 / maxFps);
    SDL_Delay(timeToWait);

    Uint64 ticksPassed = SDL_GetTicks64() - startTicks;
    delta = ticksPassed / 1000.0;
    timeElapsed = (double)SDL_GetTicks64() / 1000.0;
    frameIndex++;

    // Set Window title to fps and delta-time
    static double timer = 0.f;
    if (timer > 0.2) {
        // set Window title to framerate
        double fps = 1.0 / delta;
        auto title = FormatText("%s - %d FPS - %f delta", Game.title, (int)fps, delta);
        SetWindowTitle(title);
        timer = 0.0;
    }
    timer += delta;

    ClearKeyboardInput();
}

Basalt::~Basalt()
{
    SDL_FreeSurface(ScreenSurface);
    SDL_DestroyWindow(Window);
    CloseSystemConsole();
    spdlog::info("Closed SDL2 Window");
}

ulong GetFrameIndex()
{
    return frameIndex;
}

// TODO: Implement SetTimeScale and make sure GetTimeElapsed  updates accordingly.
double GetTimeElapsed()
{
    return timeElapsed;
}

double GetDeltaTime()
{
    return delta;
}

SDL_Surface* GetScreenOverlaySurface()
{
    return OverlaySurface;
}

SDL_Color ConvertColor(Color color)
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
