#include <basalt.h>
#include <memory>

Image BakeRainbowImage(int width, int height);
void UpdateAndRenderGame(std::shared_ptr<Image>& canvas, float delta);