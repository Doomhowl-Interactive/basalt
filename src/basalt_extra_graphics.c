#include "basalt.h"
#include "basalt_extra.h"

pubfunc void DrawTextureSheet(Texture canvas, TextureSheet sheet, int frame, Vec2 pos) {
    Assert(sheet.texture);
    Assert(sheet.texture->pixels);

    // wrap frame if needed
    frame = frame % sheet.count;
    if (frame < 0){
        frame = sheet.count - frame;
    }

    Rect source = {
        sheet.origin.x + frame * sheet.cellSize.x,
        sheet.origin.y,
        sheet.cellSize.x,
        sheet.cellSize.y,
    };
    DrawTextureEx(canvas, *sheet.texture, pos, source);
}
