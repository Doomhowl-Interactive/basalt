#include "basalt.h"

// NOTE: Taken from https://github.com/tsoding/olive.c/blob/master/olive.c
typedef struct {
    // Safe ranges to iterate over.
    int x1, x2;
    int y1, y2;

    // Original uncut ranges some parts of which may be outside of the canvas boundaries.
    int ox1, ox2;
    int oy1, oy2;
} Olivec_Normalized_Rect;

pubfunc void DrawDot(Texture canvas, int posX, int posY, int radius, Color color) {
    int halfRadius = MAX(1, radius / 2);
    int topLeftX = posX - halfRadius;
    int topLeftY = posY - halfRadius;
    DrawRectangle(canvas, topLeftX, topLeftY, radius, radius, color);
}

pubfunc void DrawDotV(Texture canvas, Vec2 pos, int radius, Color color) {
    DrawDot(canvas, pos.x, pos.y, radius, color);
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
}

pubfunc void DrawRectangleRec(Texture canvas, Rect rect, Color color) {
    DrawRectangle(canvas, rect.x, rect.y, rect.width, rect.height, color);
}

pubfunc void DrawRectangleRecF(Texture canvas, RectF rect, Color color) {
    DrawRectangle(canvas, (int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, color);
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

pubfunc void DrawRectangleLinesRecF(Texture canvas, RectF rect, int border, Color color) {
    DrawRectangleLines(canvas, (int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, border, color);
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
    memcpy(dest.pixels, source.pixels, source.width*source.height*sizeof(Color));
}

pubfunc void DisposeTexture(Texture texture) {
    if (texture.pixels) {
        free(texture.pixels);
    }
}

pubfunc void MapTextureToCorrectFormat(Texture texture) {
    assert(texture.pixels);

    uchar* pixels = (uchar*) texture.pixels;

    for (int i = 0; i < texture.width*texture.height; i++) {
        pixels[i*4+0] = pixels[i*4+1];
        pixels[i*4+1] = pixels[i*4+2];
        pixels[i*4+2] = pixels[i*4+3];
        pixels[i*4+3] = pixels[i*4+0];
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

            // drop pixel if opacity not 255 (temporary)
            pixels[destIndex] = texture.pixels[srcIndex];
        }
    }
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
}

pubfunc Color RGBA(uchar r, uchar g, uchar b, uchar a){
    return (r << 24) | (g << 16) | (b << 8) | a;
}

pubfunc Color RGB(uchar r, uchar g, uchar b){
    return RGBA(r, g, b, 255);
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

