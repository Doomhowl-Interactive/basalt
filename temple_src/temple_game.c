// compile raylib first: https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux
#include <assert.h>
#include <stdio.h>
#include "raylib.h"
#include <stdbool.h>

typedef Rectangle Rect;
typedef unsigned int uint;
typedef Vector2 Vec2;
typedef Vector3 Vec3;

typedef uint EntityID;
typedef uint EntityType;

static char* SPR_RGBA = "assets/spr_rgba.png";
static char* SPR_PLAYER_FOX = "assets/spr_player_fox.png";
static char* SPR_BLOCK = "assets/spr_block.png";
static char* TILE_BLOCK_SMALL = "assets/tile_block_small.png";

static Texture foxSprite   = { 0 };
static Texture blockSprite = { 0 };

#define COMPARE(X,Y) ((X & Y) == Y)

#define WIDTH 640
#define HEIGHT 480

#define TPS 25
#define MAX_ENTITIES 256

#define ENTITY_PLAYER           (1 << 0)
#define ENTITY_TILE             (1 << 1)
#define ENTITY_PHYSICS_BODY     (1 << 2)

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
    EntityID id;
    EntityType type;

    // player
    Vec2 pos;
    PlayerPose pose;

    // physics
    Vec2 vel;
    
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
        Entity* entity = &GameEntities[i];
        if (entity->type == 0)
        {
            entity->id = i;
            return entity; 
        }
    }

    assert(0);
    return NULL;
}

void InitPlayer(Entity* e, Vec2 pos)
{
    foxSprite = LoadTexture(SPR_PLAYER_FOX);
    e->pos = pos;
    e->type = ENTITY_PLAYER | ENTITY_PHYSICS_BODY;
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

Rect GetEntityBounds(Entity entity)
{
    Rect bounds = {
        entity.pos.x,
        entity.pos.y,
        entity.source.width,
        entity.source.height
    };
    return bounds;
}

Entity* GetTileCollision(Rect bounds, Rect* overlap)
{
    for (uint i = 0; i < MAX_ENTITIES; i++)
    {
        Entity* other = &GameEntities[i];

        // only check tiles
        if (!COMPARE(other->type,ENTITY_TILE)) continue;

        Rect otherBounds = GetEntityBounds(*other); 
        Rect colRect = GetCollisionRec(bounds, otherBounds);
        if (colRect.width > 0 && colRect.height > 0)
        {
            if (overlap) *overlap = colRect;
            return other;
        }
    }

    if (overlap)
        memset(overlap, 0, sizeof(Rect));

    return NULL;
}
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

    if (e->texture.width > 0)
    {
        DrawTextureRec(e->texture, e->source, e->pos, e->tint);
    }

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
    if (COMPARE(e->type,ENTITY_TILE))
    {

    }
    if (COMPARE(e->type,ENTITY_PHYSICS_BODY))
    {
        static float gravity = 9.8f;
        e->vel.y += delta * gravity * 10.f;

        float nextX = e->pos.x + e->vel.x * delta;
        float nextY = e->pos.y + e->vel.y * delta;

        int width = e->source.width;
        int height = e->source.height;

        Rect overlap = { 0 };

        Rect nextRect = {
            nextX, nextY,
            width, height,
        };
        bool isOverlapping = GetTileCollision(nextRect, &overlap);

        DrawRectangleLines(nextX, nextY, width, height, isOverlapping ? RED:BLACK);

        if (isOverlapping)
        {
            if (overlap.width > overlap.height)
            {
                nextY -= overlap.height;
                e->vel.y = 0.f;
            }
            else
            {
                nextX -= overlap.width;
                e->vel.y = 0.f;
            }
        }

        e->pos.x = nextX;
        e->pos.y = nextY;
    }
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Temple Mayhem");

    // TODO: Fix physics bugs at low FPS
    SetTargetFPS(60);

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
