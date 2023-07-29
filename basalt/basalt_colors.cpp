#include <cassert>
#include "basalt_colors.hpp"
#include "basalt_macros.hpp"

// ==== COLOR UTILTIES ====
Color MakeRGB(uchar r, uchar g, uchar b, uchar a)
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}

Color MakeRGBf(float r, float g, float b, float a)
{
    return MakeRGB((uchar)(r * 255.f), (uchar)(g * 255.f), (uchar)(b * 255.f), (uchar)(a * 255.f));
}

Color BlendColors(Color src, Color dst, int overrideAlpha)
{
    uchar alpha = overrideAlpha == -1 ? dst & 0x000000FF : (uchar)overrideAlpha;
    assert(alpha <= 255);
    if (alpha == 255)
        return dst;
    if (alpha == 0 || dst == WHITE)
        return src;

    const Color s = 255 - alpha;
    return ((((((src >> 0) & 0xff) * s + ((dst >> 0) & 0xff) * alpha) >> 8))
            | (((((src >> 8) & 0xff) * s + ((dst >> 8) & 0xff) * alpha)) & ~0xff)
            | (((((src >> 16) & 0xff) * s + ((dst >> 16) & 0xff) * alpha) << 8) & ~0xffff) | 255);
}

Color BlendColorsAlpha(Color src, Color dst, int overrideAlpha)
{
    uchar alpha = overrideAlpha == -1 ? dst & 0x000000FF : (uchar)overrideAlpha;
    assert(alpha <= 255);
    if (alpha == 255)
        return dst;
    if (alpha == 0 || dst == WHITE)
        return src;

    const Color s = 255 - alpha;
    return ((((((src >> 0) & 0xff) * s + ((dst >> 0) & 0xff) * alpha) >> 8))
            | (((((src >> 8) & 0xff) * s + ((dst >> 8) & 0xff) * alpha)) & ~0xff)
            | (((((src >> 16) & 0xff) * s + ((dst >> 16) & 0xff) * alpha) << 8) & ~0xffff)
            | (((((src >> 24) & 0xff) * s + ((dst >> 24) & 0xff) * alpha) << 16) & ~0xffffff));
}

Color ColorAlpha(Color col, float a)
{
    auto alpha = (uchar)(a * 255.f);
    return (col & 0xFFFFFF00) | alpha;
}
