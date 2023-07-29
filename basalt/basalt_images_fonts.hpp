#pragma once
#include <string>

#include "basalt_math.hpp"
#include "basalt_macros.hpp"

typedef uint32_t Color;

struct Font {
    std::string name;
    static Font Default();
    Point MeasureString(std::string text, int fontSize = 32);
};

struct FontStyle {
    Color color = 0xFFFFFFFF;
    uint size = 32;
    uint maxWidth = INT16_MAX;
    bool centered = false;

    FontStyle(Color color = 0xFFFFFFFF,
              uint size = 32,
              bool centered = false,
              uint maxWidth = INT16_MAX);

    FontStyle(const FontStyle& st);
    size_t Hash(std::string ofText) const;

    // returns copy with overriden property
    FontStyle center(bool centered = true) const;
    FontStyle wColor(Color color) const;
    FontStyle wSize(int size) const;
    FontStyle wMaxWidth(int maxWidth) const;
};

Font LoadFont(std::string fontName);
void DisposeFonts();