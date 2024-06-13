#pragma once

#include <string>
#include <Windows.h>

using Byte = unsigned char;

struct Pixel
{
    BYTE Blue;
    BYTE Green;
    BYTE Red;
};

struct ColorRange
{
    Pixel Lower, Higher;
};

struct CaptureViewportParams
{
    LONG X;
    LONG Y;

    LONG Width;
    LONG Height;
};

struct Configuration
{
    ColorRange DetectibleColor;
    CaptureViewportParams ViewportParameters;

    bool ShowConsole;
};

Configuration ParseCommandLineArgs(const std::string& args);