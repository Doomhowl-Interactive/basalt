#include <functional>
#include <string>
#include <unordered_map>

#include "basalt.h"

using namespace std;

typedef std::function<float(float, float, float)> TweeningFunction;

constexpr uint GRAPH_PADDING = 10;
constexpr uint GRAPH_SIZE = 100;
constexpr uint PREVIEW_SIZE = 50;
constexpr uint MARGIN_BOTTOM = 10;
constexpr float END_DELAY = 1.f;

#define TWEEN_ITEM(X) \
 { \
#  X, X \
 }

static const unordered_map<string, TweeningFunction> tweeningFunctions
    = { TWEEN_ITEM(TweenLinear), TWEEN_ITEM(TweenSmooth), TWEEN_ITEM(TweenSmooth2),
        TWEEN_ITEM(TweenExp),    TWEEN_ITEM(TweenExp2),   TWEEN_ITEM(TweenInvExp),
        TWEEN_ITEM(TweenInvExp2) };

class TweeningDisplay {
   public:
    TweeningDisplay(Texture canvas) : canvas(canvas)
    {
    }

    void UpdateAndRenderTweenings(float delta, float yOffset = 0.f)
    {
        int y = yOffset;
        for (const auto& item : tweeningFunctions) {
            DrawTweening(canvas, y, item);
            y += GRAPH_SIZE + GRAPH_PADDING;
        }
        timer += delta;
        if (timer > 1.f + END_DELAY) {
            timer -= 1.f + END_DELAY;
        }
    }

   private:
    Texture canvas;
    float timer = 0.f;

    void DrawPreview(float progress, int yOffset, TweeningFunction func)
    {
        int x = Game.width - GRAPH_PADDING * 3 - PREVIEW_SIZE;
        float scalar = func(progress, 0.f, 1.f) * PREVIEW_SIZE;
        int y = yOffset + GRAPH_PADDING * 2 + 20 + PREVIEW_SIZE / 2;
        Rect previewRect = { x - scalar / 2, y - scalar / 2, scalar, scalar };
        Color col = MakeRGBf(0.f, 0.f, progress, 1.f);
        canvas.DrawRectangle(R2(previewRect), col);
    }

    void DrawPreview2(float progress, int yOffset, TweeningFunction func)
    {
        float xOffset = func(progress, 0.f, 300.f);
        int x = Game.width / 1.8f - GRAPH_PADDING * 3 - PREVIEW_SIZE + xOffset;
        int y = yOffset + GRAPH_PADDING * 2 + 20;
        Rect previewRect = { x, y, PREVIEW_SIZE, PREVIEW_SIZE };
        Color col = MakeRGBf(progress, 0.f, 0.f, 1.f);
        canvas.DrawRectangle(R2(previewRect), col);
    }

    void DrawGraph(float progress, int yOffset, TweeningFunction func)
    {
        Rect graphRect = { GRAPH_PADDING, GRAPH_PADDING, GRAPH_SIZE, GRAPH_SIZE };
        graphRect.y += yOffset;

        canvas.DrawRectangle(R2(graphRect), WHITE);
        Point leftBotCorner = { graphRect.x, graphRect.y + graphRect.height };
        canvas.DrawLine(P2(graphRect), P2(leftBotCorner), DARKGRAY);
        Point rightBotCorner = { graphRect.x + graphRect.width, graphRect.y + graphRect.height };
        canvas.DrawLine(P2(rightBotCorner), P2(leftBotCorner), DARKGRAY);

        for (int x = 0; x < graphRect.width; x++) {
            float perc = (float)x / (float)graphRect.width;
            int y = (int)func(1.f - perc, graphRect.y, graphRect.y + graphRect.height);
            canvas.DrawDot(x + graphRect.x, y, BLUE);
        }

        constexpr int SIZE = 4;
        float x = progress * graphRect.width + graphRect.x;
        float y = func(1.f - progress, graphRect.y, graphRect.y + graphRect.height);
        Rect marker = { x - SIZE / 2, y - SIZE / 2, SIZE, SIZE };

        canvas.DrawRectangle(R2(marker), RED);
    }

    void DrawTweening(Texture canvas, int yOffset, const pair<string, TweeningFunction>& item)
    {
        float progress = Clamp(timer, 0.f, 1.f);
        DrawGraph(progress, yOffset, item.second);
        DrawPreview(progress, yOffset, item.second);
        DrawPreview2(progress, yOffset, item.second);

        // Draw title
        FontStyle titleStyle;
        titleStyle.size = 16;
        int x = GRAPH_SIZE + GRAPH_PADDING * 2;
        int y = yOffset + GRAPH_PADDING + GRAPH_SIZE / 2;
        canvas.DrawBasaltTextShadow(
            item.first, x, y, Font::Default(), titleStyle, titleStyle.wColor(DARKGRAY));
    }
};
