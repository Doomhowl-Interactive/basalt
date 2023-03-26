// compile raylib first: https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux
#include <assert.h>
#include <stdio.h>
#include "raylib.h"

typedef Rectangle Rect;
typedef unsigned int uint;
typedef Vector2 Vec2;
typedef Vector3 Vec3;

typedef uint EntityType;

static char* SPR_RGBA = "assets/spr_rgba.png";
static char* SPR_PLAYER_FOX = "assets/spr_player_fox.png";
static char* SPR_BLOCK = "assets/spr_block.png";
static char* TILE_BLOCK_SMALL = "assets/tile_block_small.png";

static Texture foxSprite   = { 0 };
static Texture blockSprite = { 0 };

#define WIDTH 640
#define HEIGHT 480

#define TPS 25
#define MAX_ENTITIES 256

#define ENTITY_PLAYER       (1 << 0)
#define ENTITY_TILE         (1 << 1)

#include "temple_assets.c"

static uint GameTick = 0;

typedef enum PlayerPose {
    POSE_IDLE,
    POSE_WALK,
    POSE_SLIDE,
    POSE_JUMP,
    POSE_FALL,
    POSE_DUCK,
} PlayerPose;

// entity mega struct
typedef struct Entity {
    EntityType type;

    // player
    Vec2 pos;
    PlayerPose pose;

    // sprite
    Color tint;
    Rect source;
    Texture texture;

} Entity;

static Entity GameEntities[MAX_ENTITIES];

void ClearEntities()
{
    memset(GameEntities, 0, sizeof(Entity)*MAX_ENTITIES);
    TraceLog(LOG_INFO, "Cleared all entities!");
}

Entity* CreateEntity()
{
    // get next available entity
    for (uint i = 0; i < MAX_ENTITIES; i++)
    {
        if (GameEntities[i].type == 0)
        {
            return &GameEntities[i];
        }
    }

    assert(0);
    return NULL;
}

void InitPlayer(Entity* e, Vec2 pos)
{
    foxSprite = LoadTexture(SPR_PLAYER_FOX);
    e->pos = pos;
    e->type = ENTITY_PLAYER;
    e->tint = WHITE;
}

void InitBlock(Entity* e, Vec2 tilePos)
{
    if (blockSprite.width == 0)
        blockSprite = LoadTexture(SPR_BLOCK);

    e->pos = (Vec2) {
        tilePos.x * blockSprite.width,
        tilePos.y * blockSprite.height
    };
    e->source = (Rect) {
        0, 0,
        blockSprite.width,
        blockSprite.height
    };
    e->texture = blockSprite;
    e->type = ENTITY_TILE;
    e->tint = WHITE;
}

void UpdateTextureFromSheet(Entity* entity, TextureSheet sheet) {
    assert(sheet.texture);
    entity->texture = *sheet.texture;

    uint frame = GameTick % sheet.count;
    entity->source = (Rect) {
        sheet.origin.x + frame * sheet.cellSize.x,
        sheet.origin.y,
        sheet.cellSize.x,
        sheet.cellSize.y,
    };
}

#define COMPARE(X,Y) ((X & Y) == Y)
void UpdateAndRenderEntity(Entity* e, float delta)
{
    static float frameInterval = 0.2f;
    static float timer = 0.f;
    static int frameID = 0;

    if (timer > frameInterval)
    {
        timer = 0.f;
        frameID++;
    }
    timer += delta;

    if (COMPARE(e->type,ENTITY_PLAYER))
    {
        TextureSheet sheet = { 0 };
        switch (e->pose)
        {
            case POSE_IDLE:
                sheet = ANIM_FOX_IDLE;
                break;
            case POSE_WALK:
                sheet = ANIM_FOX_WALK;
                break;
            case POSE_SLIDE:
                sheet = ANIM_FOX_STOP;
                break;
            case POSE_JUMP:
                sheet = ANIM_FOX_JUMP;
                break;
            case POSE_FALL:
                sheet = ANIM_FOX_LAND;
                break;
            case POSE_DUCK:
                sheet = ANIM_FOX_IDLE;
                // TODO: not implemented
                break;
            default:
                assert(0);
        }
        UpdateTextureFromSheet(e, sheet);
    }
    else if (COMPARE(e->type,ENTITY_TILE))
    {

    }
    else
    {
        assert(0);
    }

    if (e->texture.width > 0)
    {
        DrawTextureRec(e->texture, e->source, e->pos, e->tint);
    }
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Temple Mayhem");

    SetTargetFPS(999);               // Set our game to run at 60 frames-per-second

    ClearEntities();

    Entity* player = CreateEntity();
    InitPlayer(player, (Vec2){200, 50});

    int offsetY = HEIGHT / 64;
    for (uint y = 0; y < 3; y++)
    {
        for (uint x = 0; x < 10; x++)
        {
            Entity* block = CreateEntity();
            InitBlock(block, (Vec2) {x,offsetY-y});
        }
    }

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

            ClearBackground(SKYBLUE);

            float delta = GetFrameTime();

            uint count = 0;
            for (uint i = 0; i < MAX_ENTITIES; i++)
            {
                Entity* e = &GameEntities[i];
                if (e->type > 0)
                {
                    UpdateAndRenderEntity(e, delta);
                    count++;
                }
            }

            DrawFPS(10,10);
            DrawText(TextFormat("Entity count: %d", count), 10, 50, 12, YELLOW);

        EndDrawing();

        static float gameTimer = 0.f;
        gameTimer += delta;
        if (gameTimer > 1.f/TPS)
        {
            gameTimer = 0.0f;
            GameTick++;
        }

    }

    CloseWindow();

    return 0;
}
