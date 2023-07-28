#include "pch.h"
#include "framework.h"

#include <spdlog/spdlog.h>

#include "sdl2_basalt.hpp"
#include "basalt_images.hpp"

using namespace std;

struct CPURenderer {
    SDL_Window* win;
    unique_ptr<Image> canvas;

    explicit CPURenderer(SDL_Window* window)
    {
        win = window;
        canvas = std::make_unique<Image>(Image(Game.width, Game.height));
    }
};

static CPURenderer* renderer = nullptr;

shared_ptr<Image> SetupDefaultRenderer(SDL_Window* window)
{
    if (renderer) {
        spdlog::warn("Renderer already initialized!");
        return;
    }
    renderer = new CPURenderer(window);
}

void DrawDefaultFrame()
{
}

void DisposeDefaultRenderer()
{
    if (renderer) {
        delete renderer;
        renderer = nullptr;
    }
}