#include <string>
#include <SDL_log.h>
#include <filesystem>
#include <SDL_ttf.h>
#include <unordered_map>
#include <vector>

#include "basalt.h"
#include "basalt_internal.hpp"

static std::unordered_map<std::string, TTF_Font*> LoadedFonts;

INTERNAL void InitEngineFonts()
{
    if (TTF_Init() < -1) {
        SDL_LogError(0, "Failed to init TTF font rendering!: %s\n", TTF_GetError());
        exit(2);
    }
    LoadFontEx("font_fff_forward.ttf", 16);
}

BASALT inline void LoadFont(const char* fontName)
{
    LoadFontEx(fontName, 16);
}

BASALT void LoadFontEx(const char* fontName, uint size)
{
    std::string assetPath = "";
    if (SearchAsset(fontName, &assetPath)) {
        auto font = TTF_OpenFont(assetPath.c_str(), size);
        if (font == nullptr) {
            // fallback to another font
            if (LoadedFonts.empty()) {
                SDL_LogError(0, "Failed to load font %s\n", TTF_GetError());
                return;
            }
        }
    }

    LoadedFonts.insert({ fontName, LoadedFonts.at("default") });
    SDL_LogWarn(0, "Failed to load font, using default: %s\n", TTF_GetError());
}

BASALT void DrawText(Texture canvas, const char* text, int posX, int posY, Color color)
{
    DrawTextWithFont("default", canvas, text, posX, posY, color);
}

BASALT void DrawTextWithFont(const char* fontName, Texture canvas, const char* text, int posX, int posY, Color color)
{
    // SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    // SDL_Texture* texture = SDL_CreateTextureFromSurface(canvas->renderer, surface);
    // SDL_Rect rect = { posX, posY, surface->w, surface->h };
    // SDL_RenderCopy(canvas->renderer, texture, NULL, &rect);
    // SDL_FreeSurface(surface);
    // SDL_DestroyTexture(texture);
}

BASALT void DisposeFonts()
{
    for (auto& font : LoadedFonts) {
        TTF_CloseFont(font.second);
    }
    LoadedFonts.clear();
    TTF_Quit();
    SDL_LogDebug(0, "Disposed fonts context");
}
