#include <spdlog/spdlog.h>
#include <string.h>
#include <vector>

#include "basalt_utils.hpp"
#include "basalt_console.hpp"
#include "basalt_config.hpp"

namespace basalt {
using namespace std;

//////
// Over-engineered launch system
//////

EngineConfig Config = { 0 };

struct Option {
    bool* toggle;
    string shortArg;
    string longArg;
    string desc;
};

static bool NeedsHelp = false;
static bool EnableAll = false;

struct Options : vector<Option> {
    Options()
    {
        push_back({ &Config.hasArchaeo,
                    "-a",
                    "--archaeo",
                    "Enables tooling to inspect draw calls. (EXPENSIVE)" });

        push_back({ &Config.hasHotloading,
                    "-h",
                    "--hotloading",
                    "Enables hot-reloading if original asset folder is found." });
        push_back({ &Config.hasUnitTesting, "-t", "--test", "Does unit testing before launch" });
        push_back({ &Config.hasConsole, "-s", "--show", "Open console logs (Windows only)" });
        push_back({ &Config.unlockedFramerate,
                    "-u",
                    "--unlock",
                    "Unlocks the framerate (not recommended)" });
        push_back({ &Config.isHeadless,
                    "-H",
                    "--headless",
                    "Hot reloads game code from a dynamic library" });
        push_back({ &Config.lowQuality,
                    "-l",
                    "--low",
                    "Decreases visual effects for higher performance." });

        push_back({ &EnableAll, "-A", "--all", "Enables all of the above." });
        push_back({ &NeedsHelp, "-?", "--help", "Shows this help screen" });
    }
};

static void PrintHelpInfo()
{
    for (const auto& option : Options()) {
        auto padding = PadStringRight(option.longArg, ' ', 30);
        spdlog::info("{} {}{}", option.shortArg, padding, option.desc);
    }
}

static bool HasArgument(int argc, char** argv, Option o)
{
    for (int i = 0; i < argc; i++) {
        if (o.shortArg != NULL && TextIsEqual(argv[i], o.shortArg)) {
            return true;
        }

        if (o.longArg != NULL && TextIsEqual(argv[i], o.longArg)) {
            return true;
        }
    }
    return false;
}

bool ParseLaunchArguments(int argc, char** argv)
{
    for (Option* o = Options; o->shortArg != NULL; o++) {
        if (HasArgument(argc, argv, *o)) {
            if (o->toggle != NULL)
                *(o->toggle) = true;
        }
    }

    if (NeedsHelp) {
        PrintHelpInfo();
        return false;
    }

    if (EnableAll) {
        memset(&Config, 1, sizeof(EngineConfig));
    }

    return true;
}

}  // namespace basalt