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
    //mat4f tr; tr.one();
    for (int i = 0; i < 3; ++i) {
        res[0][i] = x[i];
        res[1][i] = y[i];
        res[2][i] = z[i];
        res[i][3] = -center[i];
        //tr[i][3] = -center[i];
    }
    return res;
    //return res*tr;
}

inline mat4f projection(float coef) {
    mat4f res; res.one();
    res[3][2] = coef;// coef = -1/camz
    return res;
}

} // namespace bacchus
