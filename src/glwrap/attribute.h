#pragma once

#include <GLES3/gl3.h>

#include "utils/logger.h"

namespace bacchus {

class Attribute {
public:

    Attribute(int location) {
        id = location;
    }

    int location() {
        return id;
    }

    void enable() {
        CHECK_GL_ERROR(glEnableVertexAttribArray(id));
    }

    void disable() {
        CHECK_GL_ERROR(glDisableVertexAttribArray(id));
    }

    void vertexPointer(int size, int stride, float* ptr) {
        CHECK_GL_ERROR(glVertexAttribPointer(id, size, GL_FLOAT, GL_FALSE, stride, ptr));
    }

private:
    int id;
};

} // namespace bacchus
