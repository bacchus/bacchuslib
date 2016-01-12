#pragma once

#include <iostream>
#include <cmath>

#include "mdefs.h"
#include "vec2.h"

namespace bacchus {

template <typename T>
struct vec3 {
    T x,y,z;

    vec3(): x(0), y(0), z(0) {}
    explicit vec3(const T& c): x(c), y(c), z(c) {}
    vec3(const T& x, const T& y, const T& z): x(x), y(y), z(z) {}
    vec3(const vec2<T>& v2, const T& z): x(v2.x), y(v2.y), z(z) {}
    explicit vec3(const T* rawp): x(rawp[0]), y(rawp[1]), z(rawp[2]) {}
    template<typename U> vec3(const vec3<U>& v) : x(v.x), y(v.y), z(v.z) {}

    T& operator[](int i) {
        return (&(this->x))[i];
    }

    const T& operator[](int i) const {
        return (&(this->x))[i];
    }

    vec3<T>& operator+=(const vec3<T>& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    vec3<T>& operator-=(const vec3<T>& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    vec3<T>& operator*=(const T& c) {
        x *= c;
        y *= c;
        z *= c;
        return *this;
    }

    vec3<T>& operator/=(const T& c) {
        x /= c;
        y /= c;
        z /= c;
        return *this;
    }

    vec3<T>& operator/=(const vec3<T>& v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    vec3<T>& operator=(const vec3<T>& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    T length() const {
        return sqrt(sq(x) +
                    sq(y) +
                    sq(z));
    }

    //TODO: change in other vecs to insure that it realy changes vec
    void normalize() {
        T inv = T(1)/length();
        x *= inv;
        y *= inv;
        z *= inv;
    }

    vec2<T> xy() const { return vec2<T>(x,y); } //TODO: xz yz xxx zxy zyx ...
};

/// ================ VEC3 BOOL LIES HERE ================
typedef vec3<bool> bvec3;

inline bool any(const bvec3& bv1) {
    return (bv1.x ||
            bv1.y ||
            bv1.z);
}

inline bool all(const bvec3& bv1) {
    return (bv1.x &&
            bv1.y &&
            bv1.z);
}

inline bvec3 operator not(const bvec3& bv1) {
    return bvec3(!bv1.x,
                 !bv1.y,
                 !bv1.z);
}

template <typename T>
inline bool gt(const vec3<T>& v1, const vec3<T>& v2) {
    return  gt(v1.x,v2.x) &&
            gt(v1.y,v2.y) &&
            gt(v1.z,v2.z);
}

//template <typename T>
//inline bool gt(const vec3<T>& v1, T c) {
//    return gt(v1, vec3<T>(c));
//}

//template <typename T>
//inline bool gt(T c, const vec3<T>& v2) {
//    return gt(vec3<T>(c), v2);
//}

template <typename T>
inline bvec3 gtv(const vec3<T>& v1, const vec3<T>& v2) {
    return bvec3(v1.x > v2.x,
                 v1.y > v2.y,
                 v1.z > v2.z);
}

template <typename T>
inline bvec3 eqv(const vec3<T>& v1, const vec3<T>& v2) {
    return bvec3(eq(v1.x,v2.x),
                 eq(v1.y,v2.y),
                 eq(v1.z,v2.z));
}

//template <typename T>
//inline bvec3 neqv(const vec3<T>& v1, const vec3<T>& v2) {
//    return bvec3(!eq(v1.x,v2.x),
//                 !eq(v1.y,v2.y),
//                 !eq(v1.z,v2.z));
//}

template <typename T>
inline vec3<T> ifv(const bvec3& condv, const vec3<T>& v1, const vec3<T>& v2) {
    return vec3<T>(condv.x ? v1.x : v2.x,
                   condv.y ? v1.y : v2.y,
                   condv.z ? v1.z : v2.z);
}

/// =====================================================

template <typename T>
inline T length(const vec3<T>& v1) {
    return sqrt(sq(v1.x) +
                sq(v1.y) +
                sq(v1.z));
}

template <typename T>
inline vec3<T> normalize(const vec3<T>& v) {
    T inv = T(1)/v.length();
    return vec3<T>(v.x*inv,
                   v.y*inv,
                   v.z*inv);
}

template <typename T>
inline T dot(const vec3<T>& v1, const vec3<T>& v2) {
    return  v1.x*v2.x +
            v1.y*v2.y +
            v1.z*v2.z;
}

template <typename T>
inline vec3<T> cross(const vec3<T>& v1, const vec3<T>& v2) {
    return vec3<T>(	v1.y*v2.z - v1.z*v2.y,
                    v1.z*v2.x - v1.x*v2.z,
                    v1.x*v2.y - v1.y*v2.x);
}

//TODO: other vecs
template <typename T>
inline bool operator==(const vec3<T>& v1, const vec3<T>& v2) {
    return  eq(v1.x,v2.x) &&
            eq(v1.y, v2.y) &&
            eq(v1.z, v2.z);
}

template <typename T>
inline vec3<T> operator+(const vec3<T>& v1, const vec3<T>& v2) {
    return vec3<T>(v1.x+v2.x,
                   v1.y+v2.y,
                   v1.z+v2.z);
}

template <typename T>
inline vec3<T> operator-(const vec3<T>& v1, const vec3<T>& v2) {
    return vec3<T>(v1.x-v2.x,
                   v1.y-v2.y,
                   v1.z-v2.z);
}

template <typename T, typename U>
inline vec3<T> operator*(const U& c, const vec3<T> & v) {
    return vec3<T>(v.x*c,
                   v.y*c,
                   v.z*c);
}

template <typename T, typename U>
inline vec3<T> operator*(const vec3<T> & v, const U& c) {
    return vec3<T>(v.x*c,
                   v.y*c,
                   v.z*c);
}

template <typename T>
inline vec3<T> operator*(const vec3<T>& v1, const vec3<T>& v2) {
    return vec3<T>(v1.x*v2.x,
                   v1.y*v2.y,
                   v1.z*v2.z);
}

template <typename T>
inline vec3<T> operator/(const vec3<T>& v1, const vec3<T>& v2) {
    return vec3<T>(v1.x/v2.x,
                   v1.y/v2.y,
                   v1.z/v2.z);
}

template <typename T>
inline vec3<T> operator/(const vec3<T>& v1, const T& c) {
    return vec3<T>(v1.x/c,
                   v1.y/c,
                   v1.z/c);
}

//TODO: check in glsl, add to other vecs
template <typename T>
inline vec3<T> operator/(const T& c, const vec3<T>& v1) {
    return vec3<T>(c/v1.x,
                   c/v1.y,
                   c/v1.z);
}

template <typename T>
inline vec3<T> operator-(const vec3<T>& v) {
    return vec3<T>(-v.x,
                   -v.y,
                   -v.z);
}

template <typename T>
inline uint rgb2i(const vec3<T>& v) {
    return  ((uint(255) & 0xFF)<<24) +
            ((uint(v.z) & 0xFF)<<16) +
            ((uint(v.y) & 0xFF)<< 8) +
            ((uint(v.x) & 0xFF)    );
}

//TODO: add to other vecs
template <typename T>
inline vec3<T> clamp(const vec3<T>& v, T minv, T maxv) {
    return vec3<T>(clamp(v.x, minv, maxv),
                   clamp(v.y, minv, maxv),
                   clamp(v.z, minv, maxv));
}

template <typename T>
inline vec3<T> clamp_color(const vec3<T>& v) {
    return vec3<T>(clamp_color(v.x),
                   clamp_color(v.y),
                   clamp_color(v.z));
}

template <typename T>
inline vec3<T> max(const vec3<T>& v1, const vec3<T>& v2) {
    return vec3<T>(std::max(v1.x,v2.x),
                   std::max(v1.y,v2.y),
                   std::max(v1.z,v2.z));
}

template <typename T>
inline vec3<T> min(const vec3<T>& v1, const vec3<T>& v2) {
    return vec3<T>(std::min(v1.x,v2.x),
                   std::min(v1.y,v2.y),
                   std::min(v1.z,v2.z));
}

//TODO: add to other vecs
template <typename T>
inline vec3<T> abs(const vec3<T>& v1) {
    return vec3<T>(std::abs(v1.x),
                   std::abs(v1.y),
                   std::abs(v1.z));
}

template <typename T>
inline vec3<T> reflect(const vec3<T>& I, const vec3<T>& N) {
    return I - 2.f * dot(N,I) * N;
}

template <typename T>
inline vec3<T> sin(const vec3<T>& v1) {
    return vec3<T>(std::sin(v1.x),
                   std::sin(v1.y),
                   std::sin(v1.z));
}

template <typename T>
inline vec3<T> cos(const vec3<T>& v1) {
    return vec3<T>(std::cos(v1.x),
                   std::cos(v1.y),
                   std::cos(v1.z));
}

template<typename T>
inline std::ostream& operator <<(std::ostream& ostr, const vec3<T>& v) {
    return ostr << v.x << " "
                << v.y << " "
                << v.z;
}

typedef vec3<float> vec3f;
typedef vec3<int> vec3i;

inline vec3i mod(const vec3i& v1, const vec3i& v2) {
    return vec3i(mod(v1.x, v2.x),
                 mod(v1.y, v2.y),
                 mod(v1.z, v2.z));
}

inline vec3i mod(const vec3i& v1, int c) {
    return mod(v1, vec3i(c));
}

} // namespace bacchus
