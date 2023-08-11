#include <string>
#include <memory>
#include <filesystem>

#include "basalt_replays.hpp"

using namespace std;
namespace fs = filesystem;

static fs::path RecordingPath = fs::path("recordings");

fs::path ReplayRecording::generate_path() {
    fs::create_directories(RecordingPath);
    fs::path p;
    int i = 1;
    do {
        p = RecordingPath / FormatText("recording_%d.rec", i++);  
    } while(fs::exists(p));
    return p;
}

void UpdateAndRenderReplayOverlay(Image canvas, float delta) {
    
}