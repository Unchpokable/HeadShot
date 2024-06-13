#include "ScreenScanner.h"

#include <gdiplus.h>

#pragma comment(lib, "Gdiplus.lib")

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
    using namespace Gdiplus;
    UINT  num = 0;
    UINT  size = 0;

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if(size == 0)
        return -1;

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if(pImageCodecInfo == NULL)
        return -1;

    GetImageEncoders(num, size, pImageCodecInfo);
    for(UINT j = 0; j < num; ++j) {
        if(wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;
        }
    }
    free(pImageCodecInfo);
    return 0;
}

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
                const auto pixels = CaptureScreen();
                if(CheckFramePixels(pixels))
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

Array<Pixel>* ScreenScanner::CaptureScreen() const noexcept
{
    const auto params = _config.ViewportParameters;
    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, params.Width, params.Height);

    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

    BitBlt(hMemoryDC, 0, 0, params.Width, params.Height, hScreenDC, params.X, params.Y, SRCCOPY);

    BITMAPINFOHEADER bi = { 0 };
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = params.Width;
    bi.biHeight = -params.Height;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = BI_RGB;

    const auto pixels = new Array<Pixel>(params.Width * params.Height);

    GetDIBits(hMemoryDC, hBitmap, 0, params.Height, pixels->Data(), (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    SelectObject(hMemoryDC, hOldBitmap);

    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);

    return pixels;
}

bool ScreenScanner::CheckFramePixels(const Array<Pixel>* pixels) const noexcept
{
    const auto& [Lower, Higher] = _config.DetectibleColor;

    for (std::size_t i { 0 }; i < pixels->Length(); i++)
    {
        const auto& [Blue, Green, Red] = pixels->Data()[i];
        if(Red >= Lower.Red && Red <= Higher.Red &&
            Green >= Lower.Green && Green <= Higher.Green &&
            Blue >= Lower.Blue && Blue <= Higher.Blue) 
        {
            return true;
        }
    }

    return false;
}

