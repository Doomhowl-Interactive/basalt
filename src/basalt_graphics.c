#include <string.h>

#include "basalt.h"
#include "basalt_plat.h"

#define BLEND_VALUE 180

// NOTE: Taken from https://github.com/tsoding/olive.c/blob/master/olive.c
typedef struct {
    // Safe ranges to iterate over.
    int x1, x2;
    int y1, y2;

    // Original uncut ranges some parts of which may be outside of the canvas
    // boundaries.
    int ox1, ox2;
    int oy1, oy2;
} Olivec_Normalized_Rect;

BASALT void DrawDot(Texture canvas, int posX, int posY, Color color)
{
    int i = posY * canvas.width + posX;
    canvas.pixels[i] = color;
}

// TODO: Clean up required
// NOTE: Taken from https://github.com/tsoding/olive.c/blob/master/olive.c
BASALT void DrawLine(Texture canvas, int startX, int startY, int endX, int endY, Color color)
{
    int x1 = startX;
    int x2 = endX;
    int y1 = startY;
    int y2 = endY;

    int dx = x2 - x1;
    int dy = y2 - y1;

    // If both of the differences are 0 there will be a division by 0 below.
    if (dx == 0 && dy == 0) {
        if (0 <= x1 && x1 < (int)canvas.width && 0 <= y1 && y1 < (int)canvas.height) {
            DrawDot(canvas, x1, y1, color);
        }
        return;
    }

    if (ABS(int, dx) > ABS(int, dy)) {
        if (x1 > x2) {
            SWAP(int, x1, x2);
            SWAP(int, y1, y2);
        }

        // Cull out invisible line
        if (x1 > (int)canvas.width)
            return;
        if (x2 < 0)
            return;

        // Clamp the line to the boundaries
        if (x1 < 0)
            x1 = 0;
        if (x2 >= (int)canvas.width)
            x2 = (int)canvas.width - 1;

        for (int x = x1; x <= x2; ++x) {
            int y = dy * (x - x1) / dx + y1;
            // TODO: move boundary checks out side of the loops in olivec_draw_line
            if (0 <= y && y < (int)canvas.height) {
                DrawDot(canvas, x, y, color);
            }
        }
    } else {
        if (y1 > y2) {
            SWAP(int, x1, x2);
            SWAP(int, y1, y2);
        }

        // Cull out invisible line
        if (y1 > (int)canvas.height)
            return;
        if (y2 < 0)
            return;

        // Clamp the line to the boundaries
        if (y1 < 0)
            y1 = 0;
        if (y2 >= (int)canvas.height)
            y2 = (int)canvas.height - 1;

        for (int y = y1; y <= y2; ++y) {
            int x = dx * (y - y1) / dy + x1;
            // TODO: move boundary checks out side of the loops in olivec_draw_line
            if (0 <= x && x < (int)canvas.width) {
                DrawDot(canvas, x, y, color);
            }
        }
    }
}

BASALT void DrawRectangle(Texture canvas, int posX, int posY, int width, int height, Color color)
{
    assert(canvas.pixels);

    // assume color is opaque
    color |= 0x000000FF;

    int i = posY * canvas.width + posX;
    for (int y = MAX(0, posY); y < MIN(posY + height, canvas.height); y++) {
        for (int x = MAX(0, posX); x < MIN(posX + width, canvas.width); x++) {
            canvas.pixels[i++] = color;
        }
        i -= width;
        i += canvas.width;
    }
    DRAWCALL(canvas, DrawRectangle);
}

BASALT void DrawRectangleLines(Texture canvas, int posX, int posY, int width, int height, int border, Color color)
{
    DrawRectangle(canvas, posX, posY, width, border, color);  // top
    DrawRectangle(canvas, posX + width - border, posY, border, height,
                  color);  // right
    DrawRectangle(canvas, posX, posY + height - border, width, border,
                  color);                                      // bottom
    DrawRectangle(canvas, posX, posY, border, height, color);  // left
}

static BitmapFont PixelFont = { 0 };
BASALT void DrawText(Texture canvas, const char* text, int posX, int posY, Color color)
{
    if (PixelFont.texture.pixels == NULL) {
        PixelFont.texture = RequestTexture(SPR_PIXELFONT);
        PixelFont.cols = 8;
        PixelFont.rows = 8;
        PixelFont.cellWidth = 8;
        PixelFont.cellHeight = 8;
        PixelFont.symbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.!";
    }
    DrawBitmapText(PixelFont, canvas, text, posX, posY, color);
}

