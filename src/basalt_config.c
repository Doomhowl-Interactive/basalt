#include "basalt.h"

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

static Option Options[] = {
    { &Config.hasArchaeo, "-a", "--archaeo", "Enables tooling to inspect draw calls. (EXPENSIVE)" },
    { &Config.hasHotloading, "-h", "--hotloading", "Enables hot-reloading if original asset folder is found." },
    { &Config.unlockedFramerate, "-u", "--unlock", "Unlocks the framerate (not recommended)" },
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
        usize spaces = 30 - strlen(o->longArg);
        char* padding = malloc(spaces+1);
        memset(padding, ' ', spaces);
        padding[spaces-1] = '\0';

        INFO("%s %s%s%s",o->shortArg,o->longArg, padding,o->desc);

        free(padding);
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

    return true;
}
