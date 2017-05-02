#include "uniform.h"
#include "values.h"

namespace bacchus {

void Uniform::value(const uvec1i& val) {
    value1i(val.x);
}

void Uniform::value(const uvec1f& val) {
    value1f(val.x);
}

void Uniform::value(const uvec2f& val) {
    value2f(val.x, val.y);
}

void Uniform::value(const uvec4f& val) {
    value4f(val.x, val.y, val.z, val.w);
}

void Uniform::value(const umat4f& val) {
    valueM4(&val[0][0]);//.data()
}

void Uniform::value(const uvec1fv& val) {
    valueV1(val.size(), val.data());
}

} // namespace bacchus
