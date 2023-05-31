#include <string>
#include <SDL2/SDL.h>

#include "basalt.h"

#define PLAT_SDL2
PLAT_SDL2 bool ProcessKeyboardInput(SDL_Event& e);
PLAT_SDL2 void ProcessMouseInput();
PLAT_SDL2 void ClearKeyboardInput();