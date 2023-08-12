#include <string>
#include <spdlog/spdlog.h>

#include "basalt_console.hpp"
#include "basalt_config.hpp"
#include "basalt_testing.hpp"
#include "basalt_profiler.hpp"
#include "basalt_instance.hpp"
#include "basalt_renderer.hpp"
#include "sdl2_basalt.hpp"

using namespace std;

GameConfig Game = { 0 };
EngineConfig Config = { 0 };

SDL_Window* Window = nullptr;
SDL_Surface* ScreenSurface = nullptr;

static double timeElapsed = 0;
static double delta = 0;
static Uint64 startTicks = 0;
static ulong frameIndex = 0;
static bool instanceExists = false;
static shared_ptr<Image> canvas;

void SetWindowTitle(string title)
{
    if (Window) {
        // check if changed
        std::string oldTitle = SDL_GetWindowTitle(Window);
        if (oldTitle != title) {
            SDL_SetWindowTitle(Window, title.c_str());
        }
    } else {
        spdlog::error("Failed to set change window title!");
    }
}

void Basalt::Close(int code)
{
    exitCode = code;
    spdlog::info("Closing Basalt with exit code {}", code);
}

Basalt::Basalt(GameConfig config, int argc, char** argv)
{
    if (instanceExists) {
        spdlog::error("Basalt is already initialized! There can only be one.");
        Close(EXIT_FAILURE);
        return;
    }

    instanceExists = true;
    Game = config;
    
    if (!ParseLaunchArguments(&Config, argc, argv)) {
        Close(EXIT_SUCCESS);
        return;
    }

    auto copyright = FormatText("Copyright %s (2023) - %s", Game.company, Game.title);
    PrintASCIILogo(copyright);

    if (Config.hasUnitTesting) {
        RunUnitTests();
        Close(EXIT_SUCCESS);
        return;
    }

#ifdef _DEBUG
    if (Config.hasConsole) {
        // open console (windows only)
        OpenSystemConsole();
    }
#endif  // ! _DEBUG

    // initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    spdlog::set_level(spdlog::level::debug);

    // initialize the Window
    Window = SDL_CreateWindow(Game.title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Game.width, Game.height, 0);
    if (Window == nullptr) {
        spdlog::critical("Could not create Window!");
        Close(EXIT_FAILURE);
        return;
    }

    // Initialize the default renderer
    canvas = SetupDefaultRenderer(Window);

    // Load default font
    LoadFont("FFFFORWA.TTF");

    ScreenSurface = SDL_GetWindowSurface(Window);

    SetWindowTitle(Game.title);

    srand((unsigned int)time(nullptr));
};

bool Basalt::ShouldClose()
{
    static Uint32 maxFps = Config.unlockedFramerate ? 10000 : 60;
    static Uint64 startTicks = SDL_GetTicks();
    static float delta = 0.f;

    if (exitCode.has_value()) {
        return true;
    }

    // Process window and keyboard input
    static SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT : {
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

shared_ptr<Image> Basalt::BeginFrame()
{
    if (canvas) {
        startTicks = SDL_GetTicks();
        ProcessMouseInput();
        return canvas;
    } else {
        spdlog::critical("No basalt canvas!");
    }
}

void Basalt::EndFrame()
{
    double maxFps = Config.unlockedFramerate ? 10000 : 60;

    if (Config.hasProfiler && DrawProfiler(*canvas)) {
        UpdateProfiler();
    }

    DrawDefaultFrame();

    frameIndex++;

    // FIXME TODO: This waits longer than it should!
    Uint32 timeToWait = (Uint32)(1000 / maxFps);
    SDL_Delay(timeToWait);

    Uint64 ticksPassed = SDL_GetTicks() - startTicks;
    delta = ticksPassed / 1000.0;
    timeElapsed = (double)SDL_GetTicks() / 1000.0;
    frameIndex++;

// Set Window title to fps and delta-time
#ifdef _DEBUG
    static double timer = 0.f;
    if (timer > 0.2) {
        // set Window title to framerate
        double fps = 1.0 / delta;
        auto title = FormatText("%s - %d FPS - %f delta", Game.title, (int)fps, delta);
        SetWindowTitle(title);
        timer = 0.0;
    }
    timer += delta;
#endif

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

void HandleFatalException(exception e)
{
    spdlog::critical("Fatal exception: {}", e.what());
    exit(1);
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
