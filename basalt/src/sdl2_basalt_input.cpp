#include <unordered_map>
#include "sdl2_plat.hpp"

static std::unordered_map<SDL_Keycode, bool> pressedKeys;
static std::unordered_map<SDL_Keycode, bool> pressedKeysFrame;

static Point mousePos;

static void PrintKeys()
{
    for (auto& key : pressedKeys) {
        SDL_Log("Key %s is %s", SDL_GetKeyName(key.first), key.second ? "down" : "up");
    }
}

PLAT_SDL2 bool ProcessKeyboardInput(SDL_Event* e)
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

PLAT_SDL2 void ProcessMouseInput()
{
    // Poll mouse position
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
}

PLAT_SDL2 void ClearKeyboardInput()
{
    pressedKeysFrame.clear();
}

extern "C" {

BASALT bool IsKeyDown(KeyCode code)
{
    return pressedKeys[code];
}

BASALT bool IsKeyUp(KeyCode code)
{
    return !pressedKeys[code];
}

BASALT Point GetMousePosition()
{
    return mousePos;
}

BASALT bool IsMouseDown()
{
    return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
}

BASALT bool IsMousePressed()
{
    return IsMouseDown() && !pressedKeysFrame[SDL_BUTTON_LEFT];
}

BASALT bool IsMouseUp()
{
    return !IsMouseDown();
}

BASALT bool IsKeyPressed(KeyCode code)
{
    return pressedKeysFrame[code];
}

BASALT bool IsKeyReleased(KeyCode code)
{
    return !pressedKeysFrame[code];
}
}