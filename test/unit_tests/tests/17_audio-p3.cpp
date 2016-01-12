#include "setting.h"
#include "audio/audio_tools.h"
#include "math/fft.h"
using namespace bacchus;

TEST(Audio_p3, MinEnergy) {
    int fs = 10000;
    int f1 = 80;
    int f2 = 200;
    // sum of two sinusoids
    matxf x1 = genSine(1,f1,0,fs,1024);
    matxf x2 = genSine(1,f2,0,fs,1024);
    matxf x3 = x1 + x2;
    //matxf x3(x1.size());
    //std::transform(x1.begin(), x1.end(), x2.begin(), x3.begin(), std::plus<float>());
    // compute min energy
    matxf mag = minimizeEnergySpreadDFT(x3,fs,f1,f2);

    // check sum sinusoids
    x3.resize(100);
    //PRINT(x3);
    matxf res_x3 = {
            2.        ,  1.99085166,  1.96353418,  1.91842823,  1.85616173,
            1.77760016,  1.68383317,  1.57615785,  1.45605864,  1.32518454,
            1.18532367,  1.0383758 ,  0.88632312,  0.73119988,  0.5750612 ,
            0.41995163,  0.26787401,  0.12075896, -0.01956438, -0.15139592,
           -0.2731902 , -0.38357934, -0.48139327, -0.56567673, -0.63570282,
           -0.69098301, -0.7312732 , -0.75657605, -0.76713932, -0.7634503 ,
           -0.74622647, -0.71640259, -0.67511417, -0.62367799, -0.56356958,
           -0.49639831, -0.42388031, -0.34780978, -0.27002902, -0.19239778,
           -0.1167623 , -0.04492464,  0.02138726,  0.08054837,  0.13106364,
            0.171593  ,  0.20097387,  0.21824081,  0.22264202,  0.2136524 ,
            0.19098301,  0.15458666,  0.10465974,  0.04164004, -0.03379929,
           -0.12075949, -0.21812968, -0.32460368, -0.43870008, -0.55878504,
           -0.68309771, -0.80977759, -0.93689367, -1.06247471, -1.18454021,
           -1.3011317 , -1.41034363, -1.51035367, -1.59945166, -1.67606693,
           -1.73879348, -1.78641265, -1.81791293, -1.83250658, -1.82964274,
           -1.80901699, -1.770577  , -1.71452431, -1.64131216, -1.55163949,
           -1.44644098, -1.32687361, -1.19429961, -1.05026633, -0.89648322,
           -0.73479629, -0.56716041, -0.39561006, -0.22222874, -0.04911768,
            0.12163568,  0.287989  ,  0.4479756 ,  0.59973381,  0.74153467,
            0.87180751,  0.98916306,  1.09241365,  1.18059027,  1.25295621};

    for (int i = 0; i < 100; ++i) {
        EXPECT_LE(ulp_difference(res_x3[i],x3[i], 1.0e-6f), 10);
    }
    // check res
//    PRINT(mag);
    EXPECT_EQ(126, mag.size());
    //EXPECT_EQ(1,0);
    for (int i = 0; i < 126; ++i) {
        if (i==2 || i==5) {
            EXPECT_GT(mag[i], 1);
        } else {
            EXPECT_LE(mag[i], 1);
        }
    }
}

TEST(Audio_p3, OptZeropad1) {
    int fs = 1000;
    int f1 = 100;
    int ns = 25;

    matxf x = genSine(1,f1,0,fs,ns);
    matxf mag = optimalZeropad(x,fs,f1);

    EXPECT_EQ(16, mag.size());
    auto me = std::max_element(mag.begin(), mag.end());
    EXPECT_EQ(*me, mag[3]);
}

TEST(Audio_p3, OptZeropad2) {
    int fs = 10000;
    int f1 = 250;
    int ns = 210;

    matxf x = genSine(1,f1,0,fs,ns);
    matxf mag = optimalZeropad(x,fs,f1);

    EXPECT_EQ(121, mag.size());
    auto me = std::max_element(mag.begin(), mag.end());
    EXPECT_EQ(*me, mag[6]);
}

