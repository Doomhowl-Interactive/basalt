#pragma once
#include <string>

namespace basalt {

using namespace std;

struct Font {
    string name;
    static Font Default();
};

Font LoadFont(string fontName, unsigned int baseSize = 16);
void DisposeFonts();

}  // namespace basalt