#include "setting.h"
#include "math/fft.h"
#include "utils/logger.h"
#include "audio/audio_tools.h"
using namespace bacchus;

TEST(FFTDouble, Sine) {
    matxd x = {1,2,3,4};
    std::cout << x << std::endl;
    //PRINT(x);
    matxcxd resy = {cplxd{10,0}, cplxd{-2,2}, cplxd{-2,0}, cplxd{-2,-2}};

    matxcxd y = fftd(x);
    PRINT(y);
    PRINT(fft(x));
}

TEST(FFTDouble, LongSine) {
    matxd x = genSined(1, 100, 0, 1000, 16);
    matxd resx = { 1.         , 0.80901699,  0.30901699, -0.30901699, -0.80901699, -1.,
     -0.80901699, -0.30901699,  0.30901699,  0.80901699,  1.,          0.80901699,
      0.30901699, -0.30901699, -0.80901699, -1.        };
    PRINT(x);
    PRINT(resx);

    matxcxd y = fftd(x);
    PRINT(y);
}