TEST(Audio_p3, TestRealEven1) {
//    Test case 1: If x = np.array([ 2, 3, 4, 3, 2 ]), which is a real and even signal (after zero phase
//    windowing), the function returns (True, array([ 4., 3., 2., 2., 3.]), array([14.0000+0.j, 2.6180+0.j,
//    0.3820+0.j, 0.3820+0.j, 2.6180+0.j])) (values are approximate)
    matxf x = {2, 3, 4, 3, 2};
    matxf res_buf = {4., 3., 2., 2., 3};
    matxcx res_y = {
        14.0000, 2.6180, 0.3820, 0.3820, 2.6180
    };

    matxf buf;
    matxcx y;
    EXPECT_TRUE(testRealEven(x,buf,y));

    int ny = y.size();
    EXPECT_EQ(res_y.size(), ny);
    for (int i = 0; i < ny; ++i) {
        EXPECT_LE(ulp_difference(res_y[i].real(),y[i].real(), 1.0e-4f), 1024);
        EXPECT_LE(ulp_difference(res_y[i].imag(),y[i].imag(), 1.0e-4f), 1024);
    }

    int nb = buf.size();
    EXPECT_EQ(res_buf.size(), nb);
    for (int i = 0; i < nb; ++i) {
        EXPECT_LE(ulp_difference(res_buf[i],buf[i], 1.0e-4f), 1024);
    }
}

TEST(Audio_p3, TestRealEven2) {
//    Test case 2: If x = np.array([1, 2, 3, 4, 1, 2, 3]), which is not a even signal (after zero phase
//    windowing), the function returns (False,  array([ 4.,  1.,  2.,  3.,  1.,  2.,  3.]), array([ 16.+0.j,
//    2.+0.69j, 2.+3.51j, 2.-1.08j, 2.+1.08j, 2.-3.51j, 2.-0.69j])) (values are approximate)
    matxf x = {1, 2, 3, 4, 1, 2, 3};
    matxf res_buf = {4.,  1.,  2.,  3.,  1.,  2.,  3.};
    matxcx res_y = {
        16., {2.,0.69}, {2.,3.51}, {2.,-1.08}, {2.,1.08}, {2.,-3.51}, {2.,-0.69}
    };

    matxf buf;
    matxcx y;
    EXPECT_FALSE(testRealEven(x,buf,y));
    //PRINT(res_y);
    //PRINT(y);

    int ny = y.size();
    EXPECT_EQ(res_y.size(), ny);
    for (int i = 0; i < ny; ++i) {
        EXPECT_LE(ulp_difference(res_y[i].real(),y[i].real(), 1.0e-2f), 1024);
        EXPECT_LE(ulp_difference(res_y[i].imag(),y[i].imag(), 1.0e-2f), 1024);
    }

    int nb = buf.size();
    EXPECT_EQ(res_buf.size(), nb);
    for (int i = 0; i < nb; ++i) {
        EXPECT_LE(ulp_difference(res_buf[i],buf[i], 1.0e-4f), 1024);
    }
}

TEST(Audio_p3, MyDFTmodel) {
    int fs = 1000;
    int f1 = 100;
    int ns = 128;
    matxf x1 = genSine(1,f1,0,fs,ns);

    matxf mx,px;
    matxf w = hamming(ns);
    //PRINT(w);
    dftAnal(x1,w,ns,mx,px);

    matxf res_mx = {
        -58.1863712 , -58.10303349, -57.85520663, -57.44930914,
        -56.89598603, -56.21168783, -55.42555592, -54.60321249,
        -53.93555516, -54.19297627, -63.33433075, -38.24529563,
        -10.63753817,  -6.2936241 , -17.12763953, -50.6077656 ,
        -54.7330853 , -52.54791452, -52.85083564, -53.57725377,
        -54.37182101, -55.14190798, -55.86181189, -56.52668531,
        -57.13865264, -57.70202334, -58.22152839, -58.70166929,
        -59.14650235, -59.55959685, -59.94406114, -60.3025918 ,
        -60.63752737, -60.95089886, -61.24447463, -61.51979896,
        -61.77822483, -62.02094148, -62.24899764, -62.46332089,
        -62.66473409, -62.85396915, -63.03167863, -63.1984456 ,
        -63.35479206, -63.50118602, -63.63804764, -63.76575444,
        -63.88464577, -63.99502674, -64.0971715 , -64.19132615,
        -64.27771124, -64.35652392, -64.42793981, -64.49211462,
        -64.54918556, -64.59927249, -64.64247899, -64.67889319,
        -64.70858851, -64.73162425, -64.74804605, -64.75788625,
        -64.76116418
    };

    matxf res_px = {
        3.14159265,    6.20869323,    9.27774415,   12.35050591,
        15.42841991,   18.51258288,   21.60386052,   24.70324739,
        27.81305957,   30.94375216,   28.0616614 ,   27.62569158,
        27.64677833,   27.64559991,   27.64730729,   24.5585936 ,
        24.42766327,   21.31080748,   18.17486444,   15.03696913,
        11.89936982,    8.76262174,    5.62686851,    2.49211905,
        -0.6416678 ,   -3.77455179,   -6.90659809,  -10.03787147,
        -13.16843372,  -16.29834247,  -19.42765094,  -22.55640795,
        -25.6846581 ,  -28.81244212,  -31.93979714,  -35.06675707,
        -38.19335286,  -41.31961284,  -44.44556294,  -47.57122697,
        -50.69662679,  -53.82178256,  -56.94671287,  -60.07143495,
        -63.19596473,  -66.32031705,  -69.44450572,  -72.56854365,
        -75.69244292,  -78.8162149 ,  -81.93987025,  -85.0634191 ,
        -88.18687099,  -91.31023503,  -94.4335199 ,  -97.55673387,
        -100.67988494, -103.80298077, -106.92602879, -110.04903622,
        -113.17201009, -116.29495729, -119.41788459, -122.54079866,
        -125.66370614
    };

    int nmx = mx.size();
    EXPECT_EQ(res_mx.size(), nmx);
    for (int i = 0; i < nmx; ++i) {
        EXPECT_LE(ulp_difference(res_mx[i],mx[i], 1.0e-2f), 1024);
    }

    int npx = px.size();
    EXPECT_EQ(res_px.size(), npx);
    for (int i = 0; i < npx; ++i) {
        EXPECT_LE(ulp_difference(res_px[i],px[i], 1.0e-2f), 1024);
    }
}

