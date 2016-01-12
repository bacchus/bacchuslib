#include "setting.h"
#include "math/fft.h"
#include "utils/logger.h"
#include "audio/audio_tools.h"
using namespace bacchus;

const float tol18 = 1e-4f;
const int ulpDif = 256*1024;

void FftCrossTest(const matxf& x, const matxcx& resy) {
    matxcx y = DFT(x);
    int n = y.size();
    EXPECT_EQ(resy.size(), n);
    for (int i = 0; i < n; ++i) {
        EXPECT_LE(ulp_difference(resy[i].real(),y[i].real(), tol18), i);
        EXPECT_LE(ulp_difference(resy[i].imag(),y[i].imag(), tol18), i);
    }

    matxcx y1 = fft(x);
    int n1 = y1.size();
    EXPECT_EQ(resy.size(), n1);
    for (int i = 0; i < n1; ++i) {
        EXPECT_LE(ulp_difference(resy[i].real(),y1[i].real(), tol18), i);
        EXPECT_LE(ulp_difference(resy[i].imag(),y1[i].imag(), tol18), i);
    }
}

void FftCrossTest(const matxf& x) {
    matxcx resy = DFT(x);
    matxcx y1 = fft(x);

    //PRINT(resy);
    //PRINT(y1);

//    int bd = x.size() > 1000 ? 948 : 0;
//    EXPECT_FLOAT_EQ(resy[bd].real(),y1[bd].real());

    int n1 = y1.size();
    EXPECT_EQ(resy.size(), n1);
    for (int i = 0; i < n1; ++i) {
        EXPECT_LE(ulp_difference(resy[i].real(),y1[i].real(), tol18), i);
        EXPECT_LE(ulp_difference(resy[i].imag(),y1[i].imag(), tol18), i);
    }
}

TEST(FFT2, CrossTest) {
    matxf x = {1,2,3,4};
    matxcx resy = {cplx{10,0}, cplx{-2,2}, cplx{-2,0}, cplx{-2,-2}};
    FftCrossTest(x,resy);
}

TEST(FFT2, DftNoRes) {
    matxf x = {1,2,3,4};
    FftCrossTest(x);
}

TEST(FFT2, Sine) {
    int fs = 1024*100;
    int f1 = 100;
    matxcx x1 = genSine(1,f1,0,fs,1024);

    matxcx y = fft(x1);
    //PRINT(y);
    matxcx res = fftrev(y);

    int n1 = x1.size();
    EXPECT_EQ(res.size(), n1);
    for (int i = 0; i < n1; ++i) {
        EXPECT_LE(ulp_difference(res[i].real(),x1[i].real(), tol18), i);
        EXPECT_LE(ulp_difference(res[i].imag(),x1[i].imag(), tol18), i);
    }

    //FftCrossTest(x1);
    //FftCrossTest(x2);
}
