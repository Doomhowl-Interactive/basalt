#include <basalt.h>
#include "example_game.hpp"

void DrawTrippyPattern(Image canvas, int x, int y, int count) {
    constexpr int SIZE = 100;
    constexpr int SPACING = 20;
    
    float seg = 1.0f / count;
    for (int i = 0; i < count; i++) {
        
        float f = seg * i + (float) GetTimeElapsed();
        Color tint = InterpolateHue(f);
        tint = ColorAlpha(tint, 0.5f);
        canvas.DrawRectangle(x,y,SIZE,SIZE, tint);
        
        x += SPACING;
        y += SPACING;
    }
}
