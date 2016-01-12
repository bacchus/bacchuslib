#pragma once

#include <vector>
#include "math/mdefs.h"
#include "math/matmxn.h"

namespace bacchus {

class Image;

class Fourier {
public:
    static Image& inverse(Image& out, const Image& mag, const Image& arg, float ymin, float ymax);
    static Image& inverse(Image& out, const std::vector< matxcx >& cplximg);
    static std::vector< matxcx > transform(Image& mag, Image& arg, float& ymin, float& ymax, const Image& img1);
};

} // namespace bacchus
