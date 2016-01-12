#pragma once

#include <iostream>

#include "mdefs.h"

namespace bacchus {

template <typename T>
struct vec2 {
    T x,y;

    vec2(): x(0), y(0) {}
    explicit vec2(const T& c): x(c), y(c) {}
    vec2(const T& x, const T& y): x(x), y(y) {}
    explicit vec2(const T* rawp): x(rawp[0]), y(rawp[1]) {}
    template<typename U> vec2(const vec2<U>& v) : x(v.x), y(v.y) {}

    T& operator[](int i) {
        return (&(this->x))[i];
    }

    const T& operator[](int i) const {
        return (&(this->x))[i];
    }

    vec2<T>& operator+=(const vec2<T>& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    vec2<T>& operator-=(const vec2<T>& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    vec2<T>& operator*=(const T& c) {
        x *= c;
        y *= c;
        return *this;
    }

    vec2<T>& operator/=(const T& c) {
        x /= c;
        y /= c;
        return *this;
    }

    vec2<T>& operator=(const vec2<T>& v) {
        x = v.x;
        y = v.y;
        return *this;
    }

    bool operator==(const vec2<T>& v) {
        return  eq(x, v.x) &&
                eq(y, v.y);
    }

    T length() const {
        return sqrt(x*x +
                    y*y);
    }

    vec2<T>& normalize() {
        T inv = T(1)/length();
        x *= inv;
        y *= inv;
        return *this;
    }
};

template <typename T>
inline T length(const vec2<T>& v1) {
    return sqrt(sq(v1.x) +
                sq(v1.y));
}

template <typename T>
inline vec2<T> normalize(const vec2<T>& v) {
    T inv = T(1)/v.length();
    return vec2<T>(v.x*inv,
                   v.y*inv);
}

template <typename T>
inline T dot(const vec2<T>& v1, const vec2<T>& v2) {
    return  v1.x*v2.x +
            v1.y*v2.y;
}

template <typename T>
inline T cross(const vec2<T>& v1, const vec2<T>& v2) {
    return  v1.x*v2.y -
            v1.y*v2.x;
}

template <typename T>
inline vec2<T> operator+(const vec2<T>& v1, const vec2<T>& v2) {
    return vec2<T>(v1.x+v2.x,
                   v1.y+v2.y);
}

template <typename T>
inline vec2<T> operator-(const vec2<T>& v1, const vec2<T>& v2) {
    return vec2<T>(v1.x-v2.x,
                   v1.y-v2.y);
}

template <typename T, typename U>
inline vec2<T> operator*(const U& c, const vec2<T> & v) {
    return vec2<T>(v.x*c,
                   v.y*c);
}

template <typename T, typename U>
inline vec2<T> operator*(const vec2<T> & v, const U& c) {
    return vec2<T>(v.x*c,
                   v.y*c);
}

template <typename T>
inline vec2<T> operator*(const vec2<T>& v1, const vec2<T>& v2) {
    return vec2<T>(v1.x*v2.x,
                   v1.y*v2.y);
}

template <typename T>
inline vec2<T> operator/(const vec2<T>& v, const T& c) {
    return vec2<T>(v.x/c,
                   v.y/c);
}

template <typename T>
inline vec2<T> operator/(const vec2<T>& v1, const vec2<T>& v2) {
    return vec2<T>(v1.x/v2.x,
                   v1.y/v2.y);
}

template <typename T>
inline vec2<T> operator-(const vec2<T>& v) {
    return vec2<T>(-v.x,
                   -v.y);
}

template <typename T>
inline vec2<T> abs(const vec2<T>& v1) {
    return vec2<T>(std::abs(v1.x),
                   std::abs(v1.y));
}

template <typename T>
inline vec2<T> max(const vec2<T>& v1, const vec2<T>& v2) {
    return vec2<T>(std::max(v1.x,v2.x),
                   std::max(v1.y,v2.y));
}

template <typename T>
inline vec2<T> min(const vec2<T>& v1, const vec2<T>& v2) {
    return vec2<T>(std::min(v1.x,v2.x),
                   std::min(v1.y,v2.y));
}

template <typename T>
inline vec2<T> floor(const vec2<T>& v1) {
    return vec2<T>(std::floor(v1.x),
                   std::floor(v1.y));
}

template <typename T>
inline vec2<T> frac(const vec2<T>& v1) {
    return v1 - floor(v1);
}

template<typename T>
inline std::ostream& operator <<(std::ostream& ostr, const vec2<T>& v) {
    return ostr << v.x << " "
                << v.y;
}

typedef vec2<float> vec2f;
typedef vec2<int> vec2i;

inline vec2i mod(const vec2i& v1, const vec2i& v2) {
    return vec2i(mod(v1.x, v2.x),
                 mod(v1.y, v2.y));
}

inline vec2i mod(const vec2i& v1, int c) {
    return mod(v1, vec2i(c));
}

} // namespace bacchus
