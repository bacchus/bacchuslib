#include "stft_model.h"
#include "dft_model.h"

namespace bacchus {

void stftAnald(const matxd& x, const matxd& w, uint N, uint H, matxd& xmX, matxd& xpX) {
    uint M = w.size(); // size of analysis window
    uint hm1 = int(std::floor((M+1)/2)); // half analysis window size by rounding
    uint hm2 = int(std::floor(M/2)); // half analysis window size by floor
    matxd zeros_hm2(hm2);
    matxd xex = concat(zeros_hm2, concat(x, zeros_hm2)); // add zeros at the beginning and end
    uint pin = hm1; // initialize sound pointer in middle of analysis window
    uint pend = xex.size() - hm1; // last sample to start a frame
    matxd wn = w/sum(w); // normalize analysis window
    while (pin <= pend) { // while sound pointer is smaller than last sample
        matxd::mat_citer first = xex.begin() + pin - hm1;
        matxd::mat_citer last = xex.begin() + pin + hm2;
        matxd x1(first, last); // select one frame of input sound
        matxd mX,pX;
        dftAnald(x1,wn,N,mX,pX); // compute dft
        if (pin == hm1) {
            xmX = mX; // if first frame create output arrays
            xpX = pX;
        } else {
            xmX.append_row(mX); // append output to existing array
            xpX.append_row(pX);
        }
        pin += H; // advance sound pointer
    }
}

matxd stftSynthd(const matxd& mY, const matxd& pY, uint M, uint H) {
    uint hm1 = int(std::floor((M+1)/2)); // half analysis window size by rounding
    uint hm2 = int(std::floor(M/2)); // half analysis window size by floor
    uint nFrames = mY.m; // number of frames
    matxd y(nFrames*H + hm1 + hm2); // initialize output array
    uint pin = hm1;
    for (uint i = 0; i < nFrames; ++i) { // iterate over all frames
        matxd y1 = dftSynthd(ext_row_get(mY,i), ext_row_get(pY,i), M); // compute idft
        uint offs = pin - hm1;
        for (uint j = offs; j < pin+hm2; ++j) {
            y[j] += H*y1[j-offs];   // overlap-add to generate output sound
        }
        pin += H; // advance sound pointer
    }
    return matxd(y.begin() + hm2, y.end() - hm1); // delete window which was added in stftAnal
}

} // namespace bacchus
