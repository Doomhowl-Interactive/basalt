#include <string>
#include <unordered_map>
#include <functional>

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
    TweeningDisplay(Image canvas) : canvas(canvas)
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
    Image canvas;
    float timer = 0.f;

    void DrawPreview(float progress, int yOffset, TweeningFunction func)
    {
        float x = Game.width - GRAPH_PADDING * 3 - PREVIEW_SIZE;
        float scalar = func(progress, 0.f, 1.f) * PREVIEW_SIZE;
        float y = yOffset + GRAPH_PADDING * 2 + 20 + PREVIEW_SIZE / 2;
        RectF previewRect = { x - scalar / 2.f, y - scalar / 2.f, scalar, scalar };
        Color col = MakeRGBf(0.f, 0.f, progress, 1.f);
        canvas.DrawRectangle(previewRect, col);
    }

    void DrawPreview2(float progress, int yOffset, TweeningFunction func)
    {
        float xOffset = func(progress, 0.f, 300.f);
        float x = Game.width / 1.8f - GRAPH_PADDING * 3 - PREVIEW_SIZE + xOffset;
        float y = yOffset + GRAPH_PADDING * 2 + 20;
        RectF previewRect = { x, y, PREVIEW_SIZE, PREVIEW_SIZE };
        Color col = MakeRGBf(progress, 0.f, 0.f, 1.f);
        canvas.DrawRectangle(previewRect, col);
    }

    void DrawGraph(float progress, int yOffset, TweeningFunction func)
    {
        Rect graphRect = { GRAPH_PADDING, GRAPH_PADDING, GRAPH_SIZE, GRAPH_SIZE };
        graphRect.y += yOffset;

        canvas.DrawRectangle(graphRect, WHITE);
        Point leftBotCorner = { graphRect.x, graphRect.y + graphRect.height };
        canvas.DrawLine(graphRect.origin(), leftBotCorner, DARKGRAY);
        Point rightBotCorner = { graphRect.x + graphRect.width, graphRect.y + graphRect.height };
        canvas.DrawLine(rightBotCorner, leftBotCorner, DARKGRAY);

        for (int x = 0; x < graphRect.width; x++) {
            float perc = (float)x / (float)graphRect.width;
            int y = (int)func(1.f - perc, graphRect.y, graphRect.y + graphRect.height);
            canvas.DrawDot(x + graphRect.x, y, BLUE);
        }

        constexpr int SIZE = 4;
        float x = progress * graphRect.width + graphRect.x;
        float y = func(1.f - progress, graphRect.y, graphRect.y + graphRect.height);
        RectF marker = { x - SIZE / 2.f, y - SIZE / 2.f, SIZE, SIZE };

        canvas.DrawRectangle(marker, RED);
    }

    void DrawTweening(Image canvas, int yOffset, const pair<string, TweeningFunction>& item)
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
