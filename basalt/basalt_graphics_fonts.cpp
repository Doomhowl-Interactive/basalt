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
        SDL_DestroySurface(surface);
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
    if (LoadedFonts.empty()) {
        spdlog::error("There are no fonts loaded");
        auto defaultFont = LoadFont("FFFFORWA.TTF");
        return defaultFont;
    }

    auto it = LoadedFonts.begin();
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
                throw exception("Failed to load font");
            }

            // store loaded font
            LoadedFonts.insert({ fontName, font });
            spdlog::debug("Loaded font {}.ttf", fontName);

            return { fontName };
        } else {
            spdlog::error("Failed to find font {}", fontName);
            throw exception("Failed to load font");
        }
    }
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

void Texture::DrawBasaltText(std::string text, int posX, int posY, FontStyle style)
{
    DrawBasaltText(text, posX, posY, Font::Default(), style);
}

void Texture::DrawBasaltText(std::string text, int posX, int posY, Font font, FontStyle style)
{
    if (style.size > 1000) {
        spdlog::warn("Font size is too big: {}", style.size);
        return;
    }

    size_t hash = style.Hash(text);
    Size size;
    auto surface = GetOrCacheText(text, font, style, &size);

    SDL_Rect destRect;
    destRect.x = posX;
    destRect.y = posY;
    destRect.w = size.width;
    destRect.h = size.height;

    if (style.centered) {
        destRect.x -= size.width / 2;
        destRect.y -= size.height / 2;
    }

    SDL_BlitSurface(surface->get(), NULL, GetScreenOverlaySurface(), &destRect);
}

void Texture::DrawBasaltTextShadow(string text,
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
