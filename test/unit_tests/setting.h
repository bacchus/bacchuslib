#pragma once
#include "gtest/gtest.h"

#include "utils/logger.h"
#include "math/matmxn.h"
using bacchus::operator <<;

namespace bacchus {

inline int ulp_diff(const float& a, const float& b, float eps) {
    if (a*b>0.f && std::abs(a - b)/std::abs(a + b) < eps)
        return -1;
//    if (a*b<0.f && std::abs(a + b)/std::abs(a - b) < eps)
//        return -2;
    int aint = *(int*)&a;
    if (aint < 0) aint = 0x80000000 - aint;
    int bint = *(int*)&b;
    if (bint < 0) bint = 0x80000000 - bint;
    int diff = std::abs(aint - bint);
    return diff;
}

inline void compare(const matxf& resx, const matxf& x, float eps=1.0e-2f, int ulp=1024*16) {
    int nx = x.size();
    EXPECT_EQ(resx.size(), nx);
    for (int i = 0; i < nx; ++i) {
        int diff = ulp_diff(resx[i],x[i], eps);
        if (diff > ulp) {
            std::cout << i << ": " << resx[i] << " <> " << x[i] << std::endl;
        }
        EXPECT_LE(diff, ulp);
    }
}

inline void compare(const matxcx& resx, const matxcx& x, float eps=1.0e-4f, int ulp=1024*16) {
    int nx = x.size();
    EXPECT_EQ(resx.size(), nx);
    for (int i = 0; i < nx; ++i) {
        int diffre = ulp_diff(resx[i].real(),x[i].real(), eps);
        if (diffre > ulp) {
            std::cout << i << " re: " << resx[i].real() << " <> " << x[i].real() << std::endl;
        }
        EXPECT_LE(diffre, ulp);

        int diffim = ulp_diff(resx[i].imag(),x[i].imag(), eps);
        if (diffim > ulp) {
            std::cout << i << " im: " << resx[i].imag() << " <> " << x[i].imag() << std::endl;
        }
        EXPECT_LE(diffim, ulp);
    }
}

} // namespace bacchus
