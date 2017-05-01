#pragma once

#include "glinclude.h"

#include "math/mdefs.h"
#include "utils/logger.h"

namespace bacchus {

inline void savegl(uchar*& data, uint& w, uint&h) {
    int size[4];
    CHECK_GL_ERROR(glGetIntegerv(GL_VIEWPORT, size));
    w = size[2];
    h = size[3];
    data = new uchar[w*h*4];
    CHECK_GL_ERROR(glReadPixels(0,0,w,h,GL_RGBA,GL_UNSIGNED_BYTE,data));
}

} // namespace bacchus
