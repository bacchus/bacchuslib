#pragma once

#include "filter.h"

namespace bacchus {

class FilterLighting: public Filter {
public:
    FilterLighting();
    void setSigma(float sigma);
    void onResize(int w, int h);

private:
    uvec1f m_sigma;
    uvec2f m_size;
};

} // namespace bacchus
