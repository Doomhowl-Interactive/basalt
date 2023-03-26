// compile raylib first: https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux
#include "raylib.h"

typedef unsigned int uint;
typedef Vector2 Vec2;
typedef Vector3 Vec3;

static char* SPR_RGBA = "assets/spr_rgba.png";
static char* SPR_PLAYER_FOX = "assets/spr_player_fox.png";
static char* SPR_BLOCK = "assets/spr_block.png";
static char* TILE_BLOCK_SMALL = "assets/tile_block_small.png";

static Texture foxSprite;

#include "temple_assets.c"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 640;
    const int screenHeight = 480;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    foxSprite = LoadTexture(SPR_PLAYER_FOX);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(SKYBLUE);

            DrawTexture(foxSprite, 10, 10, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
