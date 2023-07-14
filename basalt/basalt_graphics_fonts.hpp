#pragma once
#include <string>

struct Font {
    std::string name;
    static Font Default();
};

Font LoadFont(std::string fontName, unsigned int baseSize = 16);
void DisposeFonts();