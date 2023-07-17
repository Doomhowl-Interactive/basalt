#include "basalt_profiler.hpp"
#include "basalt_graphics.hpp"

using namespace std;

void DrawProfiler(Texture canvas)
{
    const auto& data = GetProfilerData();

    constexpr int PADDING = 10;
    constexpr int FONT_SIZE = 16;

    static Color bgColor = ColorAlpha(DARKGRAY, 0.8f);
    static Rect bounds = { 10, 10, 300, 300 };
    static Rect contentBounds = {
        bounds.x + PADDING, bounds.y + PADDING, bounds.x + PADDING * 2, bounds.y + PADDING * 2
    };

    string text = "Frame: " + to_string(data.frameIndex);
    text += "\nFPS: " + to_string((int)data.fps.average());
    canvas.DrawText(text, contentBounds.x, contentBounds.y, YELLOW);
    canvas.DrawRectangle(R2(bounds), bgColor);
}