#pragma once
#include <string>
#include "basalt_utils.hpp"

namespace basalt {

enum ConsoleColor {
    CBLACK,
    CLGRAY,
    CDGRAY,
    CBLUE,
    CGREEN,
    CYAN,
    CRED,
    CPURPLE,
    CYELLOW,
    CWHITE,
    CORANGE,
    CPINK,
};

// Cross platform println functions
// All text passed to this function will by stored in a buffer, allowing the possibility to render
// console output into your game or easily dump it to a file.
void BasaltPrintColored(ConsoleColor color, const char* format, va_list args);
void BasaltPrintColored(ConsoleColor color, const char* format, ...);

#define INFO(...) BasaltPrintColored(CWHITE, "INFO  : " __VA_ARGS__);

#define WARN(...) BasaltPrintColored(CYELLOW, "WARN  : " __VA_ARGS__);
#define ERR(...) BasaltPrintColored(CRED, "ERROR : " __VA_ARGS__);
#define FATAL(...) BasaltPrintColored(CPURPLE, "FATAL : " __VA_ARGS__);
#define DEBUG(...) BasaltPrintColored(CLGRAY, "DEBUG : " __VA_ARGS__);

void PrintASCIILogo(string suffix);
void OpenSystemConsole();
void CloseSystemConsole();

std::string GetBasaltLog();

}  // namespace basalt