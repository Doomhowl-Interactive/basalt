#include <unordered_map>
#include <spdlog/spdlog.h>

#include "sdl2_basalt.hpp"
#include "basalt_math.hpp"
#include "basalt_input.hpp"

using namespace std;

static unordered_map<SDL_Keycode, bool> pressedKeys;
static unordered_map<SDL_Keycode, bool> pressedKeysFrame;

enum MouseState { MOUSE_UP, MOUSE_PRESSED, MOUSE_DOWN };

static Vec2 mousePos;
static MouseState mouseState = MOUSE_UP;

static void PrintKeys()
{
    for (auto& key : pressedKeys) {
        spdlog::debug("Key {} is {}", SDL_GetKeyName(key.first), key.second ? "down" : "up");
    }
}

bool ProcessKeyboardInput(SDL_Event* e)
{
    if (e->type == SDL_EVENT_KEY_DOWN || e->type == SDL_EVENT_KEY_DOWN) {
        SDL_KeyboardEvent keyevent = e->key;
        SDL_Keycode key = keyevent.keysym.sym;
        if (key == SDLK_ESCAPE) {
            return false;
        }

        if (key == SDLK_DELETE && e->type == SDL_EVENT_KEY_DOWN) {
            PrintKeys();
        }

        bool isDown = e->type == SDL_EVENT_KEY_DOWN;
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

    // state machine for mouse
    if (mouseState == MOUSE_UP) {
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            mouseState = MOUSE_PRESSED;
        }
    } else if (mouseState == MOUSE_PRESSED) {
        mouseState = MOUSE_DOWN;
    } else if (!(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))) {
        mouseState = MOUSE_UP;
    }
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
    return mouseState == MOUSE_DOWN;
}

bool IsMousePressed()
{
    return mouseState == MOUSE_PRESSED;
}

bool IsMouseUp()
{
    return mouseState == MOUSE_UP;
}

bool IsKeyPressed(KeyCode code)
{
    return pressedKeysFrame[code];
}

bool IsKeyReleased(KeyCode code)
{
    return !pressedKeysFrame[code];
}
