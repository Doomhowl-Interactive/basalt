#include "basalt_extra.h"

typedef enum {
    LEVEL_NORMAL,
    LEVEL_BONUS,
    LEVEL_BOSS
} LevelType;

typedef struct {

} LevelLayout;

typedef struct {
} LevelObjectives;

typedef struct {
    const char* shortName;
    const char* displayName;
    const LevelLayout* layout;
    uint minScore;
    uint maxTime;
    LevelType type;
} LevelInfo;

const static LevelInfo MAP_LEVEL001 = {
    "01",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_LEVEL002 = {
    "01",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_LEVEL003 = {
    "01",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_LEVEL004 = {
    "01",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_LEVEL005 = {
    "01",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_LEVEL006 = {
    "01",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_LEVEL007 = {
    "01",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_LEVEL008 = {
    "01",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_LEVEL009 = {
    "01",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_END_BOSS = {
    "01",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_BOSS,
};

const static LevelInfo MAP_BONUS_CAVE = {
    "01",
    "UNNAMED BONUS LEVEL",
    NULL,
    10000,
    180,
    LEVEL_BONUS,
};

const static LevelInfo MAP_BONUS_PIT = {
    "01",
    "UNNAMED BONUS LEVEL",
    NULL,
    10000,
    180,
    LEVEL_BONUS,
};
