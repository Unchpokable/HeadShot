#include "ScreenScanner.h"

#define NATIVE_SCREENSHOTS

#ifdef NATIVE_SCREENSHOTS
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
#endif

ScreenScanner::~ScreenScanner() 
{
    if(_worker) 
    {
        _worker->join();
        _worker->detach();
        delete _worker;
    }
}

void ScreenScanner::Start()
{
    _scanActive = true;
    if(_worker && _worker->joinable())
        _worker->join();

    _worker = new std::thread([this]() {
        while(_scanActive)
        {
            if (_needScan)
            {
                CaptureScreen();
                if(CheckFramePixels())
                    EmitCallback();
            }

            Sleep(1);
        }
    });
}

void ScreenScanner::Stop() {
    _scanActive = false;
}

void ScreenScanner::SetNeedScanning(bool flag)
{
    _needScan = flag;
}

void ScreenScanner::CaptureScreen() const noexcept
{
    const auto vpParams = _config.ViewportParameters;
    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, vpParams.Width, vpParams.Height);
    HGDIOBJ oldBitmap = SelectObject(hMemoryDC, hBitmap);

    BitBlt(hMemoryDC, 0, 0, vpParams.Width, vpParams.Height, hScreenDC, vpParams.X, vpParams.Y, SRCCOPY);

    BITMAP bitmap;
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    memset(_data, 0, sizeof(BYTE) * vpParams.Width * vpParams.Height);

    BITMAPINFOHEADER bi = { 0 };
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = vpParams.Width;
    bi.biHeight = -vpParams.Height; 
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = BI_RGB;

    GetDIBits(hMemoryDC, hBitmap, 0, vpParams.Height, _data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    SelectObject(hMemoryDC, oldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);
}

bool ScreenScanner::CheckFramePixels() const noexcept
{
    const auto& [x,y,wid, len] = _config.ViewportParameters;
    const auto& [Lower, Higher] = _config.DetectibleColor;

    for(std::size_t i { 0 }; i < static_cast<unsigned long long>(wid) * len; i++)
    {
        const auto& [Blue, Green, Red] = _data[i];
        if(Red >= Lower.Red && Red <= Higher.Red &&
            Green >= Lower.Green && Green <= Higher.Green &&
            Blue >= Lower.Blue && Blue <= Higher.Blue) 
        {
            return true;
        }
    }

    return false;
}

