#pragma once

#include <iostream>

#include "mdefs.h"
#include "vec4.h"
#include "mat3.h"
#include "mat4.h"

namespace bacchus {

inline vec3f m2v(const vec4f& m) {
    return vec3f(m[0]/m[3], m[1]/m[3], m[2]/m[3]);
}

inline vec4f v2m(const vec3f& v) {
    return vec4f(v.x, v.y, v.z, 1.f);
}

// world cube [-1,1]*[-1,1]*[-1,1] -> [0,w]*[0,h]*[0,255]
inline mat4f viewport(float x, float y, float w, float h, float d) {
    mat4f res; res.one();
    res[0][3] = x + 0.5f*w;
    res[1][3] = y + 0.5f*h;
    res[2][3] = 0.5f*d;//1.f

    res[0][0] = 0.5f*w;
    res[1][1] = 0.5f*h;
    res[2][2] = 0.5f*d;//0.f

    return res;
}

inline mat4f lookat(const vec3f& eye, const vec3f& center, const vec3f& up) {
    vec3f z = normalize(eye - center);
    vec3f x = normalize(cross(up, z));
    vec3f y = normalize(cross(z, x));
    mat4f res; res.one();
    for (int i = 0; i < 3; ++i) {
        res[0][i] = x[i];
        res[1][i] = y[i];
        res[2][i] = z[i];
        res[i][3] = -center[i];
    }
    return res;
}

inline mat4f projection(float coef) {
    mat4f res; res.one();
    res[3][2] = coef;// coef = -1/camz
    return res;
}

inline mat4f perspective(float fov_deg, float ratio, float znear, float zfar) {
    mat4f res;

    const float f = 1.0f/std::tan(0.5f*deg2rad(fov_deg));
    const float dz = znear - zfar;

    res[0][0] = f/ratio;
    res[1][1] = f;
    res[2][2] = (znear + zfar)/dz;
    res[3][2] = -1.0f;
    res[2][3] = 2.0f*znear*zfar/dz;
    res[3][3] = 0.0f;

    return res;
}

inline mat4f frustrum(float right, float left, float top, float bottom, float znear, float zfar) {
    mat4f res;

    const float dx = right - left;
    const float dy = top - bottom;
    const float dz = znear - zfar;

    res[0][0] = 2.0f*znear/dx;
    res[1][1] = 2.0f*znear/dy;
    res[0][2] = (right + left)/dx;
    res[1][2] = (top + bottom)/dy;
    res[2][2] = (znear + zfar)/dz;
    res[3][2] = -1.0f;
    res[2][3] = 2.0f*znear*zfar/dz;
    res[3][3] = 0.0f;

    return res;
}

inline mat4f rotate(const vec3f& axis, float angle_deg) {
    const float angle = deg2rad(angle_deg);
    const float s = std::sin(angle);
    const float c = std::cos(angle);
    const vec3f ax = normalize(axis);

    mat4f res;
    res[3][3] = 1.0f;

    res[1][2] = -s*ax[0];
    res[2][1] =  s*ax[0];

    res[2][0] = -s*ax[1];
    res[0][2] =  s*ax[1];

    res[0][1] = -s*ax[2];
    res[1][0] =  s*ax[2];


    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            res[j][i] += (1.0f - c)*ax[i]*ax[j] + (i==j ? c : 0.0f);
        }
    }

    return res;
}

inline void translate(mat4f& mat, const vec3f& vec) {
    mat[0][3] += vec.x*mat[0][0] + vec.y*mat[0][1] + vec.z*mat[0][2];
    mat[1][3] += vec.x*mat[1][0] + vec.y*mat[1][1] + vec.z*mat[1][2];
    mat[2][3] += vec.x*mat[2][0] + vec.y*mat[2][1] + vec.z*mat[2][2];
    mat[3][3] += vec.x*mat[3][0] + vec.y*mat[3][1] + vec.z*mat[3][2];
}

inline void scale(mat4f& mat, const vec3f& vec) {
    mat[0][0] *= vec.x; mat[1][0] *= vec.x; mat[2][0] *= vec.x; mat[3][0] *= vec.x;
    mat[0][1] *= vec.y; mat[1][1] *= vec.y; mat[2][1] *= vec.y; mat[3][1] *= vec.y;
    mat[0][2] *= vec.z; mat[1][2] *= vec.z; mat[2][2] *= vec.z; mat[3][2] *= vec.z;
}

inline void rotate(mat4f& mat, const vec3f& axis, float angle_deg) {
    mat4f rot;

    const float angle = deg2rad(angle_deg);
    const float s = std::sin(angle);
    const float c = std::cos(angle);
    const vec3f ax = normalize(axis);

    const vec3f xyz = vec3f(ax.y*ax.z, ax.z*ax.x, ax.x*ax.y)*(1 - c);
    const vec3f ax2 = ax*ax;
    const vec3f diag = ax2 + c*(vec3f(1.0f) - ax2);
    const vec3f sax = s*ax;
    const vec3f plus = xyz + sax;
    const vec3f mins = xyz - sax;

    rot[0][0] = diag[0];
    rot[1][1] = diag[1];
    rot[2][2] = diag[2];
    rot[3][3] = 1.0f;

    rot[1][2] = mins[0]; rot[2][1] = plus[0];
    rot[2][0] = mins[1]; rot[0][2] = plus[1];
    rot[0][1] = mins[2]; rot[1][0] = plus[2];

    mat *= rot;
}

} // namespace bacchus
