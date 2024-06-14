#include <cstring>
#include "Export.h"
#include "QtScreenCapture.h"

TransferArray MakeQtScreenshot(int x, int y, int width, int height)
{
    const auto screensCap = new QtScreenCapture();

    auto pixels = screensCap->MakeScreenshot(x, y, width, height);

    TransferArray out;
    out.DataLength = pixels.size();
    out.Data = new BYTE[out.DataLength];

    memcpy(out.Data, pixels.data(), out.DataLength * sizeof(BYTE));

    return out;
}
