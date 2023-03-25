#include "basalt.h"
#include "basalt_extra.h"

const uint WIDTH = 640;
const uint HEIGHT = 480;

pubfunc void InitializeGame()
{
}

pubfunc void DisposeGame()
{
}

pubfunc void UpdateAndRenderGame(Texture canvas, float delta)
{
    DrawWeirdTestGradient(canvas);
}
