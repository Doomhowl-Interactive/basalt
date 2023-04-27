#include "basalt.h"
#include "bullet_common.h"
#define BACKGROUND
#define SCHEDULE

#define MAX_SCHEDULED_ITEMS 256

struct LevelSchedule;
typedef void (*BackgroundRenderFunc)(Texture canvas, float delta);
typedef void (*LevelSchedulerFunc)(int difficulty);
typedef void (*EntityInitializerFunc)(Entity* e);

typedef struct LevelScheduleItem {
    double delayTime;
    EntityInitializerFunc initFunc;
    Vec2 position;
    const char* description;
} LevelScheduleItem;

typedef struct LevelSchedule {
    LevelScheduleItem items[MAX_SCHEDULED_ITEMS];
    usize curIndex;
    usize itemCount;
} LevelSchedule;

typedef struct LevelInfo {
    int number;
    const char* name;
    LevelSchedulerFunc schedulerFunc;
    BackgroundRenderFunc backgroundFunc;
} LevelInfo;

extern const LevelInfo Level1;

BULLET void SwitchLevel(const LevelInfo* level);
BULLET bool UpdateAndRenderLevel(Texture canvas, Scene* scene, float delta);
BULLET void ScheduleEntity(LevelSchedule* schedule, Vec2 position);

// Level backgrounds
BACKGROUND void ScrollingPurpleNoise(Texture canvas, float delta);

// Level schedulers
SCHEDULE void TrainingLevelLayout(int difficulty);
