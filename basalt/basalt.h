#pragma once

#include "basalt_macros.hpp"
#include "basalt_config.hpp"
#include "basalt_console.hpp"
#include "basalt_graphics_fonts.hpp"
#include "basalt_graphics.hpp"
#include "basalt_input.hpp"
#include "basalt_math.hpp"
#include "basalt_utils.hpp"
#include "basalt_window.hpp"

// TODO: Replace old logging with spdlog

struct Basalt {
    int exitCode;
    ulong frameIndex;
    double timeElapsed;
    Texture canvas;

    Basalt(GameConfig config, int argc, char** argv);
    ~Basalt();

    bool ShouldClose();
    void BeginFrame();
    void EndFrame();
};