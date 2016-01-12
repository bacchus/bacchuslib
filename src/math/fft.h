#pragma once

#include <iostream>
#include <iomanip>
#include <vector>

#include "mdefs.h"
#include "matmxn.h"

namespace bacchus {

//inline uint rev_bits(uint x, uint n);
//std::vector<cplx> fft_iterative(const std::vector<cplx>& a, bool rev);
//std::vector<cplx> fft_bluestein(const std::vector<cplx>& a, bool rev);
matxcx fft_recursive(const matxcx& a, bool rev);
matxcx fft_slow_gorn(const matxcx& a, bool rev);

//std::vector<cplx> fft(const std::vector<cplx>& a);
//std::vector<cplx> fftrev(const std::vector<cplx>& y);
//std::vector<cplx> mul_vec(const std::vector<cplx>& a, const std::vector<cplx>& b);
//std::vector<cplx> convolve(const std::vector<cplx>& a, const std::vector<cplx>& b);

matxcx fft(const matxcx& a);
matxcx fftrev(const matxcx& y);
matxcx convolve(const matxcx& a, const matxcx& b);

matxcxd fftd(const matxd &a);
matxd revfftd(const matxcxd &y);

} // namespace bacchus
