#include <string>
#include <memory>
#include <fileysystem>

#include "basalt_replays.hpp"

using namespace std;
namespace fs = filesystem;

constexpr fs::path RecordingPath = "recordings";

fs::path ReplayRecording::generate_path() {
    fs::create_directories(RecordingPath);
    fs::path p;
    int i = 1;
    do {
        p = RecordingPath / FormatText("recording_%d.rec", i++);  
    } while(fs::exists(p))
    return p;
}

void UpdateAndRenderReplayOverlay(Texture canvas, float delta) {
    
}