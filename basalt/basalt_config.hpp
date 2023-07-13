#pragma once

struct EngineConfig {
    bool hasArchaeo;
    bool hasHotloading;
    bool hasUnitTesting;
    bool hasConsole;
    bool useSoftware;
    bool isHeadless;
    bool unlockedFramerate;
    bool lowQuality;
};
extern EngineConfig Config;

struct GameConfig {
    const char* title;
    const char* company;
    unsigned int width;
    unsigned int height;
    unsigned char language;
};
extern GameConfig Game;