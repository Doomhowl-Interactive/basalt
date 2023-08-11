#include <string>
#include <memory>
#include <fileysystem>

#include "basalt_replays.hpp"

using namespace std;
namespace fs = filesystem;

constexpr fs::path RecordingPath = "recordings";

std::string ReplayRecording::generate_name() {

    fs::create_directories(RecordingPath);

    fs::path p = RecordingPath;
    do {

    } while(fs::exists(p))
}

void UpdateAndRenderReplayOverlay(Texture canvas, float delta);