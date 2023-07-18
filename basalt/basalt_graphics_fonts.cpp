#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <spdlog/spdlog.h>

#include "basalt.h"
#include "basalt_console.hpp"
#include "basalt_assets.hpp"
#include "sdl2_basalt.hpp"

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

// TODO: move to utils
template <class T> inline void hash_combine(size_t& seed, const T& v)
{
    hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

class CachedText {
   public:
    CachedText(Font font, string text, Color color)
    {
        // bake text
        auto theFont = LoadedFonts.at(font.name);
        SDL_Color sdlColor = ConvertColor(color);
        // TODO: use the function from TTF_Font that wraps
        surface = TTF_RenderText_Blended(theFont, text.c_str(), sdlColor);
        lastUsed = SDL_GetTicks();
    }

    ~CachedText()
    {
        SDL_FreeSurface(surface);
    }

    bool isOld()
    {
        return SDL_GetTicks() - lastUsed > 20 * 1000;
    }

    SDL_Surface* get()
    {
        lastUsed = SDL_GetTicks();
        return surface;
    }

   private:
    SDL_Surface* surface;
    size_t lastUsed;
};

static map<size_t, shared_ptr<CachedText>> TextCache = {};

static size_t DrawSettingsHash(Font font, string text, Color color)
{
    size_t hash = 0;
    hash_combine(hash, font.name);
    hash_combine(hash, text);
    hash_combine(hash, color);
    return hash;
}

// TODO: refactor
#undef DrawText  // thank you Microsoft, very cool
void Texture::DrawText(string text, int posX, int posY, Color color, Font font)
{
    string line;
    auto stream = stringstream(text);
    while (getline(stream, line)) {
        if (line.empty()) {
            return;
        }

        size_t hash = DrawSettingsHash(font, line, color);
        try {
            weak_ptr<CachedText> text = TextCache.at(hash);
            SDL_Surface* surface = text.lock()->get();
            SDL_Rect destRect = { posX, posY, surface->w, surface->h };
            // TODO: Implement GPU: SDL_RenderCopy(canvas->renderer, texture, NULL, &rect);
            SDL_UpperBlit(surface, NULL, GetScreenOverlaySurface(), &destRect);
        } catch (out_of_range) {
            auto text = make_shared<CachedText>(font, line, color);

            SDL_Surface* surface = text->get();
            SDL_Rect destRect = { posX, posY, surface->w, surface->h };
            // TODO: Implement GPU: SDL_RenderCopy(canvas->renderer, texture, NULL, &rect);
            SDL_UpperBlit(surface, NULL, GetScreenOverlaySurface(), &destRect);

            TextCache.insert({ hash, text });

            // remove old
            if (TextCache.size() > 100) {
                for (auto it = TextCache.begin(); it != TextCache.end();) {
                    if (it->second->isOld()) {
                        it = TextCache.erase(it);
                    } else {
                        ++it;
                    }
                }
            }
            if (TextCache.size() > 100) {
                TextCache.erase(TextCache.begin());
            }
        }
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
