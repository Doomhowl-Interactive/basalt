#include "basalt_plat.h"

pubfunc bool IsMouseDown()
{
    return Input.isMouseDown;
}

pubfunc bool IsMouseUp()
{
    return !Input.isMouseDown;
}

pubfunc bool IsKeyDown(Key code)
{
    return Input.pressedKeys[code];
}

pubfunc bool IsKeyUp(Key code)
{
    return !Input.pressedKeys[code];
}

pubfunc bool IsKeyPressed(Key code)
{
    return Input.pressedKeysOnce[code];
}

pubfunc bool IsKeyReleased(Key code)
{
    return !Input.pressedKeysOnce[code];
}

pubfunc Point GetMousePosition()
{
    return Input.mousePos;
}

pubfunc usize GetFrameIndex()
{
    return Context.frameIndex;
}

pubfunc double GetTimeElapsed()
{
    return Context.timeElapsed;
}

