#include <chrono>
#include <cassert>
#include "basalt_profiler.hpp"

using namespace std;

static ProfilerData prof = ProfilerData();

static double nowInMs()
{
    return chrono::duration_cast<chrono::milliseconds>(
               chrono::system_clock::now().time_since_epoch())
        .count();
}

string& BeginProfilerTask(string name, ProfilerTaskType type, bool builtIn)
{
    // get or create the task
    auto it = prof.tasks.find(name);
    if (it == prof.tasks.end()) {
        ProfilerTask task = ProfilerTask(name, type, builtIn);
        prof.tasks.emplace(name, task);
        it = prof.tasks.find(name);
    }
    it->second.startTime = nowInMs();
    return name;
}

void EndProfilerTask(string name)
{
    auto it = prof.tasks.find(name);
    if (it != prof.tasks.end()) {
        it->second.addEntry(prof.frameIndex);
    }
}

ProfilerData& GetProfilerData()
{
    return prof;
}

void UpdateProfiler()
{
    prof.frameIndex++;
    prof.fps.addEntry(prof.frameIndex);
}

ProfilerData::ProfilerData() : fps("FPS", TASK_PROCESS, true)
{
    frameIndex = 0;
    tasks = unordered_map<string, ProfilerTask>();
}

ProfilerTask::ProfilerTask(string name, ProfilerTaskType type, bool builtIn)
    : name(name), type(type), builtIn(builtIn)
{
    history = map<long, double>();
}

void ProfilerTask::addEntry(long frame)
{
    history[frame] = nowInMs() - startTime;
    while (history.size() > PROFILER_HISTORY_SIZE) {
        history.erase(history.begin());
    }
}

double ProfilerTask::latest() const
{
    return history.rend()->second;
}

double ProfilerTask::average() const
{
    double sum = 0;
    for (const auto& it : history) {
        sum += it.second;
    }
    return sum / history.size();
}

double ProfilerTask::max() const
{
    double max = 0;
    for (const auto& it : history) {
        if (it.second > max) {
            max = it.second;
        }
    }
    return max;
}

double ProfilerTask::min() const
{
    double min = 0;
    for (const auto& it : history) {
        if (it.second < min) {
            min = it.second;
        }
    }
    return min;
}
