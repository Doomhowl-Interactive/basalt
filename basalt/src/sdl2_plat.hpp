#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_surface.h>
#include <SDL_log.h>

#include "basalt.h"

#define PLAT_SDL2

// HACK: We should probably pass some kind of RenderContext (a union maybe) 
// to each drawing function instead of using an overlaying surface!
PLAT_SDL2 SDL_Surface* GetScreenOverlaySurface();
PLAT_SDL2 SDL_Color ConvertColor(Color color);

PLAT_SDL2 bool ProcessKeyboardInput(SDL_Event* e);
PLAT_SDL2 void ProcessMouseInput();
PLAT_SDL2 void ClearKeyboardInput();
