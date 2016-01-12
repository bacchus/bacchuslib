#include "matalg.h"
#include "utils/except.h"

namespace bacchus {

float decomposition_lup(matxf &a, std::vector<int>& t) {
    uint n = a.n;
    assert(n == a.m);

    for (uint i = 0; i < n; ++i) {
        t[i] = i;
    }

    float det = 1.0f;
    for (uint k = 0; k < n; ++k) {

        float p = 0.0f;
        uint k1 = 0;
        for (uint i = k; i < n; ++i) {
            if (fabs(a(i,k)) > p) {
                p = fabs(a(i,k));
                k1 = i;
            }
        }
        if (p == 0) {
            std::cout<<"degenerate matrix"<<std::endl;
            throw MathError("degenerate matrix");
            //return 0;
        }
        if (k1 != k) {
            std::swap(t[k], t[k1]);
            for (uint i = 0; i < n; ++i) {
                std::swap(a(k,i), a(k1,i));
            }
            det =- det;
        }

        for (uint i = k+1; i < n; ++i) {
            a(i,k) /= a(k,k);
            for (uint j = k+1; j < n; ++j) {
                a(i,j) -= a(i,k)*a(k,j);
            }
        }
        det *= a(k,k);
    }
    return det;
}

float determinant(const matxf &mat) {
    uint n = mat.n;
    assert(n == mat.m);

    std::vector<int> t(n,0);
    matxf a(mat);
    return decomposition_lup(a,t);
}

matxf solve_lup(const matxf &a, const matxf &b, const std::vector<int>& t) {
    uint n = a.n;
    assert(n == a.m);
    assert(n == b.m);
    assert(1 == b.n);

    matxf x = matxf::zero(n);

    std::vector<float> y(n,0.f);
    for (uint i = 0; i < n; ++i) {
        float sum = 0;
        for (uint j = 0; j < i; ++j) {
            sum += a(i,j)*y[j];
        }
        y[i] = b[t[i]] - sum;
    }

    for (int i = n-1; i >= 0; --i) {
        float sum = 0;
        for (uint j = i+1; j < n; ++j) {
            sum += a(i,j)*x[j];
        }
        x[i] = (y[i] - sum)/a(i,i);
    }

    return x;
}

matxf solve_system(const matxf& a, const matxf& b) {
    uint n = a.n;
    assert(n == a.m);
    assert(n == b.m);
    assert(1 == b.n);

    std::vector<int> t(n,0);
    matxf da(a);
    decomposition_lup(da,t);
    return solve_lup(da, b, t);
}

matxf inverse(const matxf &mat) {
    uint n = mat.n;
    assert(n == mat.m);

    matxf inv(n,n);
    matxf x(n);
    for (uint i = 0; i < n; ++i) {
        x = matxf::zero(n);
        x[i] = 1.0f;
        matxf row = solve_system(mat, x);
        inv.set_row(i, row);
    }
    return transpose(inv);
}

//matxf transpose(const matxf &mat) {
//    matxf res(mat.n, mat.m);
//    for (uint i = 0; i < res.m; ++i) {
//        for (uint j = 0; j < res.n; ++j) {
//            res(i,j) = mat(j,i);
//        }
//    }
//    return res;
//}

//float inprod(const matxf &mat1, const matxf &mat2) {
//    uint m = mat1.m;
//    assert(mat1.n == 1);
//    assert(mat2.n == 1);
//    assert(mat2.m == m);
//    return std::inner_product(mat1.begin(), mat1.end(), mat2.begin(), 0.f);
//}

// L(i,i) = sqrt( A(i,i) - sum(k=1:i-1, sq(L(i,k))) )
// j<i: L(i,j) = (A(i,j) - sum(k=1:i-1, L(i,k)*L(j,k)))/L(j,j)
matxf decomposition_cholesky(const matxf &a) {
    uint m = a.m;
    uint n = a.n;
    assert(m == n);
    matxf L(m,n);
    for (uint i = 0; i < n; ++i) {
        for (uint j = 0; j < i; ++j) {
            float sum = 0;
            for (uint k = 0; k < j; ++k) {
                sum += L(i,k)*L(j,k);
            }
            L(i,j) = (a(i,j) - sum)/L(j,j);
        }
        float sum = 0;
        for (uint k = 0; k < i; ++k) {
            sum += sq(L(i,k));
        }
        L(i,i) = std::sqrt(a(i,i) - sum);
    }
    return std::move(L);
}

matxf decomposition_ldl(const matxf &a, matxf &d) {
    uint m = a.m;
    uint n = a.n;
    assert(m == n);
    matxf L(m,n);
    d.resize(m,n);
    for (uint i = 0; i < n; ++i) {
        for (uint j = 0; j < i; ++j) {
            float sum = 0;
            for (uint k = 0; k < j; ++k) {
                sum += L(i,k)*L(j,k)*d(k,k);
            }
            L(i,j) = (a(i,j) - sum)/d(j,j);
        }
        float sum = 0;
        for (uint k = 0; k < i; ++k) {
            sum += sq(L(i,k))*d(k,k);
        }
        d(i,i) = a(i,i) - sum;
        L(i,i) = 1.f;
    }
    return std::move(L);
}

matxf cholupdate(const matxf &L, const matxf &v, char op) {
    matxf res(L);
    matxf x(v);
    assert(x.n == 1);
    uint n = x.m;
    if (op == '+') {
        for (uint k = 0; k < n; ++k) {
            float r = std::sqrt(sq(res(k,k)) + sq(x[k]));
            float c = r/res(k,k);
            float s = x[k]/res(k,k);
            res(k,k) = r;
            for (uint i = k+1; i < n; ++i) {
                res(i,k) = (res(i,k) + s*x[i])/c;
                x[i] = c*x[i] - s*res(i,k);
            }
        }
    } else if (op == '-') {
        for (uint k = 0; k < n; ++k) {
            float r = std::sqrt(sq(res(k,k)) - sq(x[k]));
            float c = r/res(k,k);
            float s = x[k]/res(k,k);
            res(k,k) = r;
            for (uint i = k+1; i < n; ++i) {
                res(i,k) = (res(i,k) - s*x[i])/c;
                x[i] = c*x[i] - s*res(i,k);
            }
        }
    } else {
        throw LogicError("cholupdate wrong op");
    }

    return res;
}

} // namespace bacchus
