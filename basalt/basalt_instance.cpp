#include <spdlog/spdlog.h>
#include "basalt_instance.hpp"

using namespace std;

int _RunEntryPoint(int argc, char** argv, function<int(int, char**)> startFunc)
{
    try {
        return startFunc(argc, argv);
    } catch (exception e) {
        _HandleFatalException(e);
        return -1;
    }
}

void _HandleFatalException(std::exception e)
{
    spdlog::error("Fatal exception: {}", e.what());
    exit(EXIT_FAILURE);
}