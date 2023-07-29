#include <cstdarg>

#ifdef WIN32
# include <Windows.h>
#endif

#include "spdlog/spdlog.h"
#include "basalt_console.hpp"

using namespace std;

static string ConsoleLog;
static bool AllocatedConsole = false;

void PrintASCIILogo(string suffix)
{
    static const char* logo[] = {
        " ________  ________  ________  ________  ___   __________    ",
        " |\\   __  \\|\\   __  \\|\\   ____\\|\\   __  \\|\\  \\ |\\___   ___\\  ",
        " \\ \\  \\|\\ /\\ \\  \\|\\  \\ \\  \\___|\\ \\  \\|\\  \\ \\  \\\\|___ \\  \\_|  ",
        "  \\ \\   __  \\ \\   __  \\ \\_____  \\ \\   __  \\ \\  \\    \\ \\  \\   ",
        "   \\ \\  \\|\\  \\ \\  \\ \\  \\|____|\\  \\ \\  \\ \\  \\ \\  \\____\\ \\  \\  ",
        "    \\ \\_______\\ \\__\\ \\__\\____\\_\\  \\ \\__\\ \\__\\ \\_______\\ \\__\\ ",
        "     \\|_______|\\|__|\\|__|\\_________\\|__|\\|__|\\|_______|\\|__| ",
        "                         \\|_________|                        ",
    };

    for (int i = 0; i < 8; i++) {
        spdlog::info(logo[i]);
    }
    spdlog::info(">> {}", suffix);
}

void OpenSystemConsole()
{
#ifdef WIN32
    if (!AllocatedConsole) {
        AllocConsole();
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        AllocatedConsole = true;
    } else {
        spdlog::error("Failed to allocate console!");
    }
    spdlog::debug("Allocated Windows console");
#else
    spdlog::error("Custom console only supported on Windows.");
#endif
}

void CloseSystemConsole()
{
#ifdef WIN32
    if (AllocatedConsole) {
        FreeConsole();
    }
#endif
}

string GetBasaltLog()
{
    return ConsoleLog;
}