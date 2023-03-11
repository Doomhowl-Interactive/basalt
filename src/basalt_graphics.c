#include "basalt.h"

void RenderSprite(Canvas canvas, Texture texture, int posX, int posY){
    uint *pixels = (uint*) canvas.pixels;

    // TODO: optimize
    int j = 0;
    for (int y = 0; y < texture.height; y++){
        for (int x = 0; x < texture.width; x++){
            int xx = posX + x;
            int yy = posY + y;
            int i = yy * canvas.width + xx;
            pixels[i] = texture.pixels[j++];
        }
    }

}

void RenderSpriteV(Canvas canvas, Texture texture, Vec2 pos){
    RenderSprite(canvas,texture,(int)pos.x,(int)pos.y);
}

void RenderWeirdTestGradient(Canvas canvas)
{
    static int xOffset = 0;
    static int yOffset = 0;

    int i = 0;
    for(int y = 0; y < canvas.height; y++)
    {
        for(int x = 0; x < canvas.width; x++)
        {
            uchar blue = (x + xOffset);
            uchar green = (y + yOffset);
            canvas.pixels[i] = ((green << 16) | blue);
            i++;
        }
    }

    xOffset++;
    yOffset++;
}
