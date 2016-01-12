#pragma once

#include "math/mdefs.h"

namespace bacchus {

class Image;

class Smooth {
public:
    static Image& laplac_of_gauss(Image& out, const Image& img1, float s=1, int r=1);
    static Image& laplac(Image& out, const Image& img1, int eges=1);
    static Image& crimmins_lum(Image& out, const Image& img1, int n=1);
    static Image& crimmins(Image& out, const Image& img1, int n=1);
    static Image& conservative_lum(Image& out, const Image& img1, int r=1);
    static Image& conservative(Image& out, const Image& img1, int r=1);
    static Image& gauss_2wrap_lum(Image& out, const Image& img1, float s=1, int r=1);
    static Image& gauss_2wrap(Image& out, const Image& img1, float s=1, int r=1);
    static Image& gauss_slow(Image& out, const Image& img1, float s=1, int r=1);
    static Image& gauss_2r(Image& out, const Image& img1);
    static Image& median_lum(Image& out, const Image& img1, int r=1);
    static Image& median(Image& out, const Image& img1, int r=1);
    static Image& mean_lum(Image& out, const Image& img1, int r=1);
    static Image& mean(Image& out, const Image& img1, int r=1);
    static Image& sharpen3(Image& out, const Image& img1);
};

} // namespace bacchus
