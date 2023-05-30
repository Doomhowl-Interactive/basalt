#include <windows.h>
#include "basalt.h"

static String ConsoleLog = {0};
static bool AllocatedConsole = false;

#define MAX_WORKDIR_LEN 128
#define MAX_WORKDIR_LEN 128
BASALT const char* GetWorkingDirectory()
{
    static char workingDir[MAX_WORKDIR_LEN];
    if (!GetCurrentDirectory(MAX_WORKDIR_LEN, workingDir)) {
        ERR("Could not determine working directory %s", GetLastError());
    }
    return workingDir;
}

BASALT void BasaltPrintColored(ConsoleColor color, const char* format, ... )
{
    // allocate console string if not already
    if (ConsoleLog.text == NULL)
    {
        String str = MakeString();
        memcpy(&ConsoleLog, &str, sizeof(String));
    }

    static char line[1024];
    va_list list;
    va_start(list, format);
    vsnprintf(line, 1024 - 1, format, list);
    va_end(list);
    strcat(line,"\n");

    static const int colors[] = {0,7,8,9,10,11,12,5,14,15,6,13};
    if (AllocatedConsole)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colors[color]);
    }
    printf(line);

    AppendString(&ConsoleLog, line);
}

BASALT String GetBasaltLog()
{
    return ConsoleLog;
}

BASALT void OpenSystemConsole()
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

BASALT void CloseSystemConsole()
{
    if (AllocatedConsole)
    {
        FreeConsole();
    }
}
