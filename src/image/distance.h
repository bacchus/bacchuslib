#pragma once

namespace bacchus {

class Image;

class Distance {
public:
    static Image& transform(Image& out, const Image& img1);

protected:
    static Image& transform_wave(Image& out, const Image& img1);
    static void transform_test(const Image& img1);

};

} // namespace bacchus
