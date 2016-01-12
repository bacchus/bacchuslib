#pragma once

#include "math/mdefs.h"

namespace bacchus {

class Image;

class Histogram {
public:
    static void histogram(const Image& img1, int* h, int chanel=0);
    static Image& threshold_constant(Image& out, const Image& img1, uchar t);
    static Image& threshold_global(Image& out, const Image& img1, int threshold_rise);
    static Image& threshold_adaptive(Image& out, const Image& img1, int r, int c=0);
    static Image& contrast_stretch(Image& out, const Image& img1, uchar a, uchar b);
    static Image& histogram_equalize(Image& out, const Image& img1);
    static Image& histogram_equalize_local(Image& out, const Image& img1, int r);
};

} // namespace bacchus
