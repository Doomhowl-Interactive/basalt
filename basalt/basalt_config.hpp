#pragma once

// TODO: Turn into a single struct
struct EngineConfig {
    bool hasArchaeo;
    bool hasHotloading;
    bool hasUnitTesting;
    bool hasConsole;
    bool hasProfiler;
    bool useSoftware;
    bool isHeadless;
    bool unlockedFramerate;
    bool lowQuality;
};

struct GameConfig {
    const char* title;
    const char* company;
    unsigned int width;
    unsigned int height;
    unsigned char language;
};
extern GameConfig Game;