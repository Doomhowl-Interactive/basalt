#include "basalt.h"
#include <string.h>

//////
// Over-engineered launch system 
//////

EngineConfig Config = { 0 };

class(Option) {
    bool* toggle;
    const char* shortArg;
    const char* longArg;
    const char* desc;
};

static bool NeedsHelp = false;
static bool EnableAll = false;

static Option Options[] = {
    { &Config.hasArchaeo, "-a", "--archaeo", "Enables tooling to inspect draw calls. (EXPENSIVE)" },
    { &Config.hasHotloading, "-h", "--hotloading", "Enables hot-reloading if original asset folder is found." },
    { &Config.hasUnitTesting, "-t", "--test", "Does unit testing before launch" },
    { &Config.hasConsole, "-s", "--show", "Open console logs (Windows only)" },
    { &Config.unlockedFramerate, "-u", "--unlock", "Unlocks the framerate (not recommended)" },

    { &EnableAll, "-A", "--all", "Enables all of the above." },
    { &NeedsHelp, "-?", "--help", "Shows this help screen" },

    { NULL, NULL }
};

func usize GetOptionCount()
{
    usize count = 0;
    for (Option* opt = Options; opt->shortArg != NULL; opt++)
        count++;

    return count;
}

func void PrintHelpInfo()
{
    for (Option* o = Options; o->shortArg != NULL; o++)
    {
        const char* padding = PadStringRight(o->longArg, ' ', 30);
        INFO("%s %s%s",o->shortArg,padding,o->desc);
    }
}

func bool HasArgument(int argc, char** argv, Option o)
{
    for (int i = 0; i < argc; i++)
    {
        if (o.shortArg != NULL && strcmp(argv[i],o.shortArg) == 0)
            return true;

        if (o.longArg != NULL && strcmp(argv[i],o.longArg) == 0)
            return true;
    }
    return false;
}

pubfunc bool ParseLaunchArguments(int argc, char** argv)
{
    for (Option* o = Options; o->shortArg != NULL; o++)
    {
        if (HasArgument(argc, argv, *o))
        {
            if (o->toggle != NULL)
                *(o->toggle) = true;
        }
    }

    if (NeedsHelp)
    {
        PrintHelpInfo();
        return false;
    }

    if (EnableAll)
        memset(&Config, 1, sizeof(EngineConfig));

    return true;
}