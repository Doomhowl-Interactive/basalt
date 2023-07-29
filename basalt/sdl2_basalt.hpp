#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_surface.h>
#include <SDL_log.h>

#include "basalt_colors.hpp"
#include "basalt_config.hpp"

SDL_Color ConvertColor(Color color);

bool ProcessKeyboardInput(SDL_Event* e);
void ProcessMouseInput();
void ClearKeyboardInput();

bool ParseLaunchArguments(EngineConfig* config, int argc, char** argv);