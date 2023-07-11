#include <stdexcept>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

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
        return;
    }
    LoadFontEx("font_fff_forward", 16);
}

static void LogFontError(string text)
{
    // Avoid spamming the log
    if (LastYellTime + 2.0 < GetTimeElapsed()) {
        SDL_LogError(0, "%s", text.c_str());
        LastYellTime = GetTimeElapsed();
    }
}

static string LoadedFontsToString()
{
    std::string text = "Available fonts are: ";
    if (LoadedFonts.size() == 0) {
        bool first = true;
        for (const auto& entry : LoadedFonts) {
            if (!first) {
                text += ", ";
            }
            text += entry.first;
            first = false;
        }
        return text;
    }
    return text + "none";
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

BASALT void DrawText(Texture canvas, const char* text, int posX, int posY, Color color)
{
    DrawTextWithFont("default", canvas, text, posX, posY, color);
}

static void DrawTextLineWithFont(TTF_Font* font,
                                 Texture canvas,
                                 string& line,
                                 int posX,
                                 int posY,
                                 Color color)
{
    if (line.empty()) {
        return;
    }

    SDL_Color sdlColor = ConvertColor(color);
    SDL_Surface* surface = TTF_RenderText_Blended(font, line.c_str(), sdlColor);
    SDL_Rect destRect = { posX, posY, surface->w, surface->h };
    // TODO: Implement GPU: SDL_RenderCopy(canvas->renderer, texture, NULL, &rect);
    SDL_UpperBlit(surface, NULL, GetScreenOverlaySurface(), &destRect);
    SDL_FreeSurface(surface);
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

    // Split up function if text contains multiple lines
    stringstream stream(text);
    string line;
    int height = TTF_FontHeight(font);
    while (getline(stream, line)) {
        DrawTextLineWithFont(font, canvas, line, posX, posY, color);
        posY += height;
    }
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
