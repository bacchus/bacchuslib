#include "fft.h"
#include "utils/logger.h"
#include <complex>

namespace bacchus {

uint rev_bits(uint x, uint n) {
    uint res = 0;
    for (uint i = 0; i < n; ++i) {
        res = (res<<1) | (x&1);
        x >>= 1;
    }
    return res;
}

matxcx fft_recursive(const matxcx& a, bool rev) {
    int n = a.size();
    if (n==1) {
        return a;
    }
    assert(is_pow2(n));// n must be power of 2

    matxcx y(n);

    cplx w = 1.f;
    cplx wn = std::polar(1.f, 2.0f*(float)M_PI/(float)(rev?n:-n));

    matxcx a0(n/2);
    matxcx a1(n/2);
    for (int k = 0; k < n/2; ++k) {
        a0[k] = a[2*k  ];
        a1[k] = a[2*k+1];
    }
    matxcx y0 = fft_recursive(a0, rev);
    matxcx y1 = fft_recursive(a1, rev);
    for (int k = 0; k < n/2; ++k) {
        cplx t = w*y1[k];
        y[k    ] = y0[k] + t;
        y[k+n/2] = y0[k] - t;
        w = w*wn;
    }
    return y;
}

matxcx fft_slow_gorn(const matxcx& a, bool rev) {
    float c = (rev?1.f:-1.f);
    uint n = a.size();
    matxcx res(n);
    for (uint k = 0; k < n; ++k) {
        for (uint t = 0; t < n; ++t) {
            res[k] += a[t]*std::polar(1.f, 2.0f*(float)M_PI*c*t*k/(float)n);
        }
    }
    return res;
}

matxcx fft_iterative(const matxcx& a, bool rev) {
    uint n = a.size();
    assert(is_pow2(n));// n must be power of 2

    uint lgn = 0;
    uint tmp = n;
    while (tmp >>= 1) {
        ++lgn;
    }

    matxcx y(n);
    for (uint i = 0; i < a.size(); ++i) {
        y[rev_bits(i,lgn)] = a[i];
    }

    for (uint s = 2; s <= n; s*=2) {
        uint hs = s/2;
        //TODO: here is the trick: -s is unsigned
        cplx wn = std::polar(1.f, 2.0f*(float)M_PI/int(rev?s:-s));

        for (uint i = 0; i < n; i+=s) {
            cplx w = 1.f;
            for (uint j = i; j < i + hs; ++j) {
                cplx t = w*y[j+hs];
                y[j+hs] = y[j] - t;
                y[j   ] = y[j] + t;
                w = w*wn;
            }
        }
    }
    return y;
}

matxcx fft_bluestein(const matxcx& a, bool rev) {
    //assert(false);

    uint n = a.size();

    uint m = 1;
    while (m < (2*n+1)) {
        m *= 2;
    }

    float frv = (rev?1.f:-1.f);
    matxcx w(n);
    for (uint i = 0; i < n; ++i) {
        float th = M_PI*((i*i) % (2*n))/(float)n;
        w[i] = std::polar(1.f, frv*th);
    }

    matxcx ta(m);
    for (uint i = 0; i < n; ++i) {
        ta[i] = a[i]*w[i];
    }

    matxcx tb(m);
    tb[0] = w[0];
    for (uint i = 1; i < n; ++i) {
        tb[i  ] = w[i];
        tb[m-i] = w[i];//TODO: fail tb.data[m] out of size
    }

    matxcx tc = convolve(ta,tb);
    for (uint i = 0; i < n; ++i) {
        w[i] = tc[i]*w[i];
    }
    return w;
}

matxcx fft(const matxcx &a) {
    uint n = a.size();
    if (n==0) {
        return a;
    } else if ((n&(n-1))==0) {
        return fft_iterative(a, false);
    } else {
        assert(false);
        return fft_bluestein(a, false);
    }
}

matxcx fftrev(const matxcx &y) {
    uint n = y.size();
    if (n==0) {
        return y;
    } else if ((n&(n-1))==0) {
        return fft_iterative(y, true)/cplx(n);
    } else {
        assert(false);
        return fft_bluestein(y, true)/cplx(n);
    }
}

matxcx convolve(const matxcx &a, const matxcx &b) {
    return fftrev(mul(fft(a), fft(b)));
}


matxcxd fftd(const matxd& a) {
    uint n = a.size();
    assert(is_pow2(n));// n must be power of 2
    uint lgn = 0;
    uint tmp = n;
    while (tmp >>= 1) {
        ++lgn;
    }
    matxcxd y(n);
    for (uint i = 0; i < a.size(); ++i) {
        y[rev_bits(i,lgn)] = cplxd(a[i]);
    }
    for (uint s = 2; s <= n; s*=2) {
        uint hs = s/2;
        //TODO: here is the trick: -s is unsigned
        cplxd wn = std::polar(1.0, 2.0*M_PI/int(-s));
        for (uint i = 0; i < n; i+=s) {
            cplxd w = 1.0;
            for (uint j = i; j < i + hs; ++j) {
                cplxd t = w*y[j+hs];
                y[j+hs] = y[j] - t;
                y[j   ] = y[j] + t;
                w = w*wn;
            }
        }
    }
    return y;
}

matxd revfftd(const matxcxd& a) {
    uint n = a.size();
    assert(is_pow2(n));// n must be power of 2
    uint lgn = 0;
    uint tmp = n;
    while (tmp >>= 1) {
        ++lgn;
    }
    matxcxd y(n);
    for (uint i = 0; i < a.size(); ++i) {
        y[rev_bits(i,lgn)] = a[i];
    }
    for (uint s = 2; s <= n; s*=2) {
        uint hs = s/2;
        cplxd wn = std::polar(1.0, 2.0*M_PI/double(s));
        for (uint i = 0; i < n; i+=s) {
            cplxd w = 1.0;
            for (uint j = i; j < i + hs; ++j) {
                cplxd t = w*y[j+hs];
                y[j+hs] = y[j] - t;
                y[j   ] = y[j] + t;
                w = w*wn;
            }
        }
    }
    matxd rey(n);
    std::transform(y.begin(), y.end(), rey.begin(), [](const cplxd& yi){ return yi.real(); });
    return rey;
}

} // namespace bacchus
