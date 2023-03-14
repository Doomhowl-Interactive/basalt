#include "basalt.h"
#include "basalt_extra.h"

// TODO: bounds check frame index
void BlitTextureSheet(Texture canvas, TextureSheet sheet, int frame, Vec2 pos) {
    Assert(sheet.texture);
    Assert(sheet.texture->pixels);
    Rect source = {
        sheet.origin.x + frame * sheet.cellSize.x,
        sheet.origin.y,
        sheet.cellSize.x,
        sheet.cellSize.y,
    };
    BlitTextureEx(canvas, *sheet.texture, pos, source);
}
