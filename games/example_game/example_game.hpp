#include <basalt.h>
#include <memory>

Color InterpolateHue(float t);
Image BakeRainbowImage(int width, int height);
void UpdateAndRenderGame(std::shared_ptr<Image>& canvas, float delta);
void DrawTrippyPattern(Image canvas, int x, int y, int count);
