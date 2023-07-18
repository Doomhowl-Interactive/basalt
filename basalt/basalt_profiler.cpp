#include <chrono>
#include <cassert>
#include <optional>

#include "basalt_profiler.hpp"
#include "basalt_window.hpp"
#include "basalt_utils.hpp"

using namespace std;

static ProfilerData prof = ProfilerData();

static double nowInMs()
{
    return chrono::duration_cast<chrono::milliseconds>(
               chrono::system_clock::now().time_since_epoch())
        .count();
}

inline static bool IsOutlier(double value)
{
    // HACK: Don't include data thats ridiculously big or small
    return abs(value) > 100000;
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
    auto fps = GetFramesPerSecond();
    prof.frameIndex++;
    prof.fps.addCustomEntry(prof.frameIndex, fps);
}

ProfilerData::ProfilerData() : fps("FPS", TASK_PROCESS, true)
{
    frameIndex = 0;
    tasks = unordered_map<string, ProfilerTask>();
}

ProfilerTask::ProfilerTask(string name, ProfilerTaskType type, bool builtIn)
    : name(name), type(type), builtIn(builtIn)
{
    history = deque<HistoryItem>();
}

void ProfilerTask::addEntry(long frame)
{
    auto diff = nowInMs() - startTime;
    addCustomEntry(frame, diff);
}

void ProfilerTask::addCustomEntry(long frame, double value)
{
    if (IsOutlier(value)) {
        return;
    }

    HistoryItem elem = { frame, value };
    history.push_front(elem);

    // cleanup old data
    while (history.size() > PROFILER_HISTORY_SIZE) {
        history.pop_back();
    }
}

double ProfilerTask::latest() const
{
    if (history.empty()) {
        return 0.0;
    }
    return history.back().value;
}

double ProfilerTask::average() const
{
    double sum = 0;
    for (const auto& it : history) {
        sum += it.value;
    }
    if (history.empty()) {
        return sum;
    }
    return sum / history.size();
}

double ProfilerTask::max() const
{
    double max = 0;
    for (const auto& it : history) {
        if (it.value > max) {
            max = it.value;
        }
    }
    return max;
}

double ProfilerTask::min() const
{
    double min = 0;
    for (const auto& it : history) {
        if (it.value < min) {
            min = it.value;
        }
    }
    return min;
}

string ProfilerTask::toString() const
{
    return FormatText("%s : %.3f (min %.1f | avg: %.1f | max: %.1f)",
                      name.c_str(),
                      latest(),
                      min(),
                      average(),
                      max());
}
