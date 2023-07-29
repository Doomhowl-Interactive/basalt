#include <basalt.h>
#include <memory>

void UpdateAndRenderGame(std::shared_ptr<Image>& canvas, float delta);

Color InterpolateHue(float t);