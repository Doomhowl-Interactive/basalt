#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL_ttf.h>

#include "basalt.h"
#include "basalt_internal.hpp"
#include "sdl2_plat.hpp"

using namespace std;

static unordered_map<string, TTF_Font*> LoadedFonts;
static double LastYellTime = 0.0f;

INTERNAL void InitEngineFonts()
{
    if (TTF_Init() < -1) {
        SDL_LogError(0, "Failed to init TTF font rendering!: %s\n", TTF_GetError());
        exit(2);
    }
    LoadFontEx("font_fff_forward", 16);
}

func void LogFontError(string text)
{
    // Avoid spamming the log
    if (LastYellTime + 2.0 < GetTimeElapsed()) {
        SDL_LogError(0, "%s", text.c_str());
        LastYellTime = GetTimeElapsed();
    }
}

func string LoadedFontsToString()
{
    std::string text = "Available fonts are: ";
    if (LoadedFonts.size() == 0) {
        text += "none";
    } else {
        bool first = true;
        for (const auto& entry : LoadedFonts) {
            if (!first) {
                text += ", ";
            }
            text += entry.first;
            first = false;
        }
    }
    return text;
}

BASALT inline void LoadFont(const char* fontName)
{
    LoadFontEx(fontName, 16);
}

BASALT void LoadFontEx(const char* fontName, uint size)
{
    string assetPath = "";
    if (SearchAsset(fontName, &assetPath)) {
        auto font = TTF_OpenFont(assetPath.c_str(), size);
        if (font != nullptr) {
            // store loaded font
            LoadedFonts.insert({ fontName, font });
            SDL_LogDebug(0, "Loaded font %s(.ttf)", fontName);

            // if this is the first font loaded, also set it as the default font
            if (LoadedFonts.size() == 1) {
                LoadedFonts.insert({ "default", font });
            }

            return;
        }
    }

    TTF_Font* defaultFont;
    try {
        defaultFont = LoadedFonts.at("default");
    } catch (out_of_range& e) {
        SDL_LogError(0, "Failed to load font because: %s", TTF_GetError());
        return;
    }

    LoadedFonts.insert({ fontName, defaultFont });
    SDL_LogWarn(0, "Failed to load font, using default: %s", TTF_GetError());
}

BASALT void DrawText(Texture canvas, const char* text, int posX, int posY, Color color)
{
    DrawTextWithFont("default", canvas, text, posX, posY, color);
}

BASALT void DrawTextWithFont(const char* fontName,
                             Texture canvas,
                             const char* text,
                             int posX,
                             int posY,
                             Color color)
{
    TTF_Font* font;
    try {
        font = LoadedFonts.at(fontName);
    } catch (out_of_range& e) {
        LogFontError("Failed to draw text, font not found: " + string(fontName));
        LogFontError(LoadedFontsToString());
        return;
    }

    SDL_Color sdlColor = ConvertColor(color);
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, sdlColor);
    SDL_Rect destRect = { posX, posY, surface->w, surface->h };
    // TODO: Implement GPU: SDL_RenderCopy(canvas->renderer, texture, NULL, &rect);
    SDL_UpperBlit(surface, NULL, GetScreenOverlaySurface(), &destRect);
    SDL_FreeSurface(surface);
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
