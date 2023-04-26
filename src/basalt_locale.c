#include "basalt.h"

static usize CurrentLanguage = 0;

BASALT usize SwitchLocale(usize id)
{
    usize prevID = CurrentLanguage;
    CurrentLanguage = id;
    return prevID;
}

BASALT const char* LocString(const char* key)
{
    return "???";
}

BASALT const char* MappedLocString(const char* locales, const char* primary)
{
    return "???";
}
