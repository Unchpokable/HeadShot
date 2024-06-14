#include "QtScreenCapture.h"

#include <QGuiApplication>
#include <QScreen>
#include <QPixmap>


QtScreenCapture::QtScreenCapture()
{
}

std::vector<Pixel> QtScreenCapture::MakeScreenshot(int xpos, int ypos, int width, int height)
{
    static int argc = 0;
    static char* argv[] = { (char*)"" };
    static QGuiApplication app(argc, argv);

    QScreen *screen = QGuiApplication::primaryScreen();
    if(!screen)
        return {};

    QPixmap originalPixmap = screen->grabWindow(0, xpos, ypos, width, height);
    const QImage image = originalPixmap.toImage().convertToFormat(QImage::Format_RGB888);

    std::vector<Pixel> pixels;
    pixels.reserve(static_cast<std::size_t>(width) * height);

    const uchar* imageData = image.constBits();
    const qsizetype bytesPerLine = image.bytesPerLine();
    for(qsizetype y = 0; y < height; ++y) 
    {
        const uchar* line = imageData + y * bytesPerLine;
        for(qsizetype x = 0; x < width; ++x) 
        {
            const uchar* pixelData = line + x * 3;
            pixels.push_back({ pixelData[0], pixelData[1], pixelData[2] });
        }
    }

    return pixels;
}
