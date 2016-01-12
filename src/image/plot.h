#pragma once

#include "paint.h"
#include "math/matmxn.h"

namespace bacchus {

class Image;

class Plot {
    Paint m_paint;
public:
    Plot(Image& img): m_paint(img) {}
    void plotData(const matxf& x, const matxf& y, uint color);// x : y
    void plotData(const matxf& x, uint color);// n : x
    //void plotLine(float k, float b);// y = kx + b
};

} // namespace bacchus
