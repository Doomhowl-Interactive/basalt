#include "basalt.h"

float DistanceToEdge(float min, float max, float y)
{
    Point mouse = GetMousePosition();
    float dist = mouse.y - y;
    return Lerp(min, max, dist);
}

float ScrollOnWindowEdges(int height = 100)
{
    float scroll = 0.f;
    int mouseY = GetMousePosition().y;
    if (mouseY < height) {
        scroll = DistanceToEdge(0.f, 1.f, height);
    } else if (mouseY > Game.height - height) {
        scroll = DistanceToEdge(0.f, 1.f, Game.height - height);
    }
    return scroll;
}
