#include <optional>

#include "basalt_macros.hpp"
#include "basalt_profiler.hpp"
#include "basalt_graphics.hpp"
#include "basalt_input.hpp"

using namespace std;

static Rect ProfilerBounds = { 10, 10, 300, 300 };

struct DragContext {
    Point beginMouse;
    Point beginPos;
};

static Rect DragAndDropProfiler(Rect& rect)
{
    static optional<DragContext> ctx = nullopt;

    auto mouse = GetMousePosition();
    if (ctx) {
        auto mouseOffset = mouse - ctx.value().beginMouse;
        auto newPos = ctx.value().beginPos + mouseOffset;
        rect.x = newPos.x;
        rect.y = newPos.y;
        if (IsMouseUp()) {
            ctx = nullopt;
        }
    } else if (IsMouseDown() && mouse.inside(rect)) {
        ctx = { mouse, { rect.x, rect.y } };
    }
    return rect;
}

// move to utils
static string ConcatStrings(const vector<string> lines, const char sep = '\n')
{
    string result;
    for (const auto& line : lines) {
        result += line + "\n";
    }
    result.pop_back();
    return result;
}

void DrawProfiler(Texture canvas)
{
    const auto& data = GetProfilerData();

    constexpr int PADDING = 10;
    constexpr int FONT_SIZE = 12;  // TODO: implement

    static Color bgColor = ColorAlpha(DARKGRAY, 0.5f);

    DragAndDropProfiler(ProfilerBounds);

    Rect contentBounds = { ProfilerBounds.x + PADDING,
                           ProfilerBounds.y + PADDING,
                           ProfilerBounds.x + PADDING * 2,
                           ProfilerBounds.y + PADDING * 2 };

    string text = ConcatStrings({
        "FPS: " + to_string((int)data.fps.average()),
        "Frame: " + to_string(data.frameIndex),
    });

    canvas.DrawText(text, contentBounds.x, contentBounds.y, YELLOW);
    canvas.DrawRectangle(R2(ProfilerBounds), bgColor);
}