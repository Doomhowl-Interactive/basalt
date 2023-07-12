#pragma once

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

#define INFO(...) BasaltPrintColored(CWHITE, "INFO  : " __VA_ARGS__)
#define WARN(...) BasaltPrintColored(CYELLOW, "WARN  : " __VA_ARGS__)
#define ERR(...) BasaltPrintColored(CRED, "ERROR : " __VA_ARGS__)
#define FATAL(...) BasaltPrintColored(CPURPLE, "FATAL : " __VA_ARGS__)
#define DEBUG(...) BasaltPrintColored(CLGRAY, "DEBUG : " __VA_ARGS__)