BASALT void DrawBitmapText(BitmapFont font, Texture canvas, const char* text, int posX, int posY, Color tint)
{
    if (text == NULL)
        text = (const char*)"NULL";

    int x = posX;
    int y = posY;
    for (uint i = 0; i < strlen(text); i++) {
        char symbol = text[i];
        switch (symbol) {
            case '\n':  // multiline
                y += font.cellHeight;
                x = posX;
                break;
            case '_':
            case '-':
            case ' ':
                x += font.cellWidth;
                break;
            default: {
                char* found = strchr(font.symbols, symbol);
                if (found == NULL)
                    found = (char*)font.symbols;
                uint symbolIndex = (uint)(found - font.symbols);
                uint cellX = symbolIndex % font.cols;
                uint cellY = symbolIndex / font.rows;
                Rect src = { cellX * font.cellWidth, cellY * font.cellHeight, font.cellWidth, font.cellHeight };
                DrawTextureEx(canvas, font.texture, x, y, R2(src), tint);
                x += font.cellWidth;
            } break;
        }
    }
}

BASALT Texture InitTexture(int width, int height)
{
    Texture tex;
    tex.width = width;
    tex.height = height;
    tex.pixels = (uint*)malloc(width * height * 4);
    return tex;
}

BASALT Texture CopyTexture(Texture source)
{
    Texture new = InitTexture(source.width, source.height);
    CopyTextureInto(new, source);
    return new;
}

BASALT void CopyTextureInto(Texture dest, Texture source)
{
    assert(dest.width == source.width && dest.height == source.height);
    assert(dest.pixels);
    assert(source.pixels);

    memcpy(dest.pixels, source.pixels, source.width * source.height * sizeof(Color));
}

BASALT void DisposeTexture(Texture texture)
{
    if (texture.pixels) {
        free(texture.pixels);
    }
}

BASALT void SwapTextureChannels(Texture dest, Texture src, uchar first, uchar second, uchar third, uchar fourth)
{
    assert(dest.width == src.width && dest.height == src.height);
    assert(dest.pixels);
    assert(src.pixels);

    assert(first < 4);
    assert(second < 4);
    assert(third < 4);
    assert(fourth < 4);

    uchar* destPixels = (uchar*)dest.pixels;
    uchar* srcPixels = (uchar*)src.pixels;
    for (int i = 0; i < dest.width * dest.height; i++) {
        destPixels[i * 4 + 0] = srcPixels[i * 4 + first];
        destPixels[i * 4 + 1] = srcPixels[i * 4 + second];
        destPixels[i * 4 + 2] = srcPixels[i * 4 + third];
        destPixels[i * 4 + 3] = srcPixels[i * 4 + fourth];
    }
}

BASALT inline void MapTextureToCorrectFormat(Texture dest, Texture src)
{
    SwapTextureChannels(dest, src, 1, 2, 3, 0);
}

BASALT void ClearTexture(Texture canvas, Color color)
{
    assert(canvas.pixels);
    for (int i = 0; i < canvas.width * canvas.height; i++) {
        canvas.pixels[i] = color;
    }
}

BASALT inline void DrawTexture(Texture canvas, Texture texture, int posX, int posY, Color tint)
{
    DrawTextureEx(canvas, texture, posX, posY, 0, 0, texture.width, texture.height, tint);
}

BASALT void DrawTextureEx(Texture canvas,
                          Texture texture,
                          int posX,
                          int posY,
                          int srcX,
                          int srcY,
                          int srcWidth,
                          int srcHeight,
                          Color tint)
{
    assert(canvas.pixels);

    // Switch out the texture if hotreloading is on
    HotReloadTexture(texture);

    Color* pixels = (Color*)canvas.pixels;
    int canvasSize = canvas.width * canvas.height;
    int textureSize = texture.width * texture.height;

    for (int y = 0; y < srcWidth; y++) {
        for (int x = 0; x < srcWidth; x++) {
            int xx = posX + x;
            int yy = posY + y;

            int destIndex = yy * canvas.width + xx;
            if (destIndex < 0 || destIndex >= canvasSize)
                continue;

            int srcIndex = (srcY + y) * texture.width + (srcX + x);
            if (srcIndex < 0 || srcIndex >= textureSize)
                continue;

            Color srcColor = texture.pixels[srcIndex];
            uchar alpha = srcColor & 0x000000FF;
            Color tintedColor = srcColor = BlendColors(srcColor, tint, BLEND_VALUE);
            Color finalColor = BlendColors(pixels[destIndex], tintedColor, alpha);

            pixels[destIndex] = finalColor;
        }
    }
    DRAWCALL(canvas, DrawRectangle);
}

func bool olivec_normalize_rect(int x,
                                int y,
                                int w,
                                int h,
                                size_t canvas_width,
                                size_t canvas_height,
                                Olivec_Normalized_Rect* nr);

