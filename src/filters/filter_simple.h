#pragma once

#include "filter.h"

namespace bacchus {

class FilterSimple: public Filter {
public:
    FilterSimple();
    void setData(float data);
    void onResize(int w, int h);

private:
    uvec1f m_data;
    uvec2f m_size;
};

} // namespace bacchus
