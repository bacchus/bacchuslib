#pragma once

namespace bacchus {

class Image;

class Edgedetect {
public:

    static Image& robcross(Image& out, const Image& img1);
    static Image& sobel(Image& out, const Image& img1);
    static Image& sobel_2wrap(Image& out, const Image& img1);
    static Image& compass(Image& out, const Image& img1);
    static Image& zerocross(Image& out, const Image& img1, float s=1, int r=1);
    static Image& detect_lines(Image& out, const Image& img1);

};

} // namespace bacchus
