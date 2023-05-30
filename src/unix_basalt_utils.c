#include <unistd.h>
#include "basalt.h"

static String ConsoleLog = { 0 };

#define MAX_WORKDIR_LEN 128
BASALT const char* GetWorkingDirectory()
{
    static char cwd[MAX_WORKDIR_LEN];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        ERR("Could not determine working directory!");
    }
    return cwd;
}

BASALT void BasaltPrintColored(ConsoleColor color, const char* format, ...)
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

    static const int colors[] = { 30, 37, 90, 94, 92, 96, 91, 35, 33, 97, 93, 95 };
    printf("\e[1;%dm%s\e[0m", colors[color], line);

    AppendString(&ConsoleLog, line);
}

BASALT String GetBasaltLog()
{
    return ConsoleLog;
}

BASALT void OpenSystemConsole()
{
    //
}

BASALT void CloseSystemConsole()
{
    //
}
