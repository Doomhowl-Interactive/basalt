#include <cstdarg>
#include <Windows.h>

#include "basalt_console.hpp"

namespace basalt {

using namespace std;

static string ConsoleLog;
static bool AllocatedConsole = false;

void BasaltPrintColored(ConsoleColor color, const char* format, va_list args)
{
    // allocate console string if not already
    static char line[1024];
    vsnprintf(line, 1024 - 1, format, args);
    strcat(line, "\n");

    static const int colors[] = { 0, 7, 8, 9, 10, 11, 12, 5, 14, 15, 6, 13 };
    if (AllocatedConsole) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colors[color]);
    }
    printf(line);

    ConsoleLog += line;
}

void BasaltPrintColored(ConsoleColor color, const char* format, ...)
{
    BasaltPrintColored(color, format, va_list());
}

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

    const ConsoleColor rainbow[] = { CRED, CRED, CYELLOW, CGREEN, CBLUE, CPINK, CPINK, CPINK };
    for (int i = 0; i < 8; i++) {
        BasaltPrintColored(rainbow[i], logo[i]);
    }
    BasaltPrintColored(CWHITE, "\n>> %s\n", suffix);
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
        ERR("Failed to allocate console!");
    }
    DEBUG("Allocated Windows console");
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

}  // namespace basalt