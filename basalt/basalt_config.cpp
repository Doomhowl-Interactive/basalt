#include <spdlog/spdlog.h>
#include <string.h>
#include <vector>

#include "basalt_utils.hpp"
#include "basalt_console.hpp"
#include "basalt_config.hpp"

using namespace std;

//////
// Over-engineered launch system
//////

struct Option {
    bool* toggle;
    std::string shortArg;
    std::string longArg;
    std::string desc;
};

static EngineConfig* Config = nullptr;
static bool NeedsHelp = false;
static bool EnableAll = false;

struct Options : vector<Option> {
    Options(EngineConfig& conf)
    {
        push_back({ &conf.hasUnitTesting, "-t", "--test", "Does unit testing before launch" });
        push_back({ &conf.unlockedFramerate,
                    "-u",
                    "--unlock",
                    "Unlocks the framerate (not recommended)" });

        push_back({ &EnableAll, "-A", "--all", "Enables all of the above." });
        push_back({ &NeedsHelp, "-?", "--help", "Shows this help screen" });
    }
};

static void PrintHelpInfo()
{
    for (const auto& option : Options(*Config)) {
        auto padding = PadStringRight(option.longArg, ' ', 30);
        spdlog::info("{} {}{}", option.shortArg, padding, option.desc);
    }
}

static bool HasArgument(int argc, char** argv, const Option& o)
{
    for (int i = 0; i < argc; i++) {
        if (o.shortArg == argv[i]) {
            return true;
        }

        if (o.longArg == argv[i]) {
            return true;
        }
    }
    return false;
}

bool ParseLaunchArguments(EngineConfig* config, int argc, char** argv)
{
    Config = config;
    for (const auto& o : Options(*Config)) {
        if (HasArgument(argc, argv, o)) {
            if (o.toggle != nullptr)
                *(o.toggle) = true;
        }
    }

    if (NeedsHelp) {
        PrintHelpInfo();
        return false;
    }

    if (EnableAll) {
        memset(Config, 1, sizeof(EngineConfig));
    }

    return true;
}
