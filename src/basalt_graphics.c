#include "basalt.h"
#include "basalt_plat.h"

#include <string.h>

// NOTE: Taken from https://github.com/tsoding/olive.c/blob/master/olive.c
typedef struct {
    // Safe ranges to iterate over.
    int x1, x2;
    int y1, y2;

    // Original uncut ranges some parts of which may be outside of the canvas boundaries.
    int ox1, ox2;
    int oy1, oy2;
} Olivec_Normalized_Rect;

pubfunc void DrawDot(Texture canvas, int posX, int posY, Color color) {
    int i = posY * canvas.width + posX;
    canvas.pixels[i] = color;
}

pubfunc void DrawDotV(Texture canvas, Vec2 pos, Color color)
{
    DrawDot(canvas, pos.x, pos.y, color);
}

// TODO: Clean up required
// NOTE: Taken from https://github.com/tsoding/olive.c/blob/master/olive.c
pubfunc void DrawLine(Texture canvas, int startX, int startY, int endX, int endY, Color color)
{
    int x1 = startX;
    int x2 = endX;
    int y1 = startY;
    int y2 = endY;

    int dx = x2 - x1;
    int dy = y2 - y1;

    // If both of the differences are 0 there will be a division by 0 below.
    if (dx == 0 && dy == 0) {
        if (0 <= x1 && x1 < (int) canvas.width && 0 <= y1 && y1 < (int) canvas.height) {
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
        if (x1 > (int) canvas.width) return;
        if (x2 < 0) return;

        // Clamp the line to the boundaries
        if (x1 < 0) x1 = 0;
        if (x2 >= (int) canvas.width) x2 = (int) canvas.width - 1;

        for (int x = x1; x <= x2; ++x) {
            int y = dy*(x - x1)/dx + y1;
            // TODO: move boundary checks out side of the loops in olivec_draw_line
            if (0 <= y && y < (int) canvas.height) {
                DrawDot(canvas, x, y, color);
            }
        }
    } else {
        if (y1 > y2) {
            SWAP(int, x1, x2);
            SWAP(int, y1, y2);
        }

        // Cull out invisible line
        if (y1 > (int) canvas.height) return;
        if (y2 < 0) return;

        // Clamp the line to the boundaries
        if (y1 < 0) y1 = 0;
        if (y2 >= (int) canvas.height) y2 = (int) canvas.height - 1;

        for (int y = y1; y <= y2; ++y) {
            int x = dx*(y - y1)/dy + x1;
            // TODO: move boundary checks out side of the loops in olivec_draw_line
            if (0 <= x && x < (int) canvas.width) {
                DrawDot(canvas, x, y, color);
            }
        }
    }
}

pubfunc void DrawLineV(Texture canvas, Vec2 start, Vec2 end, Color color)
{
    DrawLine(canvas, start.x, start.y, end.x, end.y, color);
}

pubfunc void DrawRectangle(Texture canvas, int posX, int posY, int width, int height, Color color) {
    assert(canvas.pixels);

    // assume color is opaque
    color |= 0x000000FF;

    int i = posY * canvas.width + posX;
    for (int y = MAX(0,posY); y < MIN(posY + height, canvas.height); y++) {
        for (int x = MAX(0,posX); x < MIN(posX + width, canvas.width); x++) {
            canvas.pixels[i++] = color;
        }
        i -= width;
        i += canvas.width;
    }
    DRAWCALL(canvas, DrawRectangle);
}

pubfunc void DrawRectangleRec(Texture canvas, Rect rect, Color color) {
    DrawRectangle(canvas, rect.x, rect.y, rect.width, rect.height, color);
}

pubfunc void DrawRectangleLines(Texture canvas, int posX, int posY, int width, int height, int border, Color color) {
    DrawRectangle(canvas, posX, posY, width, border, color); // top
    DrawRectangle(canvas, posX + width - border, posY, border, height, color); // right
    DrawRectangle(canvas, posX, posY + height - border, width, border, color); // bottom
    DrawRectangle(canvas, posX, posY, border, height, color); // left
}

pubfunc void DrawRectangleLinesRec(Texture canvas, Rect rect, int border, Color color) {
    DrawRectangleLines(canvas, rect.x, rect.y, rect.width, rect.height, border, color);
}

pubfunc Texture InitTexture(int width, int height) {
    Texture tex;
    tex.width = width;
    tex.height = height;
    tex.pixels = (uint *)malloc(width * height * 4);
    return tex;
}

pubfunc Texture CopyTexture(Texture source){
    Texture new = InitTexture(source.width,source.height);
    CopyTextureInto(new, source);
    return new;
}

pubfunc void CopyTextureInto(Texture dest, Texture source){
    assert(dest.width == source.width && dest.height == source.height);
    assert(dest.pixels);
    assert(source.pixels);

    memcpy(dest.pixels, source.pixels, source.width*source.height*sizeof(Color));
}

pubfunc void DisposeTexture(Texture texture) {
    if (texture.pixels) {
        free(texture.pixels);
    }
}

pubfunc void MapTextureToCorrectFormat(Texture dest, Texture src) {
    // TODO: Add collective assert method, also write assert that prints values
    assert(dest.width == src.width && dest.height == src.height);
    assert(dest.pixels);
    assert(src.pixels);

    uchar* destPixels = (uchar*) dest.pixels;
    uchar* srcPixels = (uchar*) src.pixels;
    for (int i = 0; i < dest.width*dest.height; i++) {
        destPixels[i*4+0] = srcPixels[i*4+1];
        destPixels[i*4+1] = srcPixels[i*4+2];
        destPixels[i*4+2] = srcPixels[i*4+3];
        destPixels[i*4+3] = srcPixels[i*4+0];
    }
}

pubfunc void ClearTexture(Texture canvas, Color color) {
    assert(canvas.pixels);
    for (int i = 0; i < canvas.width * canvas.height; i++) {
        canvas.pixels[i] = color;
    }
}

pubfunc void DrawTexture(Texture canvas, Texture texture, int posX, int posY) {
    Vec2 pos = {posX, posY};
    Rect src = {0, 0, texture.width, texture.height};
    DrawTextureEx(canvas, texture, pos, src);
}

pubfunc void DrawTextureV(Texture canvas, Texture texture, Vec2 pos) {
    Rect src = {0, 0, texture.width, texture.height};
    DrawTextureEx(canvas, texture, pos, src);
}

pubfunc void DrawTextureEx(Texture canvas, Texture texture, Vec2 pos, Rect src) {
    assert(canvas.pixels);

    // Switch out the texture if hotreloading is on
    HotReloadTexture(texture);

    Color *pixels = (Color *)canvas.pixels;

    // TODO: optimize
    // TODO: Bounds checking!
    for (int y = 0; y < src.height; y++) {
        for (int x = 0; x < src.width; x++) {
            int xx = pos.x + x;
            int yy = pos.y + y;
            int destIndex = yy * canvas.width + xx;
            int srcIndex = (src.y + y) * texture.width + (src.x + x);

            // drop pixel if out of bounds
            if (srcIndex < 0 || srcIndex >= texture.width*texture.height) {
                continue;
            }

            if (destIndex < 0 || destIndex >= canvas.width*canvas.height) {
                continue;
            }

            // HACK: Accidental motion blur while messing around
            // Color srcColor = texture.pixels[srcIndex];
            // Color finalColor = BlendColors(pixels[destIndex], srcColor, 0.5f);
            // pixels[destIndex] = finalColor;
            
            Color srcColor = texture.pixels[srcIndex];
            float alpha = (0x000000FF & srcColor) / 255.f;
            Color finalColor = BlendColors(pixels[destIndex], srcColor, alpha);
            pixels[destIndex] = finalColor;
        }
    }
    DRAWCALL(canvas, DrawRectangle);
}

func bool olivec_normalize_rect(int x, int y, int w, int h,
                                size_t canvas_width, size_t canvas_height,
                                Olivec_Normalized_Rect *nr);

// NOTE: Taken from https://github.com/tsoding/olive.c/blob/master/olive.c
pubfunc void DrawTextureScaled(Texture canvas, Texture texture, Rect region) {
    assert(texture.pixels);
    assert(canvas.pixels);

    Olivec_Normalized_Rect nr = { 0 };
    if (!olivec_normalize_rect(region.x, region.y, region.width, region.height,
                               canvas.width, canvas.height, &nr)) return;

    int xa = nr.ox1;
    if (region.width < 0) xa = nr.ox2;
    int ya = nr.oy1;
    if (region.height < 0) ya = nr.oy2;
    for (int y = nr.y1; y <= nr.y2; ++y) {
        for (int x = nr.x1; x <= nr.x2; ++x) {
            size_t nx = (x - xa)*((int) texture.width)/region.width;
            size_t ny = (y - ya)*((int) texture.height)/region.height;
            size_t srcIndex = ny * texture.width + nx;
            size_t destIndex = y * canvas.width + x;
            canvas.pixels[destIndex] = texture.pixels[srcIndex];
        }
    }
    DRAWCALL(canvas, DrawRectangle);
}

pubfunc void DrawWeirdTestGradient(Texture canvas) {
    assert(canvas.pixels);

    static int xOffset = 0;
    static int yOffset = 0;

    int i = 0;
    for (int y = 0; y < canvas.height; y++) {
        for (int x = 0; x < canvas.width; x++) {
            uchar red = x + xOffset;
            uchar green = y + xOffset;
            canvas.pixels[i] = RGB(red,green,0);
            i++;
        }
    }

    xOffset++;
    yOffset++;

    DRAWCALL(canvas, DrawRectangle);
}

pubfunc Color RGBA(uchar r, uchar g, uchar b, uchar a)
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}

