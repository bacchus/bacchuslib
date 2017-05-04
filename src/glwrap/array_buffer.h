#pragma once

#include "glinclude.h"

namespace bacchus {

template<uint TARGET>
class ArrayBuffer {
public:

    static void bind(uint glid) {
        CHECK_GL_ERROR(glBindBuffer(TARGET, glid));
    }

    static void unbind() {
        CHECK_GL_ERROR(glBindBuffer(TARGET, 0));
    }

    ArrayBuffer()
    {}

    ArrayBuffer(GLsizei size, GLvoid* data)
        : m_size(size)
        , m_data(data)
    {}

    explicit ArrayBuffer(uint glid)
        : id(glid)
    {}

    ~ArrayBuffer()
    {}

    void createGl() {
        CHECK_GL_ERROR(glGenBuffers(1, &id));
    }

    void bind() {
        bind(id);
    }

    void destroyGl() {
        CHECK_GL_ERROR(glDeleteBuffers(1, &id));
    }

    void load(uint size, void* data) {
        m_size = size;
        m_data = data;
        load();
    }

    void load() {
        bind();
        CHECK_GL_ERROR(glBufferData(TARGET, m_size, m_data, GL_STATIC_DRAW));
        unbind();
    }

private:
    uint id = 0;
    uint m_size = 0;
    void* m_data = nullptr;

    friend inline uint getId(const ArrayBuffer& ab);
};

template<uint TARGET>
inline uint getId(const ArrayBuffer<TARGET>& ab) {
    return ab.id;
}

typedef ArrayBuffer<GL_ARRAY_BUFFER> VertBuffer;
typedef ArrayBuffer<GL_ELEMENT_ARRAY_BUFFER> IndxBuffer;

}
