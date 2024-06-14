#include "QtScreenCapture.h"

#include <QGuiApplication>
#include <QScreen>
#include <QPixmap>
#include <Windows.h>


QtScreenCapture::QtScreenCapture()
{
}

std::vector<Pixel> QtScreenCapture::MakeScreenshot(int x, int y, int width, int height) {
    QScreen *screen = QGuiApplication::primaryScreen();
    if(!screen)
        return {};

    QPixmap originalPixmap = screen->grabWindow(0, x, y, width, height);
    QImage image = originalPixmap.toImage().convertToFormat(QImage::Format_RGB888);

    std::vector<Pixel> pixels;
    pixels.reserve(width * height);

    const uchar* imageData = image.constBits();
    qsizetype bytesPerLine = image.bytesPerLine();
    for(qsizetype y = 0; y < height; ++y) {
        const uchar* line = imageData + y * bytesPerLine;
        for(int x = 0; x < width; ++x) {
            const uchar* pixelData = line + x * 3;
            Pixel pixel = { pixelData[0], pixelData[1], pixelData[2] };
            pixels.push_back(pixel);
        }
    }

    return pixels;
}
