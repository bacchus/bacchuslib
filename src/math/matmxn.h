#pragma once

#include <cassert>
#include <cmath>
#include <cstdlib>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <functional>

#include "mdefs.h"
#include "vec2.h"

#include "utils/except.h"

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

template<typename T>
struct matx {
    uint m,n;
    std::vector<T> data;

    typedef typename std::vector<T>::iterator mat_iter;
    typedef typename std::vector<T>::const_iterator mat_citer;
    typedef typename std::vector<T>::reverse_iterator mat_riter;
    typedef typename std::vector<T>::const_reverse_iterator mat_criter;

    mat_iter begin() { return data.begin(); }
    mat_citer begin() const { return data.begin(); }
    mat_iter end() { return data.end(); }
    mat_citer end() const { return data.end(); }

    mat_riter rbegin() { return data.rbegin(); }
    mat_criter rbegin() const { return data.rbegin(); }
    mat_riter rend() { return data.rend(); }
    mat_criter rend() const { return data.rend(); }

    class row_iter {
        uint row;
    public:
        row_iter(uint row): row(row) {}
    };

    class col_iter {
        uint col;
    public:
        col_iter(uint col): col(col) {}
    };

    uint height() const { return m; }
    uint width() const { return n; }
    uint size() const { return m*n; }

    matx(): m(0), n(0) {}

    matx(uint h, uint w, const T* idata=nullptr): m(h), n(w) {
        assert(m>0 && n>0);
        if (idata) {
            data.assign(idata, idata + size());
        } else {
            data.resize(size(), 0.f);
        }
    }

    explicit matx(uint h, const T* idata=nullptr): m(h), n(1) {
        assert(m>0 && n>0);
        if (idata) {
            data.assign(idata, idata + size());
        } else {
            data.resize(size(), 0.f);
        }
    }

    matx(uint h, uint w, const std::vector<T>& vdata): m(h), n(w) {
        assert(m>0 && n>0);
        data.assign(vdata.begin(), vdata.end());
    }

    explicit matx(const std::vector<T>& vdata): m(vdata.size()), n(1), data(vdata) {
        assert(m>0 && n>0);
    }

    matx(const std::initializer_list<T>& ilist): m(ilist.size()), n(1), data(ilist) {
        assert(m>0 && n>0);
    }

    matx(const std::initializer_list<std::initializer_list<T>>& dilist) {
        assert(dilist.size() > 0);
        int i = 0;
        std::vector<int> j;
        for (const auto& row: dilist) {
            j.push_back(0);
            for (const auto& x: row) {
                ++j[i];
                data.push_back(x);
            }
            if (i>0 && j[i]!=j[i-1]) {
                throw LogicError("matx({{}}) initializer lists has different sizes");
            }
            ++i;
        }
        m = i;
        n = j[0];
    }

    explicit matx(const std::vector< std::vector<T> >& vdata)
        : m(vdata.size())
        , n(vdata[0].size())
    {
        assert(m>0 && n>0);
        for (auto& row: vdata) {
            data.insert(data.end(), row.begin(), row.end());
        }
    }

    matx(const matx& mat): m(mat.m), n(mat.n), data(mat.data) {
        assert(m>0 && n>0);
    }

    template<typename U>
    matx(const matx<U>& matu): m(matu.m), n(matu.n), data(matu.data.begin(), matu.data.end()) {
        //assert(false);
        assert(m>0 && n>0);
    }

    matx(const mat_citer& first, const mat_citer& last): m(uint(last-first)), n(1), data(first,last) {
        assert(m>0 && n>0);
    }

    matx& operator=(const matx& mat) {
        if (this == &mat)// no copy self
            return *this;
        assert(mat.m > 0 && mat.n > 0);// no copy zero data
        assert(mat.m == m && mat.n == n);// no copy different sizes
        data = mat.data;
        m = mat.m; // same
        n = mat.n; // same
        return *this;
    }

    matx(matx&& mat)
        : m(mat.m)
        , n(mat.n)
        , data(std::move(mat.data))
    {
        assert(m>0 && n>0);
    }

    matx& operator=(matx&& mat) {
        m = mat.m;
        n = mat.n;
        assert(m>0 && n>0);
        //std::swap(data, mat.data);
        data = std::move(mat.data);
        return *this;
    }

    ~matx() {}

    static matx one(int h, int w=1) {
        matx res(h,w);
        for (uint i = 0; i < res.size(); i+=(w+1)) {
            res.data[i] = T(1);
        }
        return std::move(res);
    }

    static matx zero(int h, int w=1) {
        return std::move(matx(h,w));
    }

