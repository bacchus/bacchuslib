#pragma once

//#include <iostream>
//#include <iomanip>
//#include <vector>
//#include <complex>

//#include "mdefs.h"

//namespace bacchus {

//typedef std::complex<float> cplx;

//inline cplx set_nk(int n, int k=1) {
////    if (k==n || k==0 || n==0) {
////        //re = 1; im = 0; return
////        return cplx(1.f,0.f);
////    }

////    if (n==2*k || (k>0 && n>0 && k%n==0)) {
////        //re = -1; im = 0; return;
////        return cplx(-1.f,0.f);
////    }

//    // w(n,k) = exp(2*pi*i*k/n)
////    float th = 2.0f*M_PI*(float)k/(float)n;
////    re = std::cos(th);
////    im = std::sin(th);

//    ;
//    return std::exp(cplx(0.f, 2.0f*M_PI*(float)k/(float)n));
//}

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

//struct cplx {
//    float re,im;

//    cplx(): re(0), im(0) { }
//    cplx(float a): re(a), im(0) { }
//    cplx(float a, float b): re(a), im(b) { }
//    cplx(const cplx& z): re(z.re), im(z.im) { }

//    cplx& operator=(const cplx& z) {
//        //cplx tmp(z);
//        //swap(z);
//        re = z.re;
//        im = z.im;
//        return *this;
//    }

//    void set_nk(int n, int k=1) {
//        if (k==n || k==0 || n==0) {
//            re = 1; im = 0; return;
//        }

//        if (n==2*k || (k>0 && n>0 && k%n==0)) {
//            re = -1; im = 0; return;
//        }

//        // w(n,k) = exp(2*pi*i*k/n)
//        float th = 2.0f*M_PI*(float)k/(float)n;
//        re = std::cos(th);
//        im = std::sin(th);
//    }

//    void set_rt(float r, float th) {
//        re = r*std::cos(th);
//        im = r*std::sin(th);
//    }

//    float mag() const {
//        return sqrt(re*re + im*im);
//    }

//    float arg() const {
//        return atan2(im,re);
//    }

////    operator float() const {
////        assert(im==0);
////        return re;
////    }

//    cplx& operator*=(float c) {
//        re *= c;
//        im *= c;
//        return *this;
//    }

//    cplx& operator/=(float c) {
//        assert(c!=0);
//        re /= c;
//        im /= c;
//        return *this;
//    }

//    cplx& operator*=(const cplx& z1) {
//        float tm = re;
//        re = re*z1.re - im*z1.im;
//        im = tm*z1.im + im*z1.re;
//        return *this;
//    }

//    cplx& operator/=(const cplx& z1) {
//        assert(z1.re!=0 || z1.im!=0);
//        float div = 1.0f/(z1.re*z1.re + z1.im*z1.im);
//        float tm = re;
//        re = div*(re*z1.re + im*z1.im);
//        im = div*(im*z1.re - tm*z1.im);
//        return *this;
//    }

//    cplx& operator+=(const cplx& z1) {
//        re += z1.re;
//        im += z1.im;
//        return *this;
//    }

//    cplx& operator-=(const cplx& z1) {
//        re -= z1.re;
//        im -= z1.im;
//        return *this;
//    }
//};

//inline bool operator==(const cplx& z1, const cplx& z2) {
//    return eq(z1.re, z2.re) && eq(z1.im, z2.im);
//}

//inline cplx operator*(const cplx& z1, float c) {
//    return cplx(c*z1.re, c*z1.im);
//}

//inline cplx operator*(float c, const cplx& z1) {
//    return cplx(c*z1.re, c*z1.im);
//}

//inline cplx operator/(const cplx& z1, float c) {
//    assert(c!=0);
//    return cplx(z1.re/c, z1.im/c);
//}

//inline cplx operator*(const cplx& z1, const cplx& z2) {
//    return cplx(z1.re*z2.re - z1.im*z2.im, z1.im*z2.re + z1.re*z2.im);
//}

//inline cplx operator/(const cplx& z2, const cplx& z1) {
//    assert(z1.re!=0 || z1.im!=0);
//    float div = 1.0f/(z1.re*z1.re + z1.im*z1.im);
//    return cplx(
//                div*(z1.re*z2.re + z1.im*z2.im),
//                div*(z1.re*z2.im - z1.im*z2.re));
//}

//inline cplx operator/(float c, const cplx& z1) {
//    assert(z1.re!=0 || z1.im!=0);
//    float div = c/(z1.re*z1.re + z1.im*z1.im);
//    return cplx(div*z1.re, -div*z1.im);
//}

//inline cplx operator+(const cplx& z1, const cplx& z2) {
//    return cplx(z1.re + z2.re, z1.im + z2.im);
//}

//inline cplx operator-(const cplx& z1, const cplx& z2) {
//    return cplx(z1.re - z2.re, z1.im - z2.im);
//}

//inline cplx operator-(const cplx& z1) {
//    return cplx(-z1.re, -z1.im);
//}

//inline std::ostream& operator <<(std::ostream& ostr, const cplx& z) {
//    bool re = fabs(z.re) > BCC_M_FLOAT_EPSILON;
//    bool im = fabs(z.im) > BCC_M_FLOAT_EPSILON;
//    ostr<<std::setprecision(2);
//    if (re && im) {
//        ostr<<z.re<<"+"<<z.im<<"i";
//    } else if (re) {
//        ostr<<z.re;
//    } else if (im) {
//        ostr<<z.im<<"i";
//    } else {
//        ostr<<"0";
//    }
//    return ostr;
//}

//} // namespace bacchus
