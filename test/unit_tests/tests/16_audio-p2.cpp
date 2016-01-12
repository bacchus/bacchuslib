#include "setting.h"
#include "audio/audio_tools.h"
using namespace bacchus;

TEST(Audio_p2, Sine) {
    matxf x = genSine(1,10,1,50,0.1f);
    //PRINT(x);
    matxf resx = { 0.54030231, -0.63332387, -0.93171798,  0.05749049,  0.96724906};
    int n = x.size();
    EXPECT_EQ(resx.size(), n);
    for (int i = 0; i < n; ++i) {
        EXPECT_LE(ulp_difference(resx[i],x[i], 1.0e-6f), 10);
    }

    matxf x2 = genSine(1,10,1,50,0.2f);
    EXPECT_EQ(2*n, x2.size());
}

TEST(Audio_p2, CplxSine) {
    matxcx s = genComplexSine(1,5);
    //PRINT(s);
    matxcx ress = matxcx({cplx{1.00,-0.00}, cplx{0.31,-0.95}, cplx{-0.81,-0.59}, cplx{-0.81,0.59}, cplx{0.31,0.95}});
    //PRINT(ress);
    int n = s.size();
    EXPECT_EQ(ress.size(), n);
    //EXPECT_EQ(ress, s);
    for (int i = 0; i < n; ++i) {
        EXPECT_LE(ulp_difference(ress[i].real(),s[i].real(), 1.0e-2f), 1024);
        EXPECT_LE(ulp_difference(ress[i].imag(),s[i].imag(), 1.0e-2f), 1024);
    }
}

TEST(Audio_p2, Dft) {
    matxf x = {1,2,3,4};
    matxcx y = DFT(x);
    //PRINT(y);
    matxcx resy = {cplx{10,0}, cplx{-2,2}, cplx{-2,0}, cplx{-2,-2}};
    int n = y.size();
    EXPECT_EQ(resy.size(), n);
    //EXPECT_EQ(resy,y);
    for (int i = 0; i < n; ++i) {
        EXPECT_LE(ulp_difference(resy[i].real(),y[i].real(), 1.0e-6f), 1024);
        EXPECT_LE(ulp_difference(resy[i].imag(),y[i].imag(), 1.0e-6f), 1024);
    }
}

TEST(Audio_p2, InvDft) {
    matxcx x = {1,1,1,1};
    matxf y = IDFT(x);
    //PRINT(y);
    matxf resy = {1,0,0,0};
    int n = y.size();
    EXPECT_EQ(resy.size(), n);
    //EXPECT_EQ(resy,y);
    for (int i = 0; i < n; ++i) {
        EXPECT_LE(ulp_difference(resy[i],y[i], 1.0e-6f), 1024);
    }
}

TEST(Audio_p2, Magnitude) {
    matxf x = {1,2,3,4};
    matxf y = genMagSpec(x);
    //PRINT(y);
    matxf resy = {10, 2.82842712, 2, 2.82842712};
    int n = y.size();
    EXPECT_EQ(resy.size(), n);
    //EXPECT_EQ(resy,y);
    for (int i = 0; i < n; ++i) {
        EXPECT_LE(ulp_difference(resy[i],y[i], 1.0e-6f), 1024);
    }
}

TEST(Audio_p2, AnsSine) {
    matxf x = genSine(1,10,0,40,0.2f);
    matxcx y = DFT(x);
    //PRINT(y);

    matxcx y1 = {4,0,0,0};
    matxf x1 = IDFT(y1);
    //PRINT(x1);
}
