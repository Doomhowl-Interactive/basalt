#include "basalt.h"
#include "basalt_extra.h"

void BlitTextureSheet(Texture canvas, TextureSheet sheet, int frame, Vec2 pos) {
    Assert(sheet.texture);
    Assert(sheet.texture->pixels);
    Rect source = {
        sheet.origin.x + frame * sheet.cellSize.x,
        sheet.origin.y + frame * sheet.cellSize.y,
        sheet.cellSize.x,
        sheet.cellSize.y,
    };
    BlitTextureEx(canvas, *sheet.texture, pos, source);
}
