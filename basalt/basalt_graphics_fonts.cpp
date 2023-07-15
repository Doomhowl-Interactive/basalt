#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <spdlog/spdlog.h>

#include "basalt.h"
#include "basalt_console.hpp"
#include "basalt_assets.hpp"
#include "sdl2_plat.hpp"

using namespace std;

static map<string, TTF_Font*> LoadedFonts;

Font Font::Default()
{
    auto it = LoadedFonts.begin();
    if (it == LoadedFonts.end()) {
        spdlog::error("There are no fonts loaded");
    }

    return { it->first };
}

Font LoadFont(string fontName, unsigned int baseSize)
{
    if (TTF_WasInit() == 0) {
        TTF_Init();
    }

    auto assetPath = SearchAsset(fontName, "ttf");
    if (assetPath) {
        auto font = TTF_OpenFont(assetPath.value().c_str(), baseSize);
        if (font == nullptr) {
            spdlog::error("Failed to load font {}: {}", fontName, TTF_GetError());
            goto DEFAULT;
        }

        // store loaded font
        LoadedFonts.insert({ fontName, font });
        spdlog::debug("Loaded font {}(.ttf)", fontName);

        // if this is the first font loaded, also set it as the default font
        if (LoadedFonts.size() == 1) {
            LoadedFonts.insert({ "default", font });
        }

        return { fontName };
    }

DEFAULT:
    TTF_Font* defaultFont;
    try {
        defaultFont = LoadedFonts.at("default");
    } catch (out_of_range& e) {
        spdlog::error("There is no default font: ({})", TTF_GetError());
    }

    LoadedFonts.insert({ fontName, defaultFont });
    spdlog::warn("Failed to load font, using default: {}", TTF_GetError());

    return Font::Default();
}

#undef DrawText  // thank you Microsoft, very cool
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

    if (TTF_WasInit() > 0) {
        TTF_Quit();
    }

    spdlog::info("Disposed fonts context");
}
