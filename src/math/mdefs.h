#pragma once

#include <cstdlib>
#include <ctime>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <complex>

//#define M_PI 3.1415926535897932384626433832795f see in cmath
#define BCC_M_FLOAT_EPSILON 1.0e-4f
#define BCC_M_FLOAT_MIN    -1.0e+7f
#define BCC_M_FLOAT_MAX     1.0e+7f
#define BCC_M_INT_MAX       10000000
#define BCC_M_INT_MIN      -10000000

#define BCC_M_DOUBLE_EPSILON 1.0e-4
#define BCC_M_DOUBLE_MIN    -1.0e+7
#define BCC_M_DOUBLE_MAX     1.0e+7

#define BCC_IMG_RED 0x000000FF
#define BCC_IMG_GRN 0x0000FF00
#define BCC_IMG_BLU 0x00FF0000
#define BCC_IMG_ALF 0xFF000000

namespace bacchus {

typedef unsigned char uchar;// 1B = 8b
typedef unsigned short ushort;// 2B = 16b
typedef unsigned int uint;  // 4B = 32b
typedef unsigned long ulong;// 8B = 64b

typedef std::complex<float> cplx;
typedef std::complex<double> cplxd;

inline bool is_pow2(uint n) {
    return (n&(n-1))==0;
}

inline float deg2rad(float deg) {
    return M_PI*deg/180.0f;
}

inline float rad2deg(float rad) {
    return rad*180.0f/M_PI;
}

inline float rnd(float max=1.0f) {
    return rand()*max/RAND_MAX;
}

inline void rndmz() {
    srand(time(nullptr));
}

template<typename T>
inline bool eq(const T&, const T&) {
    assert(false);
}

template<>
inline bool eq<int>(const int& a, const int& b) {
    return a == b;
}

template<>
inline bool eq<float>(const float& a, const float& b) {
    if (std::abs(a - b) < BCC_M_FLOAT_EPSILON)
        return true;
    const int maxUlps = 1024;
    assert(maxUlps > 0 && maxUlps < 4*1024*1024);
    int aint = *(int*)&a;
    if (aint < 0) aint = 0x80000000 - aint;
    int bint = *(int*)&b;
    if (bint < 0) bint = 0x80000000 - bint;
    int diff = std::abs(aint - bint);
    return (diff <= maxUlps);
}

template<>
inline bool eq<double>(const double& a, const double& b) {
    return std::abs(a - b) < BCC_M_DOUBLE_EPSILON;
}

template<>
inline bool eq<cplx>(const cplx& a, const cplx& b) {
    return     eq(a.real(), b.real())
            && eq(a.imag(), b.imag());
}

template<>
inline bool eq<cplxd>(const cplxd& a, const cplxd& b) {
    return     eq(a.real(), b.real())
            && eq(a.imag(), b.imag());
}

inline int ulp_difference(const float& a, const float& b, float epsilon) {
    if (std::abs(a - b) < epsilon)
        return -1;

//    if (a*b>0.f && std::abs(a - b)/std::abs(a + b) < 0.01)
//        return -2;

    int aint = *(int*)&a;
    if (aint < 0) aint = 0x80000000 - aint;
    int bint = *(int*)&b;
    if (bint < 0) bint = 0x80000000 - bint;
    int diff = std::abs(aint - bint);
    return diff;
}

inline int ulp_difference(const double& a, const double& b, double epsilon) {
    if (std::abs(a - b) < epsilon)
        return -1;

//    if (a*b>0.f && std::abs(a - b)/std::abs(a + b) < 0.01)
//        return -2;

    int aint = *(int*)&a;
    if (aint < 0) aint = 0x80000000 - aint;
    int bint = *(int*)&b;
    if (bint < 0) bint = 0x80000000 - bint;
    int diff = std::abs(aint - bint);
    return diff;
}

inline bool gt(float a, float b) {
    return a > (b - BCC_M_FLOAT_EPSILON);
}

inline bool le(float a, float b) {
    return a < (b + BCC_M_FLOAT_EPSILON);
}

inline int div(int a, int b) {
    int d = a % b;
    if (d < 0)
        d += b;
    return d;
}

inline int mod(int a, int b) {
    if (b==0) return a;
    return a % b;
}

template<typename T>
inline int sgn(T x) {
    T zero(0);
    if (x < zero)
        return -1;
    else if (x > zero)
        return 1;
    else
        return 0;
}

template <typename T>
inline T min3(T a, T b, T c) {
    return std::min(a, std::min(b,c));
    //return (a<(b<c?b:c))?a:(b<c?b:c);
}

template <typename T>
inline int min3i(T a, T b, T c) {
    return ( a < (b<c?b:c) ) ? 0 : ( b<c?1:2 );
}

template <typename T>
inline T max3(T a, T b, T c) {
    return std::max(a, std::max(b,c));
}

template <typename T>
inline int max3i(T a, T b, T c) {
    return ( a > (b>c?b:c) ) ? 0 : ( b>c?1:2 );
}

inline float clamp(float c, float minval, float maxval) {
    return std::max(std::min(c, maxval), minval);
}

inline int clamp(int c, int minval, int maxval) {
    return std::max(std::min(c, maxval), minval);
}

template<typename T>
inline T mix(const T& x, const T& y, float a) {
    return (1.f - a)*x + a*y;
}

template<typename T>
inline T smoothstep(const T& x, const T& y, float a) {
    T t = clamp((T(a) - x)/(y - x), 0.f, 1.f);
    return 3.f*t*t - 2.f*t*t*t;
}

inline float fpown(float a, int n) {
    assert(n>=0);
    float res = 1.0f;
    for (int i = 0; i < n; ++i) {
        res *= a;
    }
    return res;
}

inline int pown(int a, int n) {
    assert(n>=0);
    int res = 1;
    for (int i = 0; i < n; ++i) {
        res *= a;
    }
    return res;
}


template<typename T>
inline T sq(const T& a) {
    return a*a;
}

// gcd((n-1)!, n) = 1; then n - prime
inline int gcd(int a, int b) {
    assert(a > 0);
    assert(b > 0);
    int ax = std::max(a,b);
    int in = std::min(a,b);
    while (int k = (ax%in)) {
        ax = in;
        in = k;
    }
    return in;
}

inline int lcm(int a, int b) {
    return (a*b)/gcd(a,b);
}

inline uchar clamp_color(int c) {
    return clamp(c, 0, 255);
}

inline uint rgba2i(uchar r, uchar g, uchar b, uchar a=255) {
    return (a<<24)|(b<<16)|(g<<8)|(r);
}

inline uint lum2i(uchar g) {
    return rgba2i(g,g,g,255);
}

inline void i2rgba(uint i, uchar& r, uchar& g, uchar& b, uchar& a) {
    r = (i    ) & 0xFF;//a = (i>>24) & 0xFF;
    g = (i>>=8) & 0xFF;//b = (i>>16) & 0xFF;
    b = (i>>=8) & 0xFF;//g = (i>>8 ) & 0xFF;
    a = (i>>=8) & 0xFF;//r = (i    ) & 0xFF;
}

inline uchar luminance(uchar r, uchar g, uchar b) {
    float f = 0.299f*r + 0.587f*g + 0.114f*b;
    return (uchar)f;
}

inline uint luminance(uint rgba) {
    uchar r,g,b,a;
    i2rgba(rgba, r,g,b,a);
    float f = 0.299f*r + 0.587f*g + 0.114f*b;
    uchar l = (uchar)f;
    uint llla = rgba2i(l,l,l,a);
    return llla;
}

/*
0 1 2    6 3 0
3 4 5 -> 7 4 1
6 7 8    8 5 2
*/
inline void elem9_rot90(uchar* k) {
    uchar t;
    t=k[0];k[0]=k[6];k[6]=k[8];k[8]=k[2];k[2]=t;
    t=k[1];k[1]=k[3];k[3]=k[7];k[7]=k[5];k[5]=t;
}

/*
0 1 2    3 0 1
3 4 5 -> 6 4 2
6 7 8    7 8 5
*/
inline void kern9_rot45(const float* k, float* r) {
    int rot[9] = {3,0,1,6,4,2,7,8,5};
    for (int i = 0; i < 9; ++i) {
        r[i]=k[rot[i]];
    }
}

inline uchar median_slow(uchar* a, int n) {
    int k = n/2;
    for (int i = 0; i < k+1; ++i) {
        int min_i = i;
        uchar min_v = a[i];
        for (int j = i+1; j < n; ++j) {
            if (a[j] < min_v) {
                min_i = j;
                min_v = a[j];
            }
        }
        a[min_i] = a[i];
        a[i] = min_v;
    }
    return a[k];
}

template<typename T>
inline int arr_maxi(const T* a, int n) {
    T max_v = a[0];
    int max_i = 0;
    for (int i = 1; i < n; ++i) {
        if (a[i] > max_v) {
            max_v = a[i];
            max_i = i;
        }
    }
    return max_i;
}

template<typename T>
inline int arr_mini(const T* a, int n) {
    T min_v = a[0];
    int min_i = 0;
    for (int i = 1; i < n; ++i) {
        if (a[i] < min_v) {
            min_v = a[i];
            min_i = i;
        }
    }
    return min_i;
}

inline uchar find_max_clamp(float* a, int n) {
    float max_v = clamp_color(a[0]);
    for (int i = 1; i < n; ++i) {
        if (a[i] > max_v) {
            max_v = clamp_color(a[i]);
        }
    }
    return max_v;
}

inline float func_gauss(int x, float s) {
    return exp(-0.5f*x*x/(s*s));
}

inline float func_gauss(int x, int y, float s) {
    return exp(-0.5f*(x*x+y*y)/(s*s));
}

inline float func_laplac_of_gauss(int x, int y, float s) {
    return (0.5f*(x*x+y*y)/(s*s) - 1.0f)*exp(-0.5f*(x*x+y*y)/(s*s));
}

inline bool is_prime(int n) {
    int til = sqrt(n) + 1;
    for (int i = 2; i < til; ++i)
        if (n%i==0)
            return false;
    return true;
}

} // namespace bacchus
