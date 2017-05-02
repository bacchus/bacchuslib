#pragma once

#include <cstring>

#include "glinclude.h"

namespace bacchus {

class Vertex {
public:

    Vertex(const float* mas, uint count, uint size, uint stride, uint offset)
        : m_mas(new float[count*stride])
        , m_count(count)
        , m_size(size)
        , m_stride(stride)
        , m_offset(offset)
    {
        //TODO: to much copying why
        memcpy(m_mas, mas, stride*count*sizeof(float));
    }

    ~Vertex() {
        delete[] m_mas;
    }

    float* ptr() { return &m_mas[m_offset]; }
    uint size() { return m_size; }
    uint stride() { return m_stride*sizeof(float); }
    uint count() { return m_count; }

private:
    float* m_mas;
    uint m_count;
    uint m_size;
    uint m_stride;
    uint m_offset;
};

} // namespace bacchus
