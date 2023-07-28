#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_surface.h>
#include <SDL_log.h>

#include "basalt_colors.hpp"
#include "basalt_config.hpp"

// HACK: We should probably pass some kind of RenderContext (a union maybe)
// to each drawing function instead of using an overlaying surface!
SDL_Surface* GetScreenSurface();
SDL_Color ConvertColor(Color color);

bool ProcessKeyboardInput(SDL_Event* e);
void ProcessMouseInput();
void ClearKeyboardInput();

bool ParseLaunchArguments(EngineConfig* config, int argc, char** argv);