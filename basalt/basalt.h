#pragma once
#undef DrawText  // TODO: Use DrawBasaltText instead

#include "basalt_macros.hpp"
#include "basalt_config.hpp"
#include "basalt_console.hpp"
#include "basalt_graphics_fonts.hpp"
#include "basalt_graphics.hpp"
#include "basalt_assets.hpp"
#include "basalt_input.hpp"
#include "basalt_math.hpp"
#include "basalt_utils.hpp"
#include "basalt_window.hpp"

// TODO: Replace old logging with spdlog
struct Basalt {
    int exitCode;

    Basalt(GameConfig config, int argc, char** argv);
    ~Basalt();

    bool ShouldClose();
    Texture BeginFrame();
    void EndFrame();

    // usage is discouraged
    static Basalt* GetInstance();
};

// Opens the crash handler in distributed builds
// Write a try/catch block around your main game function and pass the throwed exception into here.
void HandleFatalException(std::exception e);
