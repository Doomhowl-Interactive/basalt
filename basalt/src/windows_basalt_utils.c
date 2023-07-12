#include <windows.h>
#include <stdio.h>
#include <string>

#include "basalt.h"
#include "basalt_console.hpp"

using namespace std;

static std::string ConsoleLog;
static bool AllocatedConsole = false;

// TODO HACK: Use C++ 17 filesystem
const char* GetWorkingDirectory()
{
    static char workingDir[MAX_PATH];
    DWORD workingDirU[MAX_PATH];
    int len = GetModuleFileName(NULL, workingDirU, MAX_PATH);
    if (len == 0) {
        ERR("Could not determine working directory %s", GetLastError());
    }
    sprintf(workingDir, "%ws", workingDirU);
    // cut off the executable name
    char* lastSlash = strrchr(workingDir, '\\');
    if (lastSlash != NULL) {
        *lastSlash = '\0';
    }
    return workingDir;
}

void BasaltPrintColored(ConsoleColor color, const char* format, ...)
{
    // allocate console string if not already
    if (ConsoleLog.text == NULL) {
        String str = MakeString();
        memcpy(&ConsoleLog, &str, sizeof(String));
    }

    static char line[1024];
    va_list list;
    va_start(list, format);
    vsnprintf(line, 1024 - 1, format, list);
    va_end(list);
    strcat(line, "\n");

    static const int colors[] = { 0, 7, 8, 9, 10, 11, 12, 5, 14, 15, 6, 13 };
    if (AllocatedConsole) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colors[color]);
    }
    printf(line);

    AppendString(&ConsoleLog, line);
}

String GetBasaltLog()
{
    return ConsoleLog;
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
