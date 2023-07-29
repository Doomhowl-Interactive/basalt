#include "pch.h"
#include "framework.h"

#include <spdlog/spdlog.h>

#include "basalt_exceptions.hpp"
#include "basalt_images.hpp"
#include "sdl2_basalt.hpp"

using namespace std;

struct CPURenderer {
    SDL_Window* win;
    shared_ptr<Image> canvas;

    explicit CPURenderer(SDL_Window* window)
    {
        win = window;
        canvas = std::make_unique<Image>(Image(Game.width, Game.height));
    }

    void DrawFrame() const
    {
        SDL_Surface* windowSurface = SDL_GetWindowSurface(win);
        SDL_BlitSurface(canvas->surface->get(), nullptr, windowSurface, nullptr);
        SDL_UpdateWindowSurface(win);
    }
};

static CPURenderer* renderer = nullptr;

shared_ptr<Image> SetupDefaultRenderer(SDL_Window* window)
{
    if (renderer) {
        throw BasaltException("Renderer already initialized!");
    }
    renderer = new CPURenderer(window);
    return renderer->canvas;
}

void DrawDefaultFrame()
{
    if (!renderer) {
        throw BasaltException("Renderer not initialized!");
    }
    renderer->DrawFrame();
}

void DisposeDefaultRenderer()
{
    if (renderer) {
        delete renderer;
        renderer = nullptr;
    }
}