TEST(Audio_p3, MyDFTmodel2) {
    int fs = 1000;
    int f1 = 100;
    int ns = 100;
    matxf x1 = genSine(1,f1,0,fs,ns);

    matxf mx,px;
    matxf w = hamming(ns);
    dftAnal(x1,w,128,mx,px);

    matxf res_mx = {
        -313.07119549,  -60.34442737,  -56.01271604,  -56.1233137 ,
        -62.40111344,  -63.76649348,  -53.64725013,  -51.37820502,
        -54.53875812,  -75.51880692,  -48.28934419,  -22.00264589,
        -8.80748602,   -6.18076691,  -12.46576001,  -34.00200244,
        -56.57847473,  -50.13650937,  -60.86673645,  -53.33326345,
        -47.96324078,  -48.22551215,  -53.52674371,  -69.15452189,
        -52.51998941,  -50.04795141,  -52.07300048,  -61.646589  ,
        -59.65079738,  -52.77710104,  -52.20802779,  -56.32379637,
        -312.531429  ,  -56.92654373,  -53.41650831,  -54.59731481,
        -62.09180609,  -64.72055862,  -55.79518901,  -54.43653828,
        -57.5955298 ,  -74.93855689,  -60.03849894,  -55.47408089,
        -55.92824452,  -61.91227603,  -69.7058003 ,  -57.86869835,
        -55.74597961,  -58.0877992 ,  -70.14451511,  -62.57163617,
        -56.80980997,  -56.57995614,  -61.37135457,  -76.50165743,
        -59.39276836,  -56.4724787 ,  -58.07511635,  -67.25053911,
        -64.87875765,  -57.64908961,  -56.74198875,  -60.53575339,
        -313.07119549
    };

    matxf res_px = {
        0.        ,    1.78162012,   -1.16334504,   -4.13184516,
        -7.12742492,   -7.00868081,  -10.0593039 ,  -13.14442965,
        -10.01750314,   -7.33201299,   -9.20936776,   -6.26533127,
        -6.28695831,   -6.28176258,   -6.2819713 ,   -6.34062612,
        -4.67056049,   -2.76486374,    0.32504493,    0.33011303,
        -2.792173  ,   -5.90904314,   -9.02345546,   -8.9950705 ,
        -12.1076821 ,  -15.22001942,  -18.33226194,  -21.44451282,
        -21.41523928,  -24.52766087,  -27.64020363,  -30.75287653,
        -31.41592654,  -33.83702791,  -36.95009481,  -40.06328576,
        -43.17659513,  -43.14842423,  -46.26195211,  -49.37557989,
        -52.48930142,  -55.60311075,  -55.57540949,  -58.68937744,
        -61.80341669,  -64.91752224,  -64.89009668,  -68.00432079,
        -71.1185976 ,  -74.23292301,  -77.34729312,  -77.32011154,
        -80.43456001,  -83.54904246,  -86.66355561,  -86.63650364,
        -89.75106881,  -92.86565551,  -95.98026086,  -99.09488208,
        -99.06792376, -102.18256854, -105.29722111, -108.41187887,
        -106.81415022
    };

//    res_mx.reshape(5,13);
//    mx.reshape(5,13);
//    PRINT(res_mx);
//    PRINT(mx);

//    res_px.reshape(5,13);
//    px.reshape(5,13);
//    PRINT(res_px);
//    PRINT(px);

//    int nmx = mx.size();
//    EXPECT_EQ(res_mx.size(), nmx);
//    for (int i = 0; i < nmx; ++i) {
//        EXPECT_LE(ulp_difference(res_mx[i],mx[i], 1.0e-2f), 1024);
//    }

//    int npx = px.size();
//    EXPECT_EQ(res_px.size(), npx);
//    for (int i = 0; i < npx; ++i) {
//        EXPECT_LE(ulp_difference(res_px[i],px[i], 1.0e-2f), 1024);
//    }
}



