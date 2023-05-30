#include "basalt.h"

BASALT bool IsMouseDown()
{
    return Input.isMouseDown;
}

BASALT bool IsMouseUp()
{
    return !Input.isMouseDown;
}

BASALT bool IsKeyDown(Key code)
{
    return Input.pressedKeys[code];
}

BASALT bool IsKeyUp(Key code)
{
    return !Input.pressedKeys[code];
}

BASALT bool IsKeyPressed(Key code)
{
    return Input.pressedKeysOnce[code];
}

BASALT bool IsKeyReleased(Key code)
{
    return !Input.pressedKeysOnce[code];
}

BASALT Point GetMousePosition()
{
    return Input.mousePos;
}

BASALT usize GetFrameIndex()
{
    return Context.frameIndex;
}

BASALT double GetTimeElapsed()
{
    return Context.timeElapsed;
}
