#include "setting.h"
#include "math/fft.h"
#include "utils/logger.h"

using namespace bacchus;

//TEST(FFT_test, SlowGorner) {
//    matxcx x = {1,2,3,4};
//    matxcx y = fft_slow_gorn(x, false);
//    PRINT(y);
//    matxcx resy = {{10,0}, {-2,2}, {-2,0}, {-2,-2}};
//    int n = y.size();
//    EXPECT_EQ(resy.size(), n);

//    //EXPECT_EQ(1,0);
//    for (int i = 0; i < n; ++i) {
//        EXPECT_LE(ulp_difference(resy[i].real(),y[i].real(), 1.0e-6f), 1024);
//        EXPECT_LE(ulp_difference(resy[i].imag(),y[i].imag(), 1.0e-6f), 1024);
//    }
//}

TEST(FFT_test, Bluestein) {
    matxcx x = {1,2,3,4};
    matxcx y = fft(x);
    PRINT(y);
    matxcx resy({cplx(10,0), cplx(-2,2), cplx(-2,0), cplx(-2,-2)});
    int n = y.size();
    EXPECT_EQ(resy.size(), n);

    //EXPECT_EQ(1,0);

    for (int i = 0; i < n; ++i) {
        EXPECT_LE(ulp_difference(resy[i].real(),y[i].real(), 1.0e-6f), 1024);
        EXPECT_LE(ulp_difference(resy[i].imag(),y[i].imag(), 1.0e-6f), 1024);
    }
}

TEST(FFT_test, BluesteinByRev) {
    //const int n = 7;
    //float fa1[] = {-5,0,4,-2,0,0,0,0};
    //float fa1[] = {0,1,2,3,4,5,6,7};
    //float fa1[] = {-5,0,4,-2,-5,0,4,-2};
    //float fa1[] = {-5,0,4,-2,5,0,-4,2};
    //float fa1[] = {-5,0,4,-2,-2,4,0,-5};

    matxcx a = {0,1,2,3,4,5,6,7};
    matxcx y = fft(a);
    matxcx ra = fftrev(y);

    int n = ra.size();
    EXPECT_EQ(a.size(), n);
    for (int i = 0; i < n; ++i) {
        EXPECT_LE(ulp_difference(a[i].real(), ra[i].real(), 1.0e-6f), 10);
        EXPECT_LE(ulp_difference(a[i].imag(), ra[i].imag(), 1.0e-6f), 10);
    }

    //std::cout << "a   : " << a << std::endl;
    //std::cout << "bluestein: " << y4 << std::endl;
    //std::cout << "back: " << a4 << std::endl;

//        matxcx y1 = recursive_fft(a, false);
//        print_vec(y1, "fft8");
//        std::cout<<std::endl;
//        std::cout<<std::endl;

//        matxcx y1 = fft(a);
//        print_vec(y1, "fft8");

//        matxcx y2 = dft_slow(a);
//        print_vec(y2, "dft ");

//        matxcx y3 = iterative_fft(a, false);
//        print_vec(y3, "iter");

//        matxcx a1 = iterative_fft(y1, true);
//        print_vec(a1, "ifft");

//        matxcx a2 = dft_slow(y2, true);
//        print_vec(a2, "rdft");

//        matxcx a1 = fftrev(y1);
//        print_vec(a1, "rfft");
}