    static matx fill(T c, int h, int w=1) {
        matx res(h,w);
        std::fill(res.data.begin(), res.data.end(), c);
        return std::move(res);
    }

    void mutate(std::function<T(T)> func) {
        std::transform(data.begin(), data.end(), data.begin(), func);
        //for (T& x: data) func(x);
    }

    matx transform(std::function<T(T)> func) const {
        matx<T> res(m,n);
        std::transform(data.begin(), data.end(), res.data.begin(), func);
        return std::move(res);
    }

    void reshape(uint h, uint w) {
        assert(h*w == size());
        m = h;
        n = w;
    }

    void resize(uint h, uint w = 1) {
        assert(h>0 && w>0);
        if (h == m && w == n)
            return;
        m = h;
        n = w;
        data.resize(size());
        //std::vector<T> new_data(size(), 0.f);
        //std::swap(data, new_data);
    }

    void clear() {
        data.clear();
        m = 0;
        n = 0;
    }

    void reserve(uint h) {
        data.reserve(h);
    }

    void append(const mat_citer& first, const mat_citer& last) {
        uint count = uint(last - first);
        data.insert(data.end(), first, last);
        n = 1;
        m += count;
    }

    void push_back(const T& val) {
        data.push_back(val);
        n = 1;
        m += 1;
    }

    void append_row(const mat_citer& first, const mat_citer& last) {
        uint count = uint(last - first);
        assert(count == n);
        data.insert(data.end(), first, last);
        ++m;
    }

    void append_row(const matx& mat1) {
        assert(mat1.n == 1);
        assert(mat1.m == n);
        data.insert(data.end(), mat1.begin(), mat1.end());
        ++m;
    }


    operator T() const {
        assert(size() == 1);
        return data[0];
    }

    T& operator[](uint i) {
        assert(n == 1);// use for mat as vector only
        assert(i<m);
        return data[i];
    }

    T operator[](uint i) const {
        assert(n == 1);// use for mat as vector only
        assert(i<m);
        return data[i];
    }

    void set_row(uint rown, const matx& row) {
        assert(rown < m);
        assert(row.m == n);
        assert(row.n == 1);
        std::copy(row.data.begin(), row.data.end(), data.begin() + n*rown);
    }

    void set_col(uint coln, const matx& col) {
        assert(coln < n);
        assert(col.m == m);
        assert(col.n == 1);
        for (uint i = 0; i < m; ++i) {
            data[i*n+coln] = col[i];
        }
    }

    T& operator()(uint i, uint j) {
        assert(i<m && j<n);
        return data[i*n+j];
    }

    T operator()(uint i, uint j) const {
        assert(i<m && j<n);
        return data[i*n+j];
    }

    //matx&
    void operator+=(const matx& mat) {
        assert(m == mat.m && n == mat.n);
        for (uint i = 0; i < size(); ++i) {
            data[i] += mat.data[i];
        }
        //return *this;
    }

    //matx&
    void operator-=(const matx& mat) {
        assert(m == mat.m && n == mat.n);
        for (uint i = 0; i < size(); ++i) {
            data[i] -= mat.data[i];
        }
        //return *this;
    }

    //matx&
    void operator*=(const T& c) {
        for (uint i = 0; i < size(); ++i) {
            data[i] *= c;
        }
        //return *this;
    }

    //matx&
    void operator/=(const T& c) {
        for (uint i = 0; i < size(); ++i) {
            data[i] /= c;
        }
        //return *this;
    }

    void transpose() {
        assert(m == n);
        for (uint i = 0; i < m; ++i) {
            for (uint j = i+1; j < n; ++j) {
                std::swap(data[i*n+j], data[j*n+1]);
            }
        }
    }

    std::ostream& osprint(std::ostream& ostr) const {
        const uint precision = 8;
        ostr << "{" << std::endl;
        for(uint i=0; i < height(); ++i) {
            ostr << "{";
            for (uint j = 0; j < width(); ++j) {
                // setup format flags
                ostr << std::setw(precision + 3)
                     << std::setprecision(precision)
                     << std::setiosflags(std::ios_base::showpoint)
                     << std::setiosflags(std::ios_base::fixed)
                     << std::setiosflags(std::ios_base::skipws)
                        // round to zero
                     << (( !eq(data[i*n+j], T(0)) ) ? data[i*n+j] : T(0) ) << ", ";
            }
            ostr << "}," << std::endl;
        }
        return ostr << "};" << std::endl;
    }
};

template<typename T>
inline bool operator==(const matx<T>& mat1, const matx<T>& mat2) {
    assert(mat1.m == mat2.m && mat1.n == mat2.n);
    for (uint i = 0; i < mat1.size(); ++i) {
        if (!eq(mat1.data[i], mat2.data[i]))
            return false;
    }
    return true;
}

