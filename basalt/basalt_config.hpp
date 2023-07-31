#pragma once

struct EngineConfig {
    bool unlockedFramerate;
    bool hasUnitTesting;
    bool hasProfiler;
    bool useSoftware;
};

struct GameConfig {
    const char* title;
    const char* company;
    int width;
    int height;
    char language;
};
extern GameConfig Game;