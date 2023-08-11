#pragma once
#include <string>
#include <memory>
#include <vector>

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
    std::vector<ReplayEvent> events;

    static std::string GenerateName();
};

extern std::shared_ptr<ReplayRecording> ActiveRecording = nullptr;

// Recordings of input should be started before the first frame of the game!
std::string StartRecordingReplay();
void StopRecordingReplay();

void PlayRecordedReplay();
void StopRecordedReplay();