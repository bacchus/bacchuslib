#pragma once

#include <GLES3/gl3.h>

#include "utils/logger.h"

namespace bacchus {

class uvec1i;
class uvec1f;
class uvec2f;
class uvec4f;
class umat4f;
class uvec1fv;

class Uniform {
public:

    explicit Uniform(int location) {
        id = location;
    }

    void value1i(int value) {
        CHECK_GL_ERROR(glUniform1i(id, value));
    }

    void value1f(float value) {
        CHECK_GL_ERROR(glUniform1f(id, value));
    }

    void value2f(float v1, float v2) {
        CHECK_GL_ERROR(glUniform2f(id, v1, v2));
    }

    void value4f(float v1, float v2, float v3, float v4) {
        CHECK_GL_ERROR(glUniform4f(id, v1, v2, v3, v4));
    }

    void valueM3(const float* value) {
        CHECK_GL_ERROR(glUniformMatrix3fv(id, 1, false, value));
    }

    void valueM4(const float* value) {
        CHECK_GL_ERROR(glUniformMatrix4fv(id, 1, false, value));
    }

    void valueV1(int n, const float* value) {
        CHECK_GL_ERROR(glUniform1fv(id, n, value));
    }

    void value(const uvec1i& val);
    void value(const uvec1f& val);
    void value(const uvec2f& val);
    void value(const uvec4f& val);
    void value(const umat4f& val);
    void value(const uvec1fv& val);

private:
    int id;
};

} // namespace bacchus
