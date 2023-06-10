#include "basalt.h"
#include <string>
#include <SDL_log.h>
#include <filesystem>
#include <SDL_ttf.h>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

// TODO: DRY
static std::vector<std::string> AssetFolders = {
    ".", "assets", "../assets", "../../assets", "../../../assets",
};

static std::unordered_map<std::string, TTF_Font*> LoadedFonts;

// TODO: Put in basalt_internal.hpp
func std::string SearchAsset(std::string assetName)
{
    fs::path assetFileName = fs::path(assetName).replace_filename(".ttf");
    for (auto& folder : AssetFolders) {
        auto combined = fs::path(folder) / assetFileName;
        if (fs::exists(combined)) {
            return combined;
        }
    }
    throw new std::runtime_error("Asset not found: " + assetName);
}

func void InitFonts()
{
    if (TTF_Init() < -1) {
        SDL_LogError(0, "Failed to init TTF font rendering!: %s\n", TTF_GetError());
        exit(2);
    }
    LoadFont("font_fff_forward.ttf", 16);
}

BASALT void LoadFont(const char* fontName, uint size = 16)
{
    auto assetPath = SearchAsset(fontName);
    auto font = TTF_OpenFont(assetPath.c_str(), 16);
    if (font == nullptr) {
        // fallback to another font
        if (LoadedFonts.empty()) {
            throw new std::runtime_error("Could not load font!");
        }

        LoadedFonts.insert({ fontName, LoadedFonts.at("default") });
        SDL_LogWarn(0, "Failed to load font, using default: %s\n", TTF_GetError());
    }
}

BASALT void DrawText(Texture canvas, const char* text, int posX, int posY, Color color)
{
    DrawTextWithFont("default", canvas, text, posX, posY, color);
}

BASALT void DrawTextWithFont(TTF_Font* font, Texture canvas, const char* text, int posX, int posY, Color color)
{
    //SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    //SDL_Texture* texture = SDL_CreateTextureFromSurface(canvas->renderer, surface);
    //SDL_Rect rect = { posX, posY, surface->w, surface->h };
    //SDL_RenderCopy(canvas->renderer, texture, NULL, &rect);
    //SDL_FreeSurface(surface);
    //SDL_DestroyTexture(texture);
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
