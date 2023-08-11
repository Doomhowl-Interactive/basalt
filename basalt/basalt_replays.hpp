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

class ReplayRecording {
    public:
        unsigned int seed;
        std::vector<ReplayEvent> events;
        
        // Recordings of input should be started before the first frame of the game!
        void Record();
        void StopRecording();
        void Play();
        void Stop();

        static void RegisterEvent(ReplayEventType type, KeyCode code=0);
    private:
        static std::shared_ptr<ReplayRecording> active;
        static std::string generate_name();

        void register_event(ReplayEventType type, KeyCode code=0);
};