TEST(Audio_p3, DftAnal) {
    int fs = 1000;
    int f1 = 100;
    int ns = 10;
    matxf x = genSine(1,f1,0,fs,ns);
    matxf res_x1 = {1.        ,  0.80901699,  0.30901699, -0.30901699, -0.80901699,
                    -1.        , -0.80901699, -0.30901699,  0.30901699,  0.80901699 };

    compare(res_x1, x);

    matxf w = hamming(ns);
    matxf resw = { 0.08      ,  0.16785218,  0.39785218,  0.68214782,  0.91214782,
                   1.        ,  0.91214782,  0.68214782,  0.39785218,  0.16785218 };

    compare(resw,w);

    matxf mX;
    matxf pX;
    uint N = 16;

    EXPECT_TRUE(is_power2(N));   // raise error if N not a power of two
    EXPECT_TRUE(w.size() <= N);  // raise error if window size bigger than fft size
    EXPECT_TRUE(x.size() == w.size());

    int hN = (N/2)+1;                           // size of positive spectrum, it includes sample 0
    EXPECT_EQ(9, hN);
    int hM1 = int(std::floor((w.size()+1)/2));  // not used, half analysis window size by rounding
    EXPECT_EQ(5, hM1);
    int hM2 = int(std::floor((w.size())/2));    // half analysis window size by floor
    EXPECT_EQ(5, hM2);
    float wsum = sum(w);// normalize analysis window
    EXPECT_EQ(5.4f, wsum);
    matxf wn = w.transform([wsum](float wi){ return wi/wsum; });
    matxf reswn = {0.01481481,  0.03108374,  0.07367633,  0.12632367,  0.16891626,
                   0.18518519,  0.16891626,  0.12632367,  0.07367633,  0.03108374};
    compare(reswn, wn);
    matxf xw = mul(x, wn);// window the input sound
    matxf resxw = { 0.01481481,  0.02514727,  0.02276724, -0.03903616, -0.13665613,
                    -0.18518519, -0.13665613, -0.03903616,  0.02276724,  0.02514727};
    compare(resxw, xw);
    matxf fftbuffer(N);// initialize buffer for FFT
    std::copy(xw.begin() + hM2, xw.end(), fftbuffer.begin());// zero-phase window in fftbuffer
    std::copy(xw.begin(), xw.begin() + hM2, fftbuffer.begin() + N - hM1);
    matxf resbuf = {-0.18518519, -0.13665613, -0.03903616,  0.02276724,  0.02514727,
                    0.        ,  0.        ,  0.        ,  0.        ,  0.        ,
                    0.        ,  0.01481481,  0.02514727,  0.02276724, -0.03903616,
                    -0.13665613};
    compare(resbuf, fftbuffer);
    matxcx X = fft(fftbuffer);       // compute FFT
    matxcx resX = {
        cplx(-4.25925926e-01,+0.)        ,  cplx(-4.81142345e-01+0.0136871) ,
        cplx(-4.71414070e-01,-0.01047566),  cplx(-2.62953055e-01-0.00566938),
        cplx(-5.68183198e-02,+0.01481481),  cplx( 2.99362065e-03-0.00566938),
        cplx( 4.54612064e-04,-0.01047566),  cplx( 3.61038766e-04+0.0136871) ,
        cplx( 8.32667268e-17,+0.)        ,  cplx( 3.61038766e-04-0.0136871) ,
        cplx( 4.54612064e-04,+0.01047566),  cplx( 2.99362065e-03+0.00566938),
        cplx(-5.68183198e-02,-0.01481481),  cplx(-2.62953055e-01+0.00566938),
        cplx(-4.71414070e-01,+0.01047566),  cplx(-4.81142345e-01-0.0136871)
    };
//    compare(resX, X, 0.02f);
//    PRINT(resX);
//    PRINT(X);
    //    mX.resize(hN);                          // magnitude spectrum of positive frequencies in dB
//    X.resize(hN);
//    mX = genMagSpecDb_fromdft(X);
//    pX.resize(hN);                              // unwrapped? phase spectrum of positive frequencies
//    pX = phase_unwrap(genPhaseSpec_fromdft(X));


//    matxf mx,px;
//    dftAnal(x1,w,16,mx,px);

//    matxf res_mx = {
//        -313.07119549,  -60.34442737,  -56.01271604,  -56.1233137 ,
//        -62.40111344,  -63.76649348,  -53.64725013,  -51.37820502,
//        -54.53875812,  -75.51880692,  -48.28934419,  -22.00264589,
//        -8.80748602,   -6.18076691,  -12.46576001,  -34.00200244,
//        -56.57847473,  -50.13650937,  -60.86673645,  -53.33326345,
//        -47.96324078,  -48.22551215,  -53.52674371,  -69.15452189,
//        -52.51998941,  -50.04795141,  -52.07300048,  -61.646589  ,
//        -59.65079738,  -52.77710104,  -52.20802779,  -56.32379637,
//        -312.531429  ,  -56.92654373,  -53.41650831,  -54.59731481,
//        -62.09180609,  -64.72055862,  -55.79518901,  -54.43653828,
//        -57.5955298 ,  -74.93855689,  -60.03849894,  -55.47408089,
//        -55.92824452,  -61.91227603,  -69.7058003 ,  -57.86869835,
//        -55.74597961,  -58.0877992 ,  -70.14451511,  -62.57163617,
//        -56.80980997,  -56.57995614,  -61.37135457,  -76.50165743,
//        -59.39276836,  -56.4724787 ,  -58.07511635,  -67.25053911,
//        -64.87875765,  -57.64908961,  -56.74198875,  -60.53575339,
//        -313.07119549
//    };

//    matxf res_px = {
//        0.        ,    1.78162012,   -1.16334504,   -4.13184516,
//        -7.12742492,   -7.00868081,  -10.0593039 ,  -13.14442965,
//        -10.01750314,   -7.33201299,   -9.20936776,   -6.26533127,
//        -6.28695831,   -6.28176258,   -6.2819713 ,   -6.34062612,
//        -4.67056049,   -2.76486374,    0.32504493,    0.33011303,
//        -2.792173  ,   -5.90904314,   -9.02345546,   -8.9950705 ,
//        -12.1076821 ,  -15.22001942,  -18.33226194,  -21.44451282,
//        -21.41523928,  -24.52766087,  -27.64020363,  -30.75287653,
//        -31.41592654,  -33.83702791,  -36.95009481,  -40.06328576,
//        -43.17659513,  -43.14842423,  -46.26195211,  -49.37557989,
//        -52.48930142,  -55.60311075,  -55.57540949,  -58.68937744,
//        -61.80341669,  -64.91752224,  -64.89009668,  -68.00432079,
//        -71.1185976 ,  -74.23292301,  -77.34729312,  -77.32011154,
//        -80.43456001,  -83.54904246,  -86.66355561,  -86.63650364,
//        -89.75106881,  -92.86565551,  -95.98026086,  -99.09488208,
//        -99.06792376, -102.18256854, -105.29722111, -108.41187887,
//        -106.81415022
//    };

//    res_mx.reshape(5,13);
//    mx.reshape(5,13);
//    PRINT(res_mx);
//    PRINT(mx);

//    res_px.reshape(5,13);
//    px.reshape(5,13);
//    PRINT(res_px);
//    PRINT(px);

//    int nmx = mx.size();
//    EXPECT_EQ(res_mx.size(), nmx);
//    for (int i = 0; i < nmx; ++i) {
//        EXPECT_LE(ulp_difference(res_mx[i],mx[i], 1.0e-2f), 1024);
//    }

//    int npx = px.size();
//    EXPECT_EQ(res_px.size(), npx);
//    for (int i = 0; i < npx; ++i) {
//        EXPECT_LE(ulp_difference(res_px[i],px[i], 1.0e-2f), 1024);
//    }
}