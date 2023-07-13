#include <unordered_map>
#include <spdlog/spdlog.h>
#include "sdl2_plat.hpp"

namespace basalt {

using namespace std;

static unordered_map<SDL_Keycode, bool> pressedKeys;
static unordered_map<SDL_Keycode, bool> pressedKeysFrame;

static Point mousePos;

static void PrintKeys()
{
    for (auto& key : pressedKeys) {
        spdlog::debug("Key {} is {}", SDL_GetKeyName(key.first), key.second ? "down" : "up");
    }
}

bool ProcessKeyboardInput(SDL_Event* e)
{
    if (e->type == SDL_KEYDOWN || e->type == SDL_KEYUP) {
        SDL_KeyboardEvent keyevent = e->key;
        SDL_Keycode key = keyevent.keysym.sym;
        if (key == SDLK_ESCAPE) {
            return false;
        }

        if (key == SDLK_DELETE && e->type == SDL_KEYDOWN) {
            PrintKeys();
        }

        bool isDown = e->type == SDL_KEYDOWN;
        pressedKeys[key] = isDown;
        pressedKeysFrame[key] = isDown;

        // SDL_Log("Key %s was %s", SDL_GetKeyName(key), keyevent.state == SDL_KEYDOWN ? "pressed" :
        // "released");
    }
    return true;
}

void ProcessMouseInput()
{
    // Poll mouse position
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
}

void ClearKeyboardInput()
{
    pressedKeysFrame.clear();
}

bool IsKeyDown(KeyCode code)
{
    return pressedKeys[code];
}

bool IsKeyUp(KeyCode code)
{
    return !pressedKeys[code];
}

Point GetMousePosition()
{
    return mousePos;
}

bool IsMouseDown()
{
    return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
}

bool IsMousePressed()
{
    return IsMouseDown() && !pressedKeysFrame[SDL_BUTTON_LEFT];
}

bool IsMouseUp()
{
    return !IsMouseDown();
}

bool IsKeyPressed(KeyCode code)
{
    return pressedKeysFrame[code];
}

bool IsKeyReleased(KeyCode code)
{
    return !pressedKeysFrame[code];
}
}  // namespace basalt