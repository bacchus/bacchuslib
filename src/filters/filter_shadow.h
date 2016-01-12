#pragma once

#include "filter.h"

namespace bacchus {

class FilterShadow: public Filter {
public:
    FilterShadow();
    void setDist(float dist);
    void onResize(int w, int h);

private:
    uvec1f m_dist;
    uvec2f m_size;
};

} // namespace bacchus
