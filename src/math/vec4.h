#pragma once

#include <iostream>
#include <cmath>

#include "mdefs.h"
#include "vec2.h"
#include "vec3.h"

namespace bacchus {

template <typename T>
struct vec4 {
    T x,y,z,w;

    vec4(): x(0), y(0), z(0), w(0) {}
    explicit vec4(const T& c): x(c), y(c), z(c), w(c) {}
    vec4(const vec3<T>& v3, const T& w): x(v3.x), y(v3.y), z(v3.z), w(w) {}
    vec4(const vec2<T>& v2, const T& z, const T& w): x(v2.x), y(v2.y), z(z), w(w) {}
    vec4(const vec2<T>& v2a, const vec2<T>& v2b): x(v2a.x), y(v2a.y), z(v2b.x), w(v2b.y) {}
    vec4(const T& x, const T& y, const T& z, const T& w): x(x), y(y), z(z), w(w) {}
    explicit vec4(const T* rawp): x(rawp[0]), y(rawp[1]), z(rawp[2]), w(rawp[3]) {}

    template<typename U>
    vec4(const vec4<U>& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

    T& operator[](int i) {
        return (&(this->x))[i];
    }

    const T& operator[](int i) const {
        return (&(this->x))[i];
    }

    vec4<T>& operator+=(const vec4<T>& v) {
        x += v.x; y += v.y; z += v.z; w += v.w;
        return *this;
    }

    vec4<T>& operator-=(const vec4<T>& v) {
        x -= v.x; y -= v.y; z -= v.z; w -= v.w;
        return *this;
    }

    vec4<T>& operator*=(const T& c) {
        x *= c; y *= c; z *= c; w *= c;
        return *this;
    }

    vec4<T>& operator/=(const T& c) {
        x /= c; y /= c; z /= c; w /= c;
        return *this;
    }

    vec4<T>& operator=(const vec4<T>& v) {
        x = v.x; y = v.y; z = v.z; w = v.w;
        return *this;
    }

    bool operator==(const vec3<T>& v) {
        return eq(x, v.x) && eq(y, v.y) && eq(z, v.z) && eq(w, v.w);
    }

    T length() const {
        return sqrt(x*x + y*y + z*z, w*w);
    }

    vec4<T>& normalize() {
        T inv = T(1)/length();
        x *= inv; y *= inv; z *= inv; w *= inv;
        return *this;
    }

    vec2<T> xy() const {
        return vec2<T>(x,y);
    }

    vec3<T> xyz() const {
        return vec3<T>(x,y,z);
    }
};

template <typename T>
inline vec4<T> normalize(const vec4<T>& v) {
    T inv = T(1)/v.length();
    return vec4<T>(v.x*inv, v.y*inv, v.z*inv, v.w*inv);
}

template <typename T>
inline T dot(const vec4<T>& v1, const vec4<T>& v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}

//template <typename T>
//inline vec4<T> cross(const vec4<T>& v1, const vec4<T>& v2) {
//    return vec4<T>(	v1.y*v2.z - v1.z*v2.y,
//                    v1.z*v2.x - v1.x*v2.z,
//                    v1.x*v2.y - v1.y*v2.x);
//}

template <typename T>
inline vec4<T> operator+(const vec4<T>& v1, const vec4<T>& v2) {
    return vec4<T>(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z, v1.w+v2.w);
}

template <typename T>
inline vec4<T> operator-(const vec4<T>& v1, const vec4<T>& v2) {
    return vec4<T>(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z, v1.w-v2.w);
}

template <typename T, typename U>
inline vec4<T> operator*(const U& c, const vec4<T> & v) {
    return vec4<T>(v.x*c, v.y*c, v.z*c, v.w*c);
}

template <typename T, typename U>
inline vec4<T> operator*(const vec4<T> & v, const U& c) {
    return vec4<T>(v.x*c, v.y*c, v.z*c, v.w*c);
}

template <typename T>
inline vec4<T> operator*(const vec4<T>& v1, const vec4<T>& v2) {
    return vec4<T>(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w);
}

template <typename T>
inline vec4<T> operator/(const vec4<T>& v1, const vec4<T>& v2) {
    return vec4<T>(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z, v1.w/v2.w);
}

template <typename T>
inline vec4<T> operator/(const vec4<T>& v1, const T& c) {
    return vec4<T>(v1.x/c, v1.y/c, v1.z/c, v1.w/c);
}

template <typename T>
inline vec4<T> operator-(const vec4<T>& v) {
    return vec4<T>(-v.x, -v.y, -v.z, -v.w);
}

template <typename T>
inline uint rgba2i(const vec4<T>& v) {
    return  ((uint(v.w) & 0xFF)<<24) +
            ((uint(v.z) & 0xFF)<<16) +
            ((uint(v.y) & 0xFF)<< 8) +
            ((uint(v.x) & 0xFF)    );
}

template <typename T>
inline vec4<T> clamp_color(const vec4<T>& v) {
    return vec4<T>(
                clamp_color(v.x),
                clamp_color(v.y),
                clamp_color(v.z),
                clamp_color(v.w) );
}

template <typename T>
inline vec4<T> max(const vec4<T>& v1, const vec4<T>& v2) {
    return vec4<T>(std::max(v1.x,v2.x),
                   std::max(v1.y,v2.y),
                   std::max(v1.z,v2.z),
                   std::max(v1.w,v2.w)
                   );
}

template <typename T>
inline vec4<T> min(const vec4<T>& v1, const vec4<T>& v2) {
    return vec4<T>(std::min(v1.x,v2.x),
                   std::min(v1.y,v2.y),
                   std::min(v1.z,v2.z),
                   std::min(v1.w,v2.w)
                   );
}

template <typename T>
inline T max(const vec4<T>& v) {
    return std::max(std::max(v.x, v.y)
                    , std::max(v.z, v.w));
}

template <typename T>
inline T min(const vec4<T>& v) {
    return std::min(std::min(v.x, v.y)
                    , std::min(v.z, v.w));
}

template<typename T>
inline std::ostream& operator <<(std::ostream& ostr, const vec4<T>& v) {
    return ostr<<v.x<<" "<<v.y<<" "<<v.z<<" "<<v.w;
}

typedef vec4<float> vec4f;
typedef vec4<int> vec4i;

} // namespace bacchus