TEST(FFT_test, PolynomMultiplication) {
    //const int n = 8;
    matxcx fres = {-45,86,-75,-20,44,-14,-12,0};

    matxcx a1 = {9,-10,7,6,0,0,0,0};
    matxcx a2 = {-5,4,0,-2,0,0,0,0};

    //std::cout << "check: " << fres << std::endl;

    if (1) {
        matxcx a12 = convolve(a1,a2);

        //EXPECT_TRUE(std::equal(fres.begin(), fres.end(), a12.begin()));
        PRINT(fres);
        PRINT(a12);
        //EXPECT_EQ(0,1);

        int n = a12.size();
        EXPECT_EQ(fres.size(), n);
        for (int i = 0; i < n; ++i) {
            EXPECT_LE(ulp_difference(fres[i].real(), a12[i].real(), 1.0e-4f), 10);
            EXPECT_LE(ulp_difference(fres[i].imag(), a12[i].imag(), 1.0e-4f), 10);
        }


    }

    if (1) {
        matxcx y1 = fft(a1);
        matxcx y2 = fft(a2);

        matxcx y1xy2 = mul(y1,y2);
//        for (int i = 0; i < n; ++i) {
//            y1xy2.push_back(y1[i]*y2[i]);
//        }

        matxcx res = fftrev(y1xy2);
        //EXPECT_TRUE(std::equal(fres.begin(), fres.end(), res.begin()));
        //std::cout << "byfft: " << res << std::endl;
        PRINT(fres);
        PRINT(res);
        //EXPECT_EQ(0,1);

        int n = res.size();
        EXPECT_EQ(fres.size(), n);
        for (int i = 0; i < n; ++i) {
            EXPECT_LE(ulp_difference(fres[i].real(), res[i].real(), 1.0e-4f), 10);
            EXPECT_LE(ulp_difference(fres[i].imag(), res[i].imag(), 1.0e-4f), 10);
        }
    }

//    if (1) {
//        matxcx y1 = fft_slow_gorn(a1, false);
//        matxcx y2 = fft_slow_gorn(a2, false);

//        matxcx y1xy2;
//        for (int i = 0; i < n; ++i) {
//            y1xy2.push_back(y1[i]*y2[i]);
//        }

//        matxcx res = fft_slow_gorn(y1xy2, true);
//        float div = 1.0f/(float)res.size();
//        for (uint i = 0; i < res.size(); ++i) {
//            res[i] *= div;
//        }

//        PRINT(fres);
//        PRINT(res);
//        //EXPECT_EQ(0,1);

//        int n = res.size();
//        EXPECT_EQ(fres.size(), n);
//        for (int i = 0; i < n; ++i) {
//            EXPECT_LE(ulp_difference(fres[i].real(), res[i].real(), 1.0e-4f), 10);
//            EXPECT_LE(ulp_difference(fres[i].imag(), res[i].imag(), 1.0e-4f), 10);
//        }
//    }

}

TEST(FFT_test, Sinus) {
    const int n = 16;

    matxcx a(n);
    for (int i = 0; i < n; ++i) {
        a[i] = sin(2*M_PI*i/(float)n);
    }

    matxcx y = fft(a);
    std::vector<float> mag(n);
    std::vector<float> phi(n);
    for (int i = 0; i < n; ++i) {
        mag[i] = (eq(std::abs(y[i]), 0.0f) ? 0.0f : std::abs(y[i]));
        phi[i] = (eq(std::arg(y[i]), 0.0f) ? 0.0f : std::arg(y[i]));
    }

    matxcx b = fftrev(y);
    //EXPECT_TRUE(std::equal(b.begin(), b.end(), a.begin()));

    PRINT(a);
    PRINT(b);
    //EXPECT_EQ(0,1);
    //int n = b.size();
    EXPECT_EQ(a.size(), n);
    for (int i = 0; i < n; ++i) {
        EXPECT_LE(ulp_difference(a[i].real(), b[i].real(), 1.0e-4f), 10);
        EXPECT_LE(ulp_difference(a[i].imag(), b[i].imag(), 1.0e-4f), 10);
    }


    //std::cout << "a: " << a << std::endl;
    //std::cout << "y: " << y << std::endl;
    //std::cout << "mag: " << mag << std::endl;
    //std::cout << "phi: " << phi << std::endl;
    //std::cout << "b: " << b << std::endl;
}

//TEST(FFT_test, RevBits) {
//    for (uint i = 0; i < 8; ++i) {
//        std::cout<<rev_bits(i,3)<<" ";
//    }
//    std::cout<<std::endl;
//}