pubfunc Color RGB(uchar r, uchar g, uchar b)
{
    return RGBA(r, g, b, 255);
}

pubfunc Color BlendColors(Color color1, Color color2, float percent)
{
    // TODO: cleanup
    unsigned char r1, g1, b1, a1;
    unsigned char r2, g2, b2, a2;
    unsigned char r3, g3, b3, a3;
    
    // Extract the RGBA components of color1
    r1 = (color1 >> 24) & 0xFF;
    g1 = (color1 >> 16) & 0xFF;
    b1 = (color1 >> 8) & 0xFF;
    a1 = color1 & 0xFF;
    
    // Extract the RGBA components of color2
    r2 = (color2 >> 24) & 0xFF;
    g2 = (color2 >> 16) & 0xFF;
    b2 = (color2 >> 8) & 0xFF;
    a2 = color2 & 0xFF;
    
    // Calculate the blended RGBA components
    r3 = (unsigned char)((1 - percent) * r1 + percent * r2);
    g3 = (unsigned char)((1 - percent) * g1 + percent * g2);
    b3 = (unsigned char)((1 - percent) * b1 + percent * b2);
    a3 = (unsigned char)((1 - percent) * a1 + percent * a2);
    
    // Combine the blended RGBA components into a single unsigned int
    return (r3 << 24) | (g3 << 16) | (b3 << 8) | a3;
}

