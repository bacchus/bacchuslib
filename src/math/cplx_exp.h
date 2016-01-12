#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>

namespace bacchus {

// exp(i*u) = cos(u) + i*sin(u)
// w(n,k) = exp(2*pi*i*k/n)
// w(n) = w(n,1) = exp(2*pi*i/n)
// w(n,n) = w(n,0) = 1
// w(n,j)*w(n,k) = w(n,(j+k)%n)
// w(n,-1) = w(n,n-1)
// w(dn,dk) = w(n,k)
// w(n,n/2) = w(2*n,n) = w(2) = -1; n%2=0, n>0
// w(n,k)^2 = w(n/2,k); n%2=0, n>0
// w(n,k+n/2)^2 = w(n,2*k+n) = w(n,2*k)*w(n,n) = w(n,2*k) = w(n,k)^2
// w(n,k+n/2) = w(n,n/2)*w(n,k) = -w(n,k)
// sum( w(n,k)^j, j=0..n ) = 0
// w(n,k) = 1; k%n=0, k>0

struct cplx_exp {
    int n,k;
    float a;

    cplx_exp(): n(0), k(0), a(0) {
        check();
    }

    cplx_exp(float aa): n(0), k(0), a(aa) {
        check();
    }

    cplx_exp(int nn, int kk, float aa=1): n(nn), k(kk), a(aa) {
        check();
    }

    explicit cplx_exp(int nn, float aa=1): n(nn), k(1), a(aa) {
        check();
    }

    cplx_exp(const cplx_exp& z): n(z.n), k(z.k), a(z.a) {
        check();
    }

    cplx_exp& operator=(const cplx_exp& z) {
        n = z.n;
        k = z.k;
        a = z.a;
        check();
        return *this;
    }

    operator float() const {
        return to_float();
    }

    float to_float() const {
        assert(n==0);
        return a;
    }

    void check() {
        if (k==n || k==0 || n==0 || a==0) {
            n = 0; k = 0;
            return;
        }

        if (n==2*k) {
            a = -a; n = 0; k = 0;
            return;
        }

        while (k < 0) {
            k += n;
        }

        if (k>0 && n>0) {
            k = k % n;
            int g = gcd(n,k);
            n = n / g;
            k = k / g;
        }
    }
};

inline cplx_exp operator*(const cplx_exp& z1, float c) {
    return cplx_exp(z1.n, z1.k, c*z1.a);
}

inline cplx_exp operator*(float c, const cplx_exp& z1) {
    return cplx_exp(z1.n, z1.k, c*z1.a);
}

inline cplx_exp operator*(const cplx_exp& z1, const cplx_exp& z2) {
    if (z1.n == z2.n)
        return cplx_exp(z1.n, z1.k + z2.k, z1.a*z2.a);
    if (z1.n==0)
        return cplx_exp(z2.n, z2.k, z1.a*z2.a);
    if (z2.n==0)
        return cplx_exp(z1.n, z1.k, z1.a*z2.a);
    int l = lcm(z1.n,z2.n);
    return cplx_exp(l, z1.k*l/z2.k + z2.k*l/z1.k, z1.a*z2.a);
}

inline cplx_exp operator^(const cplx_exp& z1, int c) {
    if (c==0)
        return cplx_exp(0,0,1);
//    if (z1.n%c==0)
//        return cplx_exp(z1.n/c, z1.k, pow(z1.a,c));
    return cplx_exp(z1.n, c*z1.k, pow(z1.a,c));
}

inline cplx_exp operator+(const cplx_exp& z1, const cplx_exp& z2) {
    assert(z1.n == z2.n);
    assert(z1.k == z2.k);
    return cplx_exp(z1.n, z1.k, z1.a+z2.a);
}

inline cplx_exp operator-(const cplx_exp& z1, const cplx_exp& z2) {
    assert(z1.n == z2.n);
    assert(z1.k == z2.k);
    return cplx_exp(z1.n, z1.k, z1.a-z2.a);
}

inline cplx_exp operator-(const cplx_exp& z1) {
    return cplx_exp(z1.n, z1.k, -z1.a);
}

inline std::ostream& operator <<(std::ostream& ostr, const cplx_exp& z) {
    if (z.n==0) {
        ostr<<z.a;
    } else {
        if (z.a!=1) ostr<<z.a<<"*";
        ostr<<"("<<z.n;
        if (z.k == 0) ostr<<")";
        else ostr<<","<<z.k<<")";
    }
    return ostr;
}

} // namespace bacchus
