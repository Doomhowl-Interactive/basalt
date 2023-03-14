#include "basalt_extra.h"

typedef enum {
    LEVEL_NORMAL,
    LEVEL_BONUS,
    LEVEL_BOSS
} LevelType;

typedef struct {
    const char* layout;
    uint width;
    uint height;
} LevelLayout;

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
    "02",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_LEVEL003 = {
    "03",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_LEVEL004 = {
    "04",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_LEVEL005 = {
    "05",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_LEVEL006 = {
    "06",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_LEVEL007 = {
    "07",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_LEVEL008 = {
    "08",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_LEVEL009 = {
    "09",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_NORMAL,
};

const static LevelInfo MAP_END_BOSS = {
    "10",
    "UNNAMED LEVEL",
    NULL,
    10000,
    180,
    LEVEL_BOSS,
};

const static LevelInfo MAP_BONUS_CAVE = {
    "A",
    "UNNAMED BONUS LEVEL",
    NULL,
    10000,
    180,
    LEVEL_BONUS,
};

const static LevelInfo MAP_BONUS_PIT = {
    "B",
    "UNNAMED BONUS LEVEL",
    NULL,
    10000,
    180,
    LEVEL_BONUS,
};
