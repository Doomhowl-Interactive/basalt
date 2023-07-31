#pragma once
#include <memory>

#include "basalt_images.hpp"
#include "basalt_config.hpp"
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
void _HandleFatalException(std::exception e);
int _RunEntryPoint(int argc, char** argv, std::function<int(int, char**)> startFunc);

// Cross platform main function.
// On windows we use the WinMain entry point, on other platforms we use main.
#ifdef WIN32
# define BasaltMain \
     int WINAPI WinMain( \
         HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) \
     { \
         _RunEntryPoint(__argc, __argv, RunGame); \
     }
#else
# define BasaltMain \
     int main(int argc, char** argv) \
     { \
         _RunEntryPoint(argc, argv, RunGame); \
     }
#endif
