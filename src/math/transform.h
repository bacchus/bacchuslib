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
    const float delta = znear - zfar;

    res[0][0] = f/ratio;
    res[1][1] = f;
    res[2][2] = (znear + zfar)/delta;
    res[3][2] = -1.0f;
    res[2][3] = znear*zfar/delta;//2.0f*
    res[3][3] = 0.0f;

    return res;
}

//angle in deg
inline mat4f rotate(const vec3f& axis, float angle_deg) {
    const float angle = deg2rad(angle_deg);
    const float s = std::sin(angle);
    const float c = std::cos(angle);
    const vec3f ax = normalize(axis);

    mat4f res;
    res[3][3] = 1.0f;

    for (int i = 0; i < 3; ++i) {
        res[(i+1)%3][i] =  s*ax[(i+2)%3];
        res[(i+2)%3][i] = -s*ax[(i+1)%3];
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            res[j][i] += (1.0f - c)*ax[i]*ax[j] + (i==j ? c : 0.0f);
        }
    }

    return res;
}

} // namespace bacchus
