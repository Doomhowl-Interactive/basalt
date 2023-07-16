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
        prof.tasks[name] = ProfilerTask(name, type, builtIn);
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

ProfilerTask::ProfilerTask()
{
    assert(0);
}

void ProfilerTask::addEntry(long frame)
{
    history[frame] = nowInMs() - startTime;
    while (history.size() > PROFILER_HISTORY_SIZE) {
        history.erase(history.begin());
    }
}

double ProfilerTask::latest()
{
    return history.rend()->second;
}

double ProfilerTask::average()
{
    double sum = 0;
    for (auto& it : history) {
        sum += it.second;
    }
    return sum / history.size();
}

double ProfilerTask::max()
{
    double max = 0;
    for (auto& it : history) {
        if (it.second > max) {
            max = it.second;
        }
    }
    return max;
}

double ProfilerTask::min()
{
    double min = 0;
    for (auto& it : history) {
        if (it.second < min) {
            min = it.second;
        }
    }
    return min;
}