template<typename T>
inline matx<T> operator*(const matx<T>& m1, const matx<T>& m2) {
    assert(m1.n == m2.m);
    matx<T> res(m1.m, m2.n);
    for (uint i = 0; i < res.m; ++i) {
        for (uint j = 0; j < res.n; ++j) {
            T sum = 0.0f;
            for (uint k = 0; k < m1.n; ++k) {
                sum += m1(i,k)*m2(k,j);
            }
            res(i,j) = sum;
        }
    }
    return std::move(res);
}

template<typename T>
inline matx<T> operator-(const matx<T>& mat) {
    matx<T> res(mat);
    res *= -1;
    return std::move(res);
}

template<typename T>
inline matx<T> operator+(const matx<T>& m1, const matx<T>& m2) {
    matx<T> res(m1);
    res += m2;
    return std::move(res);
}

template<typename T>
inline matx<T> operator-(const matx<T>& m1, const matx<T>& m2) {
    matx<T> res(m1);
    res -= m2;
    return std::move(res);
}

template<typename T>
inline matx<T> operator*(const T& c, const matx<T>& m1) {
    matx<T> res(m1);
    for (uint i = 0; i < m1.size(); ++i) {
        res.data[i] *= c;
    }
    return std::move(res);
}

template<typename T>
inline matx<T> operator*(const matx<T>& m1, const T& c) {
    matx<T> res(m1);
    for (uint i = 0; i < m1.size(); ++i) {
        res.data[i] *= c;
    }
    return std::move(res);
}

template<typename T>
inline matx<T> operator/(const matx<T>& m1, const T& c) {
    matx<T> res(m1);
    for (uint i = 0; i < m1.size(); ++i) {
        res.data[i] /= c;
    }
    return std::move(res);
}

template<typename T>
inline std::ostream& operator <<(std::ostream& ostr, const matx<T>& mat) {
    return mat.osprint(ostr);
}

// by elements
template<typename T>
inline matx<T> mul(const matx<T>& m1, const matx<T>& m2) {
    assert(m1.m == m2.m);
    assert(m1.n == m2.n);
    matx<T> res(m1);
    for (uint i = 0; i < res.size(); ++i) {
        res.data[i] *= m2.data[i];
    }
    return std::move(res);
}

template<typename T>
inline matx<T> div(const matx<T>& m1, const matx<T>& m2) {
    assert(m1.m == m2.m);
    assert(m1.n == m2.n);
    matx<T> res(m1);
    for (uint i = 0; i < res.size(); ++i) {
        res.data[i] /= m2.data[i];
    }
    return std::move(res);
}

template<typename T>
inline T sum(const matx<T>& m1) {
    assert(m1.n == 1);
    return std::accumulate(m1.begin(), m1.end(), 0.f);
}

template<typename T>
inline vec2f minmax(const matx<T>& m1) {
    assert(m1.n == 1);
    vec2f minmax_val(m1[0], m1[0]);
    for (uint i = 1; i < m1.m; ++i) {
        minmax_val.x = std::min(minmax_val.x, m1[i]);
        minmax_val.y = std::max(minmax_val.y, m1[i]);
    }
    return minmax_val;
}

template<typename T>
inline matx<T> ext_col_mul(const matx<T>& m1, const matx<T>& m2) {
    assert(m1.m == m2.m);
    assert(m2.n == 1);
    matx<T> res(m1);
    for (uint j = 0; j < m1.n; ++j) {
        for (uint i = 0; i < m1.m; ++i) {
            res(i,j) *= m2[i];
        }
    }
    return std::move(res);
}

template<typename T>
inline matx<T> ext_col_div(const matx<T>& m1, const matx<T>& m2) {
    assert(m1.m == m2.m);
    assert(m2.n == 1);
    matx<T> res(m1);
    for (uint j = 0; j < m1.n; ++j) {
        for (uint i = 0; i < m1.m; ++i) {
            res(i,j) /= m2[i];
        }
    }
    return std::move(res);
}

template<typename T>
inline matx<T> ext_col_add(const matx<T>& m1, const matx<T>& m2) {
    assert(m1.m == m2.m);
    assert(m2.n == 1);
    matx<T> res(m1);
    for (uint j = 0; j < m1.n; ++j) {
        for (uint i = 0; i < m1.m; ++i) {
            res(i,j) += m2[i];
        }
    }
    return std::move(res);
}

