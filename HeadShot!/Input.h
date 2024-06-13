#pragma once

#include <Windows.h>

typedef struct MousePos__
{
    LONG X, Y;
} MousePos, *PMousePos;

MousePos GetMousePosition(void);

void MouseLeft(void);
