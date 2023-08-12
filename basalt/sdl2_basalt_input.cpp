#include <unordered_map>
#include <spdlog/spdlog.h>

#include "sdl2_basalt.hpp"
#include "basalt_math.hpp"
#include "basalt_input.hpp"

using namespace std;

static unordered_map<SDL_Keycode, bool> pressedKeys;
static unordered_map<SDL_Keycode, bool> pressedKeysFrame;

enum MouseState { MOUSE_UP, MOUSE_PRESSED, MOUSE_DOWN };

static Point mousePos;
static MouseState mouseState = MOUSE_UP;

static void PrintKeys()
{
    for (auto& key : pressedKeys) {
        spdlog::debug("Key {} is {}", SDL_GetKeyName(key.first), key.second ? "down" : "up");
    }
}

bool ProcessKeyboardInput(SDL_Event* e)
{
    if (e->type == SDL_KEYDOWN || e->type == SDL_KEYUP) {
        SDL_KeyboardEvent keyEvent = e->key;
        SDL_Keycode key = keyEvent.keysym.sym;
        if (key == SDLK_ESCAPE) {
            return false;
        }

        if (key == SDLK_DELETE && e->type == SDL_KEYDOWN) {
            PrintKeys();
        }

        if (e->type == SDL_KEYDOWN) {
            ProcessKeyDown((KeyCode)key);
        }else {
            ProcessKeyUp((KeyCode)key);
        }
        
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

void ProcessKeyDown(KeyCode code){
    pressedKeys[code] = true;
    pressedKeysFrame[code] = true;
}

void ProcessKeyUp(KeyCode code) {
    pressedKeys[code] = false;
    pressedKeysFrame[code] = false;
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
