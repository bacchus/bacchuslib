#include "dft_model.h"
#include "math/mdefs.h"
#include "math/fft.h"

namespace bacchus {

matxd phase_unwrapd(const matxd &phi) {
    matxd res(phi.size());
    std::adjacent_difference(phi.begin(), phi.end(), res.begin());
    res[0] = 0;
    std::transform(res.begin(), res.end(), res.begin(),
                   [](double p) {
        if (p>M_PI) return 1;
        if (p<-M_PI) return -1;
        return 0;
    });
    int n = 0;
    for (uint i = 0; i < res.size(); ++i) {
        n -= res[i];
        res[i] = phi[i] + n*2.0*M_PI;
    }
    return res;
}

matxd genMagSpecDb_fromdftd(const matxcxd &y) {
    matxd mag(y.size());
    std::transform(y.begin(), y.end(), mag.begin(), [](const cplxd& c){ return 20.0*std::log10(std::abs(c)); });
    return mag;
}

matxd genPhaseSpec_fromdftd(const matxcxd &y) {
    matxd phi(y.size());
    std::transform(y.begin(), y.end(), phi.begin(), [](const cplxd& c){ return std::arg(c); });
    return phi;
}

void dftAnald(const matxd& x, const matxd& w, uint N, matxd& mX, matxd& pX) {
    assert(is_power2(N)); // raise error if N not a power of two
    assert(w.size() <= N); // raise error if window size bigger than fft size
    assert(x.size() == w.size()); // raise error if window size not same as sample size
    int hN = (N/2)+1; // size of positive spectrum, it includes sample 0
    int hM1 = int(std::floor((w.size()+1)/2)); // half analysis window size by rounding
    int hM2 = int(std::floor((w.size())/2)); // half analysis window size by floor
    matxd wn = w/sum(w);
    matxd xw = mul(x, wn);// window the input sound
    matxd fftbuffer(N);// initialize buffer for FFT
    std::copy(xw.begin() + hM2, xw.end(), fftbuffer.begin());// zero-phase window in fftbuffer
    std::copy(xw.begin(), xw.begin() + hM2, fftbuffer.end() - hM1);
    matxcxd X = fftd(fftbuffer); // compute FFT
    X.resize(hN); // take half of spectrum positive frequencies
    mX = genMagSpecDb_fromdftd(X); // magnitude spectrum of positive frequencies in dB
    pX = phase_unwrapd(genPhaseSpec_fromdftd(X)); // unwrapped phase spectrum of positive frequencies
}

matxd dftSynthd(const matxd& mX, const matxd& pX, uint M) {
    uint hN = mX.size(); // size of positive spectrum, it includes sample 0
    uint N = (hN-1)*2; // FFT size
    assert(is_power2(N)); // raise error if N not a power of two, thus mX is wrong
    uint hM1 = int(std::floor((M+1)/2)); // half analysis window size by rounding
    uint hM2 = int(std::floor(M/2)); // half analysis window size by floor
    matxcxd Y(N); // clean output spectrum
    std::transform(mX.rbegin(), mX.rend(), pX.rbegin(), Y.begin(),
                   [](double mx, double px){
        return std::pow(10.0, mx/20.0)*std::polar(1.0, -px);
    }); // generate negative frequencies
    std::transform(mX.begin(), mX.end(), pX.begin(), Y.begin() + hN,
                   [](double mx, double px){
        return std::pow(10.0, mx/20.0)*std::polar(1.0, px);
    }); // generate positive frequencies
    matxd fftbuffer = revfftd(Y);
    matxd y; // initialize output array
    y.reserve(M);
    y.append(fftbuffer.begin() + hM2, fftbuffer.end()); // undo zero-phase window
    y.append(fftbuffer.begin(), fftbuffer.begin() + hM1); // undo zero-phase window
    return y;
}

} // namespace bacchus
