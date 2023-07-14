#include <cstdarg>
#include <Windows.h>

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
}

void CloseSystemConsole()
{
    if (AllocatedConsole) {
        FreeConsole();
    }
}

string GetBasaltLog()
{
    return ConsoleLog;
}