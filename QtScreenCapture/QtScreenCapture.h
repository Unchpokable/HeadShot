#pragma once

#include <vector>
#include <Windows.h>
#include "qtscreencapture_global.h"

struct Pixel
{
    BYTE R, G, B;
};

class QTSCREENCAPTURE_EXPORT QtScreenCapture
{
public:
    QtScreenCapture();

    static std::vector<Pixel> MakeScreenshot(int x, int y, int width, int height);
};
