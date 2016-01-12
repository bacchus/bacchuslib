#pragma once

#include <vector>

#include "math/mdefs.h"
#include "math/vec2.h"

namespace bacchus {

class Image;

class Hough {
public:
    static std::vector<vec2f> transform(Image &out, const Image& img1);

};

} // namespace bacchus
