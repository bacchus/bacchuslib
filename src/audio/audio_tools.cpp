#include "audio_tools.h"
#include "math/fft.h"
#include "utils/logger.h"

namespace bacchus {

matxf genSine(float A, float f, float phi, float fs, float t) {
    float N = fs/f;
    int num_samlpes = fs*t;
    matxf n(num_samlpes);
    std::iota(n.begin(), n.end(), 0);
    return n.transform([A,N,phi](float ni){ return A*std::cos(2.f*M_PI*ni/N + phi); });
}

matxf genSine(float A, float f, float phi, float fs, int num_samlpes) {
    float N = fs/f;
    matxf n(num_samlpes);
    std::iota(n.begin(), n.end(), 0.f);
    return n.transform([A,N,phi](float ni){ return A*std::cos(2.f*M_PI*ni/N + phi); });
}

matxd genSined(double A, double f, double phi, double fs, int num_samlpes) {
    double N = fs/f;
    matxd n(num_samlpes);
    std::iota(n.begin(), n.end(), 0.0);
    return n.transform([A,N,phi](double ni){ return A*std::cos(2.0*M_PI*ni/N + phi); });
}

matxcx genComplexSine(int k, int N) {
    int num_samlpes = N;
    matxf n(num_samlpes);
    std::iota(n.begin(), n.end(), 0);
    float i2pk_N = -2.f*M_PI*k/(float)N;
    //TODO: mat type mismatch
    matxcx s(num_samlpes);
    std::transform(n.begin(), n.end(), s.begin(), [i2pk_N](float ni){ return std::polar(1.f, i2pk_N*ni); });
    return s;
}

matxcx DFT(const matxf &x) {
    int N = x.size();
    matxcx y(N);
    for (int k = 0; k < N; ++k) {
        matxcx s = genComplexSine(k,N);
        //TODO: mat type mismatch
        y[k] = std::inner_product(x.begin(), x.end(), s.begin(), cplx(0.f,0.f));
    }
    return y;
}

matxf IDFT(const matxcx &x) {
    // same as DFT but with + in exponent
    int N = x.size();
    matxf y(N);
    for (int k = 0; k < N; ++k) {
        matxcx s = genComplexSine(-k,N);
        cplx yk = inprod(x,s);
        y[k] = yk.real()/(float)N;
    }
    return y;
}

matxf genMagSpecDb_fromdft(const matxcx &y) {
    matxf mag(y.size());
    std::transform(y.begin(), y.end(), mag.begin(), [](const cplx& c){ return 20.f*std::log10(std::abs(c)); });
    return mag;
}

matxf genPhaseSpec_fromdft(const matxcx &y) {
    matxf phi(y.size());
    std::transform(y.begin(), y.end(), phi.begin(), [](const cplx& c){ return std::arg(c); });
    return phi;
}

matxf phase_unwrap(const matxf &phi) {
    matxf res(phi.size());
    std::adjacent_difference(phi.begin(), phi.end(), res.begin());
    res[0] = 0;
    std::transform(res.begin(), res.end(), res.begin(),
                   [](float p) {
        if (p>M_PI) return 1;
        if (p<-M_PI) return -1;
        return 0;
    });
    int n = 0;
    for (uint i = 0; i < res.size(); ++i) {
        n -= res[i];
        res[i] = phi[i] + n*2.f*M_PI;
    }
    return res;
}

matxf genMagSpec(const matxf &x) {
    matxcx y = DFT(x);
    matxf mag(x.size());
    std::transform(y.begin(), y.end(), mag.begin(), [](const cplx& c){ return std::abs(c); });
    return mag;
}

matxf genMagSpecDb(const matxf &x) {
    return genMagSpecDb_fromdft(DFT(x));
}

matxf minimizeEnergySpreadDFT(const matxf& x, float fs, float f1, float f2) {
    int M = lcm(fs/f1, fs/f2);
    matxf x1(x.begin(), x.begin() + M);
    matxcx y = DFT(x1);
    y.resize(M/2+1);
    return genMagSpecDb_fromdft(y);
}

matxf optimalZeropad(const matxf& x, float fs, float f) {
    int M = x.size();
    int samples_in_period = fs/f;
    int dn = samples_in_period - M % samples_in_period;
    int N = M + dn;
    matxf x1(x);
    x1.resize(N);
    matxcx y = DFT(x1);
    y.resize(N/2+1);
    return genMagSpecDb_fromdft(y);
}

bool testRealEven(const matxf& x, matxf& dftbuffer, matxcx& y) {
    int N = x.size();
    int hN = N/2;
    dftbuffer.clear();
    dftbuffer.reserve(N);
    dftbuffer.append(x.begin() + hN, x.end());
    dftbuffer.append(x.begin(), x.begin() + hN);
    y = DFT(dftbuffer);
    y.mutate([](const cplx& c) {
        return cplx(c.real(), eq(0.f, c.imag()) ? 0.f : c.imag());
    });
    float sum_phase = std::accumulate(y.begin(), y.end(), 0.f,
                                [](float sum, const cplx& c) {
        return sum + std::abs(c.imag());
    });
    return eq(0.f, sum_phase);
}

matxf hamming(int N) {
    matxf res(N);
    float p2n = 2.f*M_PI/(float)N;
    for (uint i = 0; i < res.size(); ++i) {
        res[i] = 0.54f - 0.46f*cos(i*p2n);
    }
    return res;
}

void dftAnal(const matxf& x, const matxf& w, uint N, matxf& mX, matxf& pX) {
    assert(is_power2(N));   // raise error if N not a power of two
    assert(w.size() <= N);  // raise error if window size bigger than fft size
    assert(x.size() == w.size());
    int hN = (N/2)+1;                           // size of positive spectrum, it includes sample 0
    int hM1 = int(std::floor((w.size()+1)/2));  // not used, half analysis window size by rounding
    int hM2 = int(std::floor((w.size())/2));    // half analysis window size by floor
    float wsum = sum(w);// normalize analysis window
    matxf wn = w.transform([wsum](float wi){ return wi/wsum; });
    matxf xw = mul(x, wn);// window the input sound
    matxf fftbuffer(N);// initialize buffer for FFT
    std::copy(xw.begin() + hM2, xw.end(), fftbuffer.begin());// zero-phase window in fftbuffer
    std::copy(xw.begin(), xw.begin() + hM2, fftbuffer.begin() + N - hM1);
    matxcx X = DFT(fftbuffer);       // compute FFT
    mX.resize(hN);                          // magnitude spectrum of positive frequencies in dB
    X.resize(hN);
    mX = genMagSpecDb_fromdft(X);
    pX.resize(hN);                              // unwrapped? phase spectrum of positive frequencies
    pX = phase_unwrap(genPhaseSpec_fromdft(X));
}

matxf dftSynth(const matxf& mX, const matxf& pX, uint M) {
    uint hN = mX.size();                        // size of positive spectrum, it includes sample 0
    uint N = (hN-1)*2;                          // FFT size
    assert(is_power2(N));                       // raise error if N not a power of two, thus mX is wrong
    uint hM1 = int(std::floor((M+1)/2));        // half analysis window size by rounding
    uint hM2 = int(std::floor(M/2));            // half analysis window size by floor
    matxcx Y(N);                                // clean output spectrum
    std::transform(mX.rbegin(), mX.rend(), pX.rbegin(), Y.begin(),
                   [](float mx, float px){
        return std::pow(10.f, mx/20.f)*std::exp(cplx(0.f, -px));
    }); // generate negative frequencies
    std::transform(mX.begin(), mX.end(), pX.begin(), Y.begin() + hN,
                   [](float mx, float px){
        return std::pow(10.f, mx/20.f)*std::exp(cplx(0.f, px));
    }); // generate positive frequencies
    matxcx cxfftbuffer = IDFT(Y);
    matxf fftbuffer(cxfftbuffer.size());
    std::transform(cxfftbuffer.begin(), cxfftbuffer.end(), fftbuffer.begin(),
            [](const cplx& c) { return c.real(); });
    matxf y;
    y.reserve(M);                               // initialize output array
    y.append(fftbuffer.begin() + hM2, fftbuffer.end());  // undo zero-phase window
    y.append(fftbuffer.begin(), fftbuffer.begin() + hM1);// undo zero-phase window
    return y;
}

void suppressFreqDFTmodel(const matxf& x, float fs, int N, matxf& y, matxf& yfilt) {
    int M = x.size();
    const matxf w = hamming(M);
    float wsum = std::accumulate(w.begin(),w.end(),0);
    matxf mX;
    matxf pX;
    dftAnal(x,w,N,mX,pX);
    y = dftSynth(mX, pX, w.size());
    std::transform(y.begin(), y.end(), y.begin(), [wsum](float yi){return yi*wsum;});
    int idx70 = std::ceil(70.f*N/fs);
    matxf mXfilt(mX);
    std::transform(mXfilt.begin(), mXfilt.begin()+idx70,mXfilt.begin(),
                   [](const float&){
        return -60.f;
    });
    yfilt = dftSynth(mXfilt,pX,w.size());
    std::transform(y.begin(), y.end(), y.begin(), [wsum](float yi){return yi*wsum;});
}

void stftAnal(const matxf &x, const matxf &w, uint N, uint H, matxf &xmX, matxf &xpX) {
    uint m = w.size();
    uint hm1 = int(std::floor((m+1)/2));  // not used, half analysis window size by rounding
    uint hm2 = int(std::floor(m/2));    // half analysis window size by floor
    matxf zeros_hm2 = matxf::zero(hm2);
    matxf xex = concat(zeros_hm2, concat(x, zeros_hm2));
    uint pin = hm1;
    uint pend = xex.size() - hm1;
    matxf wn = w/sum(w);
    //matxf y = matxf::zero(xex.size());
    while (pin <= pend) {
        matxf::mat_citer first = xex.begin() + pin - hm1;
        matxf::mat_citer last = xex.begin() + hm2;
        matxf x1(first, last);
        matxf mX,pX;
        dftAnal(x1,wn,N,mX,pX);
        if (pin == hm1) {
            xmX = mX;
            xpX = pX;
        } else {
            xmX.append_row(mX);
            xpX.append_row(pX);
        }
        pin += H;
    }
}

matxf stftSynth(const matxf &mY, const matxf &pY, uint M, uint H) {
    uint hm1 = int(std::floor((M+1)/2));  // not used, half analysis window size by rounding
    uint hm2 = int(std::floor(M/2));    // half analysis window size by floor
    uint nFrames = mY.m;
    matxf y(nFrames*H + hm1 + hm2);
    uint pin = hm1;
    for (uint i = 0; i < nFrames; ++i) {
        matxf y1 = dftSynth(ext_row_get(mY,i), ext_row_get(pY,i), M);
        uint offs = pin-hm1;
        for (uint j = offs; j < pin+hm2; ++j) {
            y[j] += H*y1[j-offs];   // overlap-add to generate output sound
        }
        pin += H; // advance sound pointer
    }
    return matxf(y.begin() + hm2, y.end() - hm1);
}

//------------ DOUBLE ------------
matxd get_window_hamming(int N) {
    matxd res(N);
    double p2n = 2.0*M_PI/(double)N;
    for (uint i = 0; i < res.size(); ++i) {
        res[i] = 0.54 - 0.46*cos(p2n*i);
    }
    return res;
}

} // namespace bacchus
