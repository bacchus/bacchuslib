#pragma once

#include "math/mdefs.h"

namespace bacchus {

class Image;

class Arthops {
public:
    static Image& add(Image& out, const Image& img1, const Image& img2);
    static Image& add(Image& out, const Image& img1, uchar c);
    static Image& sub(Image& out, const Image& img1, const Image& img2);
    static Image& asub(Image& out, const Image& img1, const Image& img2);
    static Image& sub(Image& out, const Image& img1, uchar c);
    static Image& asub(Image& out, const Image& img1, uchar c);
    static Image& mul(Image& out, const Image& img1, float c);
    static Image& mul(Image& out, const Image& img1, const Image& img2);
    static Image& div(Image& out, const Image& img1, const Image& img2);
    static Image& blend(Image& out, const Image& img1, const Image& img2, float c);
    static Image& blend(Image& out, const Image& img1, const Image& img2);
    static Image& logic_not(Image& out, const Image& img1);
    static Image& logic_and(Image& out, const Image& img1, const Image& img2);
    static Image& logic_or(Image& out, const Image& img1, const Image& img2);
    static Image& logic_xor(Image& out, const Image& img1, const Image& img2);
    static Image& logarithm(Image& out, const Image& img1);
    static Image& exponential(Image& out, const Image& img1, float b, float c);
};

} // namespace bacchus
