#pragma once

#include <QColor>

template<typename Ft, typename St>
struct Tuple
{
    Ft First;
    St Second;
};

using byte = unsigned char;

class ColorUtils
{
public:
    static Tuple<QColor, QColor>  GenerateColorRange(QColor baseColor, byte offset);
};

