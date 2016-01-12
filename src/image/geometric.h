#pragma once

#include "math/mdefs.h"

namespace bacchus {

class Image;

class Geometric {
public:
    enum Filter {
        FILTER_NEAREST,
        FILTER_LINEAR
    };

    static Image& affine(Image& out, const Image& img1
                         , int x10, int y10, int x11, int y11, int x12, int y12
                         , int x20, int y20, int x21, int y21, int x22, int y22);
    static Image& affine(Image& out, const Image& img1, float a00, float a01, float a10, float a11, float b0, float b1);
    static Image& translate(Image& out, const Image& img1, int x, int y);
    static Image& reflect(Image& out, const Image& img1, int x, int y, float dg);
    static Image& rotate(Image& out, const Image& img1, int x, int y, float dg);
    static Image& scale_down(Image& out, const Image& img1, int w, int h, Filter filter);
    static Image& scale_up_size(Image& out, const Image& img1, int w, int h, int dx, int dy, Filter filter);
    static Image& scale_up(Image& out, const Image& img1, int w, int h, Filter filter);
    static Image& scale(Image& out, const Image& img1, int w, int h, Filter filter);
    static Image& flip_vert(Image& out, const Image& img1);

};

} // namespace bacchus
