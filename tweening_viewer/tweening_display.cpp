#include <functional>
#include <string>
#include <unordered_map>

#include "basalt.h"

using namespace std;

typedef std::function<float(float, float, float)> TweeningFunction;

constexpr uint GRAPH_PADDING = 10;
constexpr uint GRAPH_SIZE = 100;
constexpr uint MARGIN_BOTTOM = 10;

#define TWEEN_ITEM(X) \
 { \
#  X, X \
 }

static const unordered_map<string, TweeningFunction> tweeningFunctions = {
    TWEEN_ITEM(TweenLinear),
    TWEEN_ITEM(TweenSmooth),
};

class TweeningDisplay {
   public:
    TweeningDisplay(Texture canvas) : canvas(canvas)
    {
    }

    void UpdateAndRenderTweenings(float delta)
    {
        int y = 0;
        for (const auto& item : tweeningFunctions) {
            DrawTweening(canvas, y, item);
            y += GRAPH_SIZE + GRAPH_PADDING;
        }
    }

   private:
    Texture canvas;

    void DrawGraph(int yOffset, TweeningFunction func)
    {
        Rect graphRect = { GRAPH_PADDING, GRAPH_PADDING, GRAPH_SIZE, GRAPH_SIZE };
        graphRect.y += yOffset;

        canvas.DrawRectangle(R2(graphRect), WHITE);
        Point leftBotCorner = { graphRect.x, graphRect.y + graphRect.height };
        canvas.DrawLine(P2(graphRect), P2(leftBotCorner), DARKGRAY);
        Point rightBotCorner = { graphRect.x + graphRect.width, graphRect.y + graphRect.height };
        canvas.DrawLine(P2(rightBotCorner), P2(leftBotCorner), DARKGRAY);

        for (int i = 0; i < graphRect.width; i++) {
            float perc = (float)i / (float)graphRect.width;
            float value = func(perc, 0.f, 1.f);
            int y = graphRect.y + graphRect.height - (int)(value * graphRect.height);
            canvas.DrawDot(graphRect.x + i, y, BLUE);
        }
    }

    void DrawTweening(Texture canvas, int yOffset, const pair<string, TweeningFunction>& item)
    {
        DrawGraph(yOffset, item.second);

        // Draw title
        FontStyle titleStyle;
        titleStyle.size = 16;
        int x = GRAPH_SIZE + GRAPH_PADDING * 2;
        int y = yOffset + GRAPH_PADDING + GRAPH_SIZE / 2;
        canvas.DrawBasaltTextShadow(
            item.first, x, y, Font::Default(), titleStyle, titleStyle.wColor(DARKGRAY));
    }
};
