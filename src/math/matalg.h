#pragma once

#include <assert.h>
#include <string.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>

#include "matmxn.h"

namespace bacchus {

/*
|a00 a01 a02| |b00 b01 B02| |... ... ...|
|A10 A11 A12|*|b10 b11 B12|=|... ... C12|
|a20 a21 a22| |b20 b21 B22| |... ... ...|

|a00 a01 a02| |x| |.|
|A10 A11 A12|*|y|=|C|
|a20 a21 a22| |z| |.|

        |a00 A01 a02|
|x y z|*|a10 A11 a12|=|. C .|
        |a20 A21 a22|
*/
float decomposition_lup(matxf& a, std::vector<int>& t);// also returns determinant
float determinant(const matxf& mat);// uses decomposition_lup
matxf solve_lup(const matxf& a, const matxf &b, const std::vector<int> &t);// LUx=Pb
matxf solve_system(const matxf& a, const matxf& b);// Ax=b
matxf inverse(const matxf& mat);// AX=I
//matxf transpose(const matxf& mat);
//float inprod(const matxf& mat1, const matxf& mat2);
matxf decomposition_cholesky(const matxf& a);// cholesky decomposition
matxf decomposition_ldl(const matxf& a, matxf& d);// LDL cholesky decomposition
matxf cholupdate(const matxf& L, const matxf& v, char op);// cholesky update op: '+' or '-'
//-------------------------------------------------------------------------------------------------

//// deprecated
// det = sum(j=0:n) -1^(j+1) * m(1,j) * M(1,j)
// det = (M(1;1)*M(k;k) - M(1;k)*M(k;1))/M(1,k;1,k)
//float mat_minor(const matxf& mat, std::set<int> si, std::set<int> sj, int ti, int tj, std::vector<int> prev_sgn) {
//    const int dbg = 0;

//    si.erase(ti);
//    sj.erase(tj);
//    for (uint i = ti; i < prev_sgn.size(); ++i) {
//        prev_sgn[i] = -prev_sgn[i];
//    }

//    assert(mat.m == mat.n);
//    assert(si.size() == sj.size());
//    assert(si.size() > 0);

//    int i = *si.begin();

//    if (dbg) std::cout<<ti<<" "<<tj<<": "<<std::endl;

//    if (si.size() == 1) {
//        int j = *sj.begin();
//        if (dbg) std::cout<<"    "<<mat(i,j)<<std::endl;
//        return mat(i,j);
//    }

//    int jn = 0;

//    float sum = 0.0f;
//    //for (std::set<int>::iterator jit = sj.begin(); jit != sj.end(); ++jit) {
//    //int j = *jit;
//    for (auto j : sj) {
//        float sgn = ( ((i+jn)%2) ? -1.0f : 1.0f );
//        sgn *= prev_sgn[i];
//        float mij = mat(i,j);
//        if (mij != 0) {
//            if (dbg) std::cout<<(sgn>0?"+":"-")<<mij<<"*"<<std::endl;
//            float mnr = mat_minor(mat, si,sj,i,j,prev_sgn);
//            float sum_add = sgn*mij*mnr;
//            sum += sum_add;
//            if (dbg) std::cout<<"sum += "<<sum_add<<std::endl;
//        }
//        ++jn;
//    }

//    if (dbg) std::cout<<"return: "<<sum<<std::endl;
//    return sum;
//}

//// deprecated: too slow
//float determinant_minor(const matxf& mat) {
//    assert(mat.m == mat.n);
//    std::set<int> si,sj;
//    std::vector<int> prev_sgn(mat.m+1,1);
//    for (uint i = 0; i < mat.m+1; ++i) {
//        si.insert(i);
//        sj.insert(i);
//    }
//    return mat_minor(mat,si,sj,mat.m,mat.m,prev_sgn);
//}

//// deprecated
//matxf inverse_minor(const matxf& mat) {
//    uint n = mat.n;
//    assert(n == mat.m);

//    std::set<int> si,sj;
//    for (uint i = 0; i < n; ++i) {
//        si.insert(i);
//        sj.insert(i);
//    }

//    float determinant(const matxf&);//prev decl
//    float mdet = determinant(mat);

//    matxf res(n, n);
//    for (uint i = 0; i < n; ++i) {
//        for (uint j = 0; j < n; ++j) {
//            float sgn = ( ((i+j)%2) ? -1.0f : 1.0f );
//            std::vector<int> prev_sgn(n,1);
//            res(i,j) = sgn*mat_minor(mat, sj,si,j,i,prev_sgn)/mdet;
//        }
//    }

//    return res;
//}

//// deprecated; in: L - one, U - zero
//void decomposition_lu(const matxf& mat, matxf& l, matxf& u) {
//    int n = mat.n;
//    assert(n == mat.m);
//    matxf a(mat);
//    for (int k = 0; k < n; ++k) {
//        u(k,k) = a(k,k);
//        for (int i = k+1; i < n; ++i) {
//            u(k,i) = a(k,i);
//            l(i,k) = a(i,k)/u(k,k);
//            for (int j = k+1; j < n; ++j) {
//                a(i,j) -= l(i,k)*u(k,j);
//            }
//        }
//    }
//}

//// deprecated
//float determinant_lu(const matxf& mat) {
//    int n = mat.n;
//    assert(n == mat.m);
//    float det = 1.0f;
//    matxf a(mat);
//    for (int k = 0; k < n; ++k) {
//        for (int i = k+1; i < n; ++i) {
//            a(i,k) /= a(k,k);
//            for (int j = k+1; j < n; ++j) {
//                a(i,j) -= a(i,k)*a(k,j);
//            }
//        }
//        det *= a(k,k);
//    }
//    return det;
//}

//// deprecated
//void decomposition_lup(const matxf& mat, matxf& l, matxf& u, matxf& t) {
//    matxf a(mat);
//    int* tr = new int[a.n];
//    decomposition_lup(a,tr);
//    l.one();
//    u.zero();
//    t.zero();
//    for (uint i = 0; i < a.n; ++i) {
//        u(i,i) = a(i,i);
//        t(i,tr[i]) = 1.0f;
//        for (uint j = i+1; j < a.n; ++j) {
//            u(i,j) = a(i,j);
//            l(j,i) = a(j,i);
//        }
//    }
//    delete[] tr;
//}

} // namespace bacchus
