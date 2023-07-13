#include <vector>
#include <string>
#include <sstream>
#include <map>

#include "basalt.h"
#include "basalt_internal.hpp"
#include "sdl2_plat.hpp"
#include "basalt_console.hpp"

namespace basalt {

using namespace std;

static std::map<string, TTF_Font*> LoadedFonts;

Font Font::Default()
{
    auto it = LoadedFonts.begin();
    if (it == LoadedFonts.end()) {
        ERR("There are no fonts loaded");
    }

    return { it->first };
}

Font LoadFont(string fontName, unsigned int baseSize)
{
    string assetPath = "";
    if (SearchAsset(fontName, &assetPath)) {
        auto font = TTF_OpenFont(assetPath.c_str(), baseSize);
        if (font == nullptr) {
            SDL_LogError(0, "Failed to load font %s: %s", fontName, TTF_GetError());
            goto DEFAULT;
        }

        // store loaded font
        LoadedFonts.insert({ fontName, font });
        SDL_LogDebug(0, "Loaded font %s(.ttf)", fontName);

        // if this is the first font loaded, also set it as the default font
        if (LoadedFonts.size() == 1) {
            LoadedFonts.insert({ "default", font });
        }

        return;
    }

DEFAULT:
    TTF_Font* defaultFont;
    try {
        defaultFont = LoadedFonts.at("default");
    } catch (out_of_range& e) {
        SDL_LogError(0, "There is no default font: (%s)", TTF_GetError());
        return;
    }

    LoadedFonts.insert({ fontName, defaultFont });
    SDL_LogWarn(0, "Failed to load font, using default: %s", TTF_GetError());
}

void Texture::DrawText(string text, int posX, int posY, Color color, Font font)
{
    string line;
    auto stream = stringstream(text);
    while (getline(stream, line)) {
        if (line.empty()) {
            return;
        }

        auto theFont = LoadedFonts.at(font.name);

        SDL_Color sdlColor = ConvertColor(color);
        SDL_Surface* surface = TTF_RenderText_Blended(theFont, line.c_str(), sdlColor);
        SDL_Rect destRect = { posX, posY, surface->w, surface->h };
        // TODO: Implement GPU: SDL_RenderCopy(canvas->renderer, texture, NULL, &rect);
        SDL_UpperBlit(surface, NULL, GetScreenOverlaySurface(), &destRect);
        SDL_FreeSurface(surface);
        posY += TTF_FontHeight(theFont);
    }
}

void DisposeFonts()
{
    for (auto& font : LoadedFonts) {
        TTF_CloseFont(font.second);
    }
    LoadedFonts.clear();
    TTF_Quit();
    SDL_LogDebug(0, "Disposed fonts context");
}

}  // namespace basalt