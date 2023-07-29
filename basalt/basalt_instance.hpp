#pragma once
#include <memory>

#include "basalt_instance.hpp"

// TODO: Replace old logging with spdlog
struct Basalt {
    int exitCode;

    Basalt(GameConfig config, int argc, char** argv);
    ~Basalt();

    void Close(int code);
    bool ShouldClose();
    std::shared_ptr<Image> BeginFrame();
    void EndFrame();
};

// Opens the crash handler in distributed builds
// Write a try/catch block around your main game function and pass the throwed exception into here.
void HandleFatalException(std::exception e);