template<typename T>
inline matx<T> ext_col_sub(const matx<T>& m1, const matx<T>& m2) {
    assert(m1.m == m2.m);
    assert(m2.n == 1);
    matx<T> res(m1);
    for (uint j = 0; j < m1.n; ++j) {
        for (uint i = 0; i < m1.m; ++i) {
            res(i,j) -= m2[i];
        }
    }
    return std::move(res);
}

template<typename T>
inline matx<T> ext_col_sum(const matx<T>& m1) {
    matx<T> sums = matx<T>::zero(m1.n);
    for (uint j = 0; j < m1.n; ++j) {
        for (uint i = 0; i < m1.m; ++i) {
            sums[j] += m1(i,j);
        }
    }
    return std::move(sums);
}

template<typename T>
inline matx<T> ext_col_get(const matx<T>& m1, uint col) {
    matx<T> res(m1.m);
    for (uint i = 0; i < m1.m; ++i) {
        res[i] = m1(i,col);
    }
    return std::move(res);
}

template<typename T>
inline matx<T> ext_row_mul(const matx<T>& m1, const matx<T>& m2) {
    assert(m1.n == m2.m);
    assert(m2.n == 1);
    matx<T> res(m1);
    for (uint j = 0; j < m1.n; ++j) {
        for (uint i = 0; i < m1.m; ++i) {
            res(i,j) *= m2[j];
        }
    }
    return std::move(res);
}

template<typename T>
inline matx<T> ext_row_div(const matx<T>& m1, const matx<T>& m2) {
    assert(m1.n == m2.m);
    assert(m2.n == 1);
    matx<T> res(m1);
    for (uint j = 0; j < m1.n; ++j) {
        for (uint i = 0; i < m1.m; ++i) {
            res(i,j) /= m2[j];
        }
    }
    return std::move(res);
}

template<typename T>
inline matx<T> ext_row_add(const matx<T>& m1, const matx<T>& m2) {
    assert(m1.n == m2.m);
    assert(m2.n == 1);
    matx<T> res(m1);
    for (uint j = 0; j < m1.n; ++j) {
        for (uint i = 0; i < m1.m; ++i) {
            res(i,j) += m2[j];
        }
    }
    return std::move(res);
}

template<typename T>
inline matx<T> ext_row_sub(const matx<T>& m1, const matx<T>& m2) {
    assert(m1.n == m2.m);
    assert(m2.n == 1);
    matx<T> res(m1);
    for (uint j = 0; j < m1.n; ++j) {
        for (uint i = 0; i < m1.m; ++i) {
            res(i,j) -= m2[j];
        }
    }
    return std::move(res);
}

template<typename T>
inline matx<T> ext_row_sum(const matx<T>& m1) {
    matx<T> sums = matx<T>::zero(m1.m);
    for (uint j = 0; j < m1.n; ++j) {
        for (uint i = 0; i < m1.m; ++i) {
            sums[i] += m1(i,j);
        }
    }
    return std::move(sums);
}

template<typename T>
inline matx<T> ext_row_get(const matx<T>& m1, uint row) {
    matx<T> res(m1.n);
    for (uint j = 0; j < m1.n; ++j) {
        res[j] = m1(row,j);
    }
    return std::move(res);
}

template<typename T>
inline matx<T> transpose(const matx<T> &mat) {
    matx<T> res(mat.n, mat.m);
    for (uint i = 0; i < res.m; ++i) {
        for (uint j = 0; j < res.n; ++j) {
            res(i,j) = mat(j,i);
        }
    }
    return res;
}

template<typename T>
inline T inprod(const matx<T> &mat1, const matx<T> &mat2) {
    uint m = mat1.m;
    assert(mat1.n == 1);
    assert(mat2.n == 1);
    assert(mat2.m == m);
    return std::inner_product(mat1.begin(), mat1.end(), mat2.begin(), T(0));
}

template<typename T>
inline matx<T> concat(const matx<T> &mat1, const matx<T> &mat2) {
    assert(mat1.n == 1);
    assert(mat2.n == 1);
    matx<T> res(mat1);
    res.append(mat2.begin(), mat2.end());
    return res;
}

template <typename T>
inline std::vector<int> argsort(const matx<T>& v) {
    std::vector<int> idx(v.size());
    std::iota(idx.begin(), idx.end(), 0);
    std::sort(idx.begin(), idx.end(), [&v](int i1, int i2) { return v[i1] < v[i2]; });
    return idx;
}

template<typename T>
inline matx<T> abs(const matx<T> &mat) {
    return mat.transform([](const T& x){ return std::fabs(x);});
}

typedef matx<double> matxd;
typedef matx<float> matxf;
typedef matx<cplx> matxcx;
typedef matx<cplxd> matxcxd;

} // namespace bacchus
