#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <spdlog/spdlog.h>

#include "basalt_exceptions.hpp"
#include "basalt_macros.hpp"
#include "basalt_console.hpp"
#include "basalt_assets.hpp"
#include "basalt_utils.hpp"
#include "sdl2_basalt.hpp"

using namespace std;

static map<string, TTF_Font*> LoadedFonts = {};

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

    Point getSize()
    {
        return { surface->w, surface->h };
    }

   private:
    SDL_Surface* surface;
    size_t lastUsed;
};

static shared_ptr<CachedText> GetOrCacheText(string text,
                                             Font font,
                                             FontStyle style,
                                             Point* outSize = nullptr)
{
    size_t hash = style.Hash(text);

    // resize the font to be the requested size
    TTF_Font* theFont = LoadedFonts.at(font.name);
    TTF_SetFontSize(theFont, style.size);

    shared_ptr<CachedText> cached;
    try {
        cached = TextCache.at(hash);
    } catch (out_of_range) {
        constexpr int MAX_CACHE = 30;
        cached = make_shared<CachedText>(text, font, style);

        // remove old
        if (TextCache.size() > MAX_CACHE) {
            for (auto it = TextCache.begin(); it != TextCache.end();) {
                if (it->second->isOld()) {
                    it = TextCache.erase(it);
                } else {
                    ++it;
                }
            }
        }
        // remove any if previous step wasn't enough
        if (TextCache.size() > MAX_CACHE) {
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
    if (LoadedFonts.empty()) {
        spdlog::error("There are no fonts loaded");
        auto defaultFont = LoadFont("FFFFORWA.TTF");
        return defaultFont;
    }

    auto it = LoadedFonts.begin();
    return { it->first };
}

Point Font::MeasureString(string text, int fontSize)
{
    TTF_Font* theFont = LoadedFonts.at(name);
    TTF_SetFontSize(theFont, fontSize);

    int w, h;
    TTF_SizeUTF8(theFont, text.c_str(), &w, &h);
    return { w, h };
}

Font LoadFont(string fontName)
{
    if (TTF_WasInit() == 0) {
        TTF_Init();
    }

    // check if font is already loaded
    try {
        if (LoadedFonts.empty()) {
            throw out_of_range("No fonts loaded");
        }
        LoadedFonts.at(fontName);
        return { fontName };
    } catch (out_of_range& e) {
        auto assetPath = SearchAsset(fontName, "ttf");
        if (assetPath) {
            auto font = TTF_OpenFont(assetPath.value().c_str(), 16);
            if (font == nullptr) {
                spdlog::error("Failed to load font {}: {}", fontName, TTF_GetError());
                throw BasaltException("Failed to load font");
            }

            // store loaded font
            LoadedFonts.insert({ fontName, font });
            spdlog::debug("Loaded font {}.ttf", fontName);

            return { fontName };
        } else {
            spdlog::error("Failed to find font {}", fontName);
            throw BasaltException("Failed to load font");
        }
    }
}

void Image::DrawBasaltText(string text,
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
    DrawBasaltText(text, posX, posY, font, style);
}

void Image::DrawBasaltText(std::string text, int posX, int posY, FontStyle style)
{
    DrawBasaltText(text, posX, posY, Font::Default(), style);
}

void Image::DrawBasaltText(std::string text, int posX, int posY, Font font, FontStyle style)
{
    if (style.size > 1000) {
        spdlog::warn("Font size is too big: {}", style.size);
        return;
    }
    
    if (text.empty()) {
        return;
    }

    size_t hash = style.Hash(text);
    Point size = {};
    auto textSurface = GetOrCacheText(text, font, style, &size);

    SDL_Rect destRect = {
        posX,
        posY,
        size.x,
        size.y,
    };

    if (style.centered) {
        destRect.x -= size.x / 2;
        destRect.y -= size.y / 2;
    }

    SDL_BlitSurface(textSurface->get(), nullptr, this->surface->get(), &destRect);
}

void Image::DrawBasaltTextShadow(string text,
                                 int posX,
                                 int posY,
                                 Font font,
                                 FontStyle foreStyle,
                                 FontStyle backStyle,
                                 int spacingX,
                                 int spacingY)
{
    DrawBasaltText(text, posX - spacingX, posY - spacingY, font, backStyle);
    DrawBasaltText(text, posX, posY, font, foreStyle);
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
FontStyle::FontStyle(Color color, uint size, bool centered, uint maxWidth)
{
    this->color = color;
    this->size = size;
    this->maxWidth = maxWidth;
    this->centered = centered;
}

FontStyle::FontStyle(const FontStyle& st)
{
    this->color = st.color;
    this->size = st.size;
    this->maxWidth = st.maxWidth;
    this->centered = st.centered;
}

size_t FontStyle::Hash(string ofText) const
{
    size_t hash = 0;
    CombineHash(hash, ofText);
    CombineHash(hash, color);
    CombineHash(hash, maxWidth);
    return hash;
}

FontStyle FontStyle::center(bool centered) const
{
    auto c = FontStyle(*this);
    c.centered = centered;
    return c;
}

FontStyle FontStyle::wColor(Color color) const
{
    auto c = FontStyle(*this);
    c.color = color;
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
