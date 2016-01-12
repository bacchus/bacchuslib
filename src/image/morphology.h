#pragma once

namespace bacchus {

class Image;

class Morphology {
public:
    static Image& skiz(Image& out, const Image& img1);
    static Image& thick(Image& out, const Image& img1);
    static Image& thinn(Image& out, const Image& img1);
    static Image& hit_miss_corners(Image& out, const Image& img1);
    static Image& close(Image& out, const Image& img1, int r=1);
    static Image& open(Image& out, const Image& img1, int r=1);
    static Image& erode(Image& out, const Image& img1, int r=1);
    static Image& dilate(Image& out, const Image& img1, int r=1);
};

} // namespace bacchus
