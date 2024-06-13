#include "Input.h"

MousePos GetMousePosition(void)
{
    POINT point;
    MousePos position;

    position.X = 0;
    position.Y = 0;

    if (GetCursorPos(&point))
    {
        position.X = point.x;
        position.Y = point.y;
    }

    return position;
}

void MouseLeft(void)
{
    MousePos where = GetMousePosition();

    mouse_event(MOUSEEVENTF_LEFTDOWN, where.X, where.Y, 0, 0);
    Sleep(50);

    where = GetMousePosition();
    mouse_event(MOUSEEVENTF_LEFTUP, where.X, where.Y, 0, 0);
}