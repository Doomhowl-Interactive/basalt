#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <spdlog/spdlog.h>

#include "basalt_macros.hpp"
#include "basalt_console.hpp"
#include "basalt_assets.hpp"
#include "basalt_utils.hpp"
#include "sdl2_basalt.hpp"

using namespace std;

static map<string, TTF_Font*> LoadedFonts;

struct CachedText;
static map<size_t, shared_ptr<CachedText>> TextCache = {};

class CachedText {
   public:
    CachedText(string text, Font font, FontStyle style)
    {
        // bake text
        auto theFont = LoadedFonts.at(font.name);
        SDL_Color sdlColor = ConvertColor(style.color);
        // TODO: Implement wrapping
        surface = TTF_RenderUTF8_Blended_Wrapped(theFont, text.c_str(), sdlColor, style.maxWidth);
        if (!surface) {
            spdlog::error("Failed to render text {}: {}", text, TTF_GetError());
            return;
        }
        assert(surface->w > 0 && surface->h > 0);
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

    Size getSize()
    {
        return { (uint)surface->w, (uint)surface->h };
    }

   private:
    SDL_Surface* surface;
    size_t lastUsed;
};

static shared_ptr<CachedText> GetOrCacheText(string text,
                                             Font font,
                                             FontStyle style,
                                             Size* outSize = nullptr)
{
    size_t hash = style.Hash(text);

    // resize the font to be the requested size
    TTF_Font* theFont = LoadedFonts.at(font.name);
    TTF_SetFontSize(theFont, style.size);

    shared_ptr<CachedText> cached;
    try {
        cached = TextCache.at(hash);
    } catch (out_of_range) {
        cached = make_shared<CachedText>(text, font, style);

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
        // remove any if previous step wasn't enough
        if (TextCache.size() > 100) {
            TextCache.erase(TextCache.begin());
        }

        TextCache.insert({ hash, cached });
    }
    if (outSize) {
        *outSize = cached->getSize();
    }
    return cached;
}

Font Font::Default()
{
    auto it = LoadedFonts.begin();
    if (it == LoadedFonts.end()) {
        spdlog::error("There are no fonts loaded");
    }
    return { it->first };
}

Size Font::MeasureString(string text, int fontSize)
{
    TTF_Font* theFont = LoadedFonts.at(name);
    TTF_SetFontSize(theFont, fontSize);

    int w, h;
    TTF_SizeUTF8(theFont, text.c_str(), &w, &h);
    return { (uint)w, (uint)h };
}

Font LoadFont(string fontName)
{
    if (TTF_WasInit() == 0) {
        TTF_Init();
    }

    auto assetPath = SearchAsset(fontName, "ttf");
    if (assetPath) {
        auto font = TTF_OpenFont(assetPath.value().c_str(), 16);
        if (font == nullptr) {
            spdlog::error("Failed to load font {}: {}", fontName, TTF_GetError());
            goto DEFAULT;
        }

        // store loaded font
        LoadedFonts.insert({ fontName, font });
        spdlog::debug("Loaded font {}.ttf", fontName);

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

void Texture::DrawBasaltText(string text,
                             int posX,
                             int posY,
                             Color color,
                             Font font,
                             int size,
                             int maxWidth,
                             bool centered)
{
    FontStyle style;
    style.centered = centered;
    style.color = color;
    style.size = size;
    style.maxWidth = maxWidth;
    style.centered = centered;
    DrawBasaltText(text, posX, posY, font, style);
}

void Texture::DrawBasaltText(std::string text, int posX, int posY, Font font, FontStyle style)
{
    size_t hash = style.Hash(text);
    Size size;
    auto surface = GetOrCacheText(text, font, style, &size);

    SDL_Rect destRect;
    destRect.x = posX;
    destRect.y = posY;
    destRect.w = size.width;
    destRect.h = size.height;

    SDL_UpperBlit(surface->get(), NULL, GetScreenOverlaySurface(), &destRect);
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

// ==== Font styles ====
FontStyle::FontStyle(const FontStyle& st)
{
    this->font = st.font;
    this->color = st.color;
    this->size = st.size;
    this->maxWidth = st.maxWidth;
    this->centered = st.centered;
}

size_t FontStyle::Hash(string ofText) const
{
    size_t hash = 0;
    CombineHash(hash, font.name);
    CombineHash(hash, ofText);
    CombineHash(hash, color);
    CombineHash(hash, maxWidth);
    return hash;
}

FontStyle FontStyle::center() const
{
    auto c = FontStyle(*this);
    c.centered = true;
    return c;
}

FontStyle FontStyle::wColor(Color color) const
{
    auto c = FontStyle(*this);
    c.color = color;
    return c;
}

FontStyle FontStyle::wFont(Font font) const
{
    auto c = FontStyle(*this);
    c.font = font;
    return c;
}

FontStyle FontStyle::wSize(int size) const
{
    auto c = FontStyle(*this);
    c.size = (uint)std::max(1, size);
    return c;
}

FontStyle FontStyle::wMaxWidth(int maxWidth) const
{
    auto c = FontStyle(*this);
    c.maxWidth = maxWidth;
    return c;
}