// NOTE: Taken from https://github.com/tsoding/olive.c/blob/master/olive.c
BASALT void
DrawTextureScaled(Texture canvas, Texture texture, int destX, int destY, int destWidth, int destHeight, Color tint)
{
    assert(texture.pixels);
    assert(canvas.pixels);

    Olivec_Normalized_Rect nr = { 0 };
    if (!olivec_normalize_rect(destX, destY, destWidth, destHeight, canvas.width, canvas.height, &nr))
        return;

    int xa = nr.ox1;
    if (destWidth < 0)
        xa = nr.ox2;
    int ya = nr.oy1;
    if (destHeight < 0)
        ya = nr.oy2;
    for (int y = nr.y1; y <= nr.y2; ++y) {
        for (int x = nr.x1; x <= nr.x2; ++x) {
            size_t nx = (x - xa) * ((int)texture.width) / destWidth;
            size_t ny = (y - ya) * ((int)texture.height) / destHeight;
            size_t srcIndex = ny * texture.width + nx;
            size_t destIndex = y * canvas.width + x;
            Color color = texture.pixels[srcIndex];
            if (tint != WHITE)
                color = BlendColors(color, tint, BLEND_VALUE);
            canvas.pixels[destIndex] = color;
        }
    }
    DRAWCALL(canvas, DrawRectangle);
}

BASALT void DrawWeirdTestGradient(Texture canvas)
{
    assert(canvas.pixels);

    static int xOffset = 0;
    static int yOffset = 0;

    int i = 0;
    for (int y = 0; y < canvas.height; y++) {
        for (int x = 0; x < canvas.width; x++) {
            uchar red = x + xOffset;
            uchar green = y + xOffset;
            canvas.pixels[i] = RGB(red, green, 0);
            i++;
        }
    }

    xOffset++;
    yOffset++;

    DRAWCALL(canvas, DrawRectangle);
}

BASALT inline Color RGBA(uchar r, uchar g, uchar b, uchar a)
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}

BASALT inline Color RGB(uchar r, uchar g, uchar b)
{
    return RGBA(r, g, b, 255);
}

BASALT inline Color BlendColors(Color src, Color dst, uchar t)
{
    assert(t <= 255);
    if (t == 255)
        return dst;
    if (t == 0 || dst == WHITE)
        return src;

    const Color s = 255 - t;
    return ((((((src >> 0) & 0xff) * s + ((dst >> 0) & 0xff) * t) >> 8))
            | (((((src >> 8) & 0xff) * s + ((dst >> 8) & 0xff) * t)) & ~0xff)
            | (((((src >> 16) & 0xff) * s + ((dst >> 16) & 0xff) * t) << 8) & ~0xffff)
            | (((((src >> 24) & 0xff) * s + ((dst >> 24) & 0xff) * t) << 16) & ~0xffffff));
}

// NOTE: Taken from https://github.com/tsoding/olive.c/blob/master/olive.c
func bool
olivec_normalize_rect(int x, int y, int w, int h, size_t canvas_width, size_t canvas_height, Olivec_Normalized_Rect* nr)
{
    // No need to render empty rectangle
    if (w == 0)
        return false;
    if (h == 0)
        return false;

    nr->ox1 = x;
    nr->oy1 = y;

    // Convert the rectangle to 2-points representation
    nr->ox2 = nr->ox1 + SIGN(int, w) * (ABS(int, w) - 1);
    if (nr->ox1 > nr->ox2)
        SWAP(int, nr->ox1, nr->ox2);
    nr->oy2 = nr->oy1 + SIGN(int, h) * (ABS(int, h) - 1);
    if (nr->oy1 > nr->oy2)
        SWAP(int, nr->oy1, nr->oy2);

    // Cull out invisible rectangle
    if (nr->ox1 >= (int)canvas_width)
        return false;
    if (nr->ox2 < 0)
        return false;
    if (nr->oy1 >= (int)canvas_height)
        return false;
    if (nr->oy2 < 0)
        return false;

    nr->x1 = nr->ox1;
    nr->y1 = nr->oy1;
    nr->x2 = nr->ox2;
    nr->y2 = nr->oy2;

    // Clamp the rectangle to the boundaries
    if (nr->x1 < 0)
        nr->x1 = 0;
    if (nr->x2 >= (int)canvas_width)
        nr->x2 = (int)canvas_width - 1;
    if (nr->y1 < 0)
        nr->y1 = 0;
    if (nr->y2 >= (int)canvas_height)
        nr->y2 = (int)canvas_height - 1;

    return true;
}
