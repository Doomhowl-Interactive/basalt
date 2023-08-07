#include <string>

#include "basalt_replays.hpp"
#include "basalt_input.hpp"

using namespace std;

enum class ReplayEventType : uint8_t {
    MOUSE_DOWN,
    MOUSE_UP,
    KEY_DOWN,
    KEY_UP
};

struct ReplayEvent {
    uint64_t frame;
    ReplayEventType type;
    KeyCode keyCode;
};

struct ReplayRecording {
    unsigned int seed;
    uint64_t eventCount;
    ReplayEvent* events;
};

static string GenerateRecordingName();

void StartRecordingReplay() {
    
}

void StopRecordingReplay() {
    
}

void PlayRecordedReplay() {
    
}

void StopRecordedReplay() {
    
}

static string GenerateRecordingName() {
    return "TODO";
}
