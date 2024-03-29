#pragma once
#include <string>
#include <memory>
#include <vector>
#include <filesystem>

#include "basalt_images.hpp"
#include "basalt_input.hpp"

enum class ReplayEventType : uint8_t { MOUSE_DOWN, MOUSE_UP, KEY_DOWN, KEY_UP };

struct ReplayEvent {
    uint64_t frame;
    ReplayEventType type;
    KeyCode keyCode;
    int mouseIndex;
};

class ReplayRecording {
   public:
    unsigned int seed;
    std::vector<ReplayEvent> events;
    std::filesystem::path filePath;

    // Recordings of input should be started before the first frame of the game!
    void Record();
    void StopRecording();
    void Play();
    void Stop();

    void UpdateAndRender(Image canvas, float delta);
    static void RegisterEvent(ReplayEventType type, KeyCode code = KEY_UNKNOWN, int mouseIndex = 0);

   private:
    static std::shared_ptr<ReplayRecording> active;
    static std::filesystem::path generate_path();

    void register_event(ReplayEventType type, KeyCode code = KEY_UNKNOWN);
};

void UpdateAndRenderReplayOverlay(Image canvas, float delta);