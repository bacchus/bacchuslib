#pragma once

#include "filter.h"

namespace bacchus {

class FilterGauss: public Filter {
public:
    FilterGauss();
    void setSigma(float sigma);
    void onResize(int w, int h);

private:
    uvec1f m_sigma;
    uvec1fv m_kern;
    uvec1fv m_offs;
    uvec2f m_size;
};

} // namespace bacchus
