#include "ScreenScanner.h"

#define QT_SCREENSHOTS

#ifdef QT_SCREENSHOTS
#include "../QtScreenCapture/Export.h"
#endif

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
    const auto vpParams = _config.ViewportParameters;
#ifdef NATIVE_SCREENSHOTS

    using namespace Gdiplus;
    IStream* istream;
    HRESULT res = CreateStreamOnHGlobal(NULL, true, &istream);
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    {
        auto out = new Array<Pixel>(vpParams.Height * vpParams.Width);
        HDC scrdc, memdc;
        HBITMAP membit;
        scrdc = ::GetDC(0);
        int Height = GetSystemMetrics(SM_CYSCREEN);
        int Width = GetSystemMetrics(SM_CXSCREEN);
        memdc = CreateCompatibleDC(scrdc);
        membit = CreateCompatibleBitmap(scrdc, Width, Height);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, membit);
        BitBlt(memdc, vpParams.X, vpParams.Y, vpParams.Width, vpParams.Height, scrdc, 0, 0, SRCCOPY);

        Bitmap bitmap(membit, NULL);
        BitmapData bitmapData;

        DeleteObject(memdc);
        DeleteObject(membit);
        ::ReleaseDC(0, scrdc);
    }
    GdiplusShutdown(gdiplusToken);
#endif
    const auto transferArray = MakeQtScreenshot(vpParams.X, vpParams.Y, vpParams.Width, vpParams.Height);
    const auto ret = new Array<Pixel>();
    ret->SetData(reinterpret_cast<Pixel*>(transferArray.Data), transferArray.DataLength);
    return ret;
#ifdef QT_SCREENSHOTS
#endif
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

