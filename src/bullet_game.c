#include "basalt.h"
#include "basalt_extra.h"

const uint WIDTH = 800;
const uint HEIGHT = 600;
const char* GAME_TITLE = "Bullethell game";

const uint MAX_FPS = 900;
const uint TPS = 25;

#define SKY_COLOR 0x000000FF

#include "bullet_entities.c"

typedef struct Bulletgame {
    Entity* player;
} Bulletgame;

pubfunc void InitializeGame()
{
    Entity* player = CreateEntity();
    Vec2 spawnPos = {WIDTH / 2, HEIGHT / 1.2f};
    InitPlayer(player, spawnPos);
}

pubfunc void DisposeGame()
{
    WARN("TODO: dispose game properly");
}

pubfunc void UpdateAndRenderGame(Texture canvas, float delta)
{
    ClearTexture(canvas, SKY_COLOR);
    UpdateAndRenderEntities(canvas, delta);
}