// NOTE: Taken from https://github.com/tsoding/olive.c/blob/master/olive.c
func bool olivec_normalize_rect(int x, int y, int w, int h,
                                size_t canvas_width, size_t canvas_height,
                                Olivec_Normalized_Rect *nr)
{
    // No need to render empty rectangle
    if (w == 0) return false;
    if (h == 0) return false;

    nr->ox1 = x;
    nr->oy1 = y;

    // Convert the rectangle to 2-points representation
    nr->ox2 = nr->ox1 + SIGN(int, w)*(ABS(int, w) - 1);
    if (nr->ox1 > nr->ox2) SWAP(int, nr->ox1, nr->ox2);
    nr->oy2 = nr->oy1 + SIGN(int, h)*(ABS(int, h) - 1);
    if (nr->oy1 > nr->oy2) SWAP(int, nr->oy1, nr->oy2);

    // Cull out invisible rectangle
    if (nr->ox1 >= (int) canvas_width) return false;
    if (nr->ox2 < 0) return false;
    if (nr->oy1 >= (int) canvas_height) return false;
    if (nr->oy2 < 0) return false;

    nr->x1 = nr->ox1;
    nr->y1 = nr->oy1;
    nr->x2 = nr->ox2;
    nr->y2 = nr->oy2;

    // Clamp the rectangle to the boundaries
    if (nr->x1 < 0) nr->x1 = 0;
    if (nr->x2 >= (int) canvas_width) nr->x2 = (int) canvas_width - 1;
    if (nr->y1 < 0) nr->y1 = 0;
    if (nr->y2 >= (int) canvas_height) nr->y2 = (int) canvas_height - 1;

    return true;
}

