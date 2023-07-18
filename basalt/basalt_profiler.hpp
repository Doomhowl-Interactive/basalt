#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <map>

#include "basalt_graphics.hpp"

constexpr int PROFILER_HISTORY_SIZE = 100;

enum ProfilerTaskType {
    TASK_PROCESS,
    TASK_RENDER_CPU,
    TASK_RENDER_GPU,
    TASK_DISK,
    TASK_IDLE,
};

class ProfilerTask {
   public:
    std::string name;
    bool builtIn;
    ProfilerTaskType type;
    std::map<long, double> history;
    double startTime;

    ProfilerTask(std::string name, ProfilerTaskType type, bool builtIn);

    double latest() const;
    double average() const;
    double max() const;
    double min() const;

    void addEntry(long frame);
    void addCustomEntry(long frame, double value);

   private:
    void cleanup();
};

struct ProfilerData {
    long frameIndex;
    ProfilerTask fps;
    std::unordered_map<std::string, ProfilerTask> tasks;

    ProfilerData();
};

std::string& BeginProfilerTask(std::string name,
                               ProfilerTaskType type = TASK_PROCESS,
                               bool builtIn = false);
void EndProfilerTask(std::string name);

ProfilerData& GetProfilerData();
void UpdateProfiler();
void DrawProfiler(Texture canvas);

#ifndef NO_PROFILER
# define BeginMeasurement(X) BeginProfiling(X)
# define EndMeasurement(X) EndMeasurementImpl(X)
#else
# define BeginMeasurement(X)
# define EndMeasurement(X)
#endif
