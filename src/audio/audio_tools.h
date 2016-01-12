#pragma once

#include <vector>

#include "math/mdefs.h"
#include "math/matmxn.h"

namespace bacchus {

//- amplitude A,
//- frequency f (Hz),
//- initial phase phi (radians),
//- sampling rate fs (Hz)
//- duration t (seconds)

// in DFT bin = f*N/fs
matxf genSine(float A, float f, float phi, float fs, float t);
matxf genSine(float A, float f, float phi, float fs, int num_samlpes);
matxcx genComplexSine(int k, int N);
matxcx DFT(const matxf& x);
matxf IDFT(const matxcx& x);
matxf genMagSpec(const matxf& x);
matxf genMagSpecDb(const matxf &x);
matxf genMagSpecDb_fromdft(const matxcx &y);
matxf genPhaseSpec_fromdft(const matxcx &y);
matxf phase_unwrap(const matxf &phi);

void suppressFreqDFTmodel(const matxf& x, float fs, int N, matxf& y, matxf& yfilt);
matxf hamming(int N);
bool testRealEven(const matxf& x, matxf& dftbuffer, matxcx& y);
matxf optimalZeropad(const matxf& x, float fs, float f);
matxf minimizeEnergySpreadDFT(const matxf& x, float fs, float f1, float f2);

void dftAnal(const matxf& x, const matxf& w, uint N, matxf& mX, matxf& pX);
matxf dftSynth(const matxf& mX, const matxf& pX, uint M);

void stftAnal(const matxf& x, const matxf& w, uint N, uint H, matxf& xmX, matxf& xpX);
matxf stftSynth(const matxf& mY, const matxf& pY, uint M, uint H);

matxd get_window_hamming(int N);
matxd genSined(double A, double f, double phi, double fs, int num_samlpes);

} // namespace bacchus
