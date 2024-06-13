#include "ColorUtils.h"

Tuple<QColor, QColor> ColorUtils::GenerateColorRange(QColor baseColor, byte offset)
{
    auto adjustColor = [](int value, int offset) {
        int newValue = value + offset;
        if(newValue > 255) newValue = 255;
        if(newValue < 0) newValue = 0;
        return newValue;
    };

    int red = baseColor.red();
    int green = baseColor.green();
    int blue = baseColor.blue();

    QColor color1(adjustColor(red, offset), adjustColor(green, offset), adjustColor(blue, offset));
    QColor color2(adjustColor(red, -offset), adjustColor(green, -offset), adjustColor(blue, -offset));

    return { color1, color2 };
}
