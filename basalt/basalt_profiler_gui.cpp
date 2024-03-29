#include <optional>

#include "basalt_macros.hpp"
#include "basalt_profiler.hpp"
#include "basalt_images.hpp"
#include "basalt_input.hpp"
#include "basalt_utils.hpp"

// TODO: When making a seperate GUI library, make this a part of it
// TODO: Rewrite this overall sloppy code.

using namespace std;

static Rect ProfilerBounds = { 10, 10, 300, 300 };
static bool ProfilerOpened = false;
static bool ProfilerAvailable = true;

struct DragContext {
    Point beginMouse;
    Point beginPos;
};

static Rect DragAndDropProfiler(Rect& rect, Rect ignoreSpot)
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
    } else if (IsMousePressed() && mouse.inside(rect) && !mouse.inside(ignoreSpot)) {
        ctx = { mouse, { rect.x, rect.y } };
    }
    return rect;
}

static Rect ResizeProfiler(const Rect& spot, Rect& rect)
{
    static optional<DragContext> ctx = nullopt;

    auto mouse = GetMousePosition();
    if (ctx) {
        auto mouseOffset = mouse - ctx.value().beginMouse;
        auto newSize = ctx.value().beginPos + mouseOffset;
        rect.width = newSize.x;
        rect.height = newSize.y;
        if (IsMouseUp()) {
            ctx = nullopt;
        }
    } else if (IsMousePressed() && mouse.inside(spot)) {
        ctx = { mouse, { rect.width, rect.height } };
    }
    return rect;
}

static Rect DrawProfilerResizer(Image canvas, const Rect& winBounds)
{
    constexpr int SIZE = 10;
    Rect resizer = {
        winBounds.x + winBounds.width - SIZE, winBounds.y + winBounds.height - SIZE, SIZE, SIZE
    };

    static Color col = ColorAlpha(DARKGRAY, 0.7f);
    canvas.DrawRectangle(resizer, col);
    return resizer;
}

void SetProfilerEnabled(bool enabled)
{
    if (!enabled && ProfilerOpened) {
        ProfilerOpened = false;
    }
    ProfilerAvailable = enabled;
}

bool DrawProfiler(Image canvas)
{
    if (IsKeyPressed(KEY_F3) && ProfilerAvailable) {
        ProfilerOpened = !ProfilerOpened;
    }
    if (!ProfilerOpened) {
        return false;
    }

    const auto& data = GetProfilerData();

    constexpr int PADDING = 10;

    static Color bgColor = ColorAlpha(DARKGRAY, 0.5f);
    // static Color bgColor = DARKGRAY;

    Rect contentBounds = { ProfilerBounds.x + PADDING,
                           ProfilerBounds.y + PADDING,
                           ProfilerBounds.width + PADDING * 2,
                           ProfilerBounds.height + PADDING * 2 };

    string text = ConcatStrings({
        data.fps.toString(),
        "Frame: " + to_string(data.frameIndex),
    });

    canvas.DrawRectangle(ProfilerBounds, bgColor);

    canvas.DrawBasaltText(
        text, contentBounds.x, contentBounds.y, YELLOW, Font::Default(), 16, contentBounds.width);

    Rect resizer = DrawProfilerResizer(canvas, ProfilerBounds);
    DragAndDropProfiler(ProfilerBounds, resizer);
    ResizeProfiler(resizer, ProfilerBounds);

    return true;
}