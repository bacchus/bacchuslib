#pragma once

#include <iostream>

#include "mdefs.h"
#include "vec2.h"

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

template <typename T>
struct mat2 {
    T data[2][2];

    int dim() const { return 2; }

    mat2() {
        zero();
    }

    template<typename U>
    mat2(const mat2<U>& m) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] = m.data[i][j];
            }
        }
    }

    explicit mat2(const T* rawp) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] = rawp[i*dim()+j];
            }
        }
    }

    mat2(const T& x0, const T& y0,
         const T& x1, const T& y1)
    {
        data[0][0] = x0; data[0][1] = y0;
        data[1][0] = x1; data[1][1] = y1;
    }

    void one() {
        zero();
        for (int i = 0; i < dim(); ++i) {
            data[i][i] = T(1);
        }
    }

    void zero() {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] = T(0);
            }
        }
    }

    void set_row(int i, const vec2<T>& v) {
        for (int j = 0; j < dim(); ++j) {
            data[i][j] = v[j];
        }
    }

    void set_col(int i, const vec2<T>& v) {
        for (int j = 0; j < dim(); ++j) {
            data[j][i] = v[j];
        }
    }

    T* operator[](int i) {
        return data[i];
    }

    const T* operator[](int i) const {
        return data[i];
    }

    mat2<T>& operator=(const mat2<T>& m) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] = m.data[i][j];
            }
        }
        return *this;
    }

    bool operator==(const mat2<T>& m) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                if (!eq(data[i][j], m.data[i][j]))
                    return false;
            }
        }
        return true;
    }

    mat2<T>& operator+=(const mat2<T>& m) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] += m.data[i][j];
            }
        }
        return *this;
    }

    mat2<T>& operator-=(const mat2<T>& m) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] -= m.data[i][j];
            }
        }
        return *this;
    }

    mat2<T>& operator*=(const T& c) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] *= c;
            }
        }
        return *this;
    }

//    mat2<T>& operator*=(const mat2<T>& m) {
//        mat2<T> tmp;
//        tmp.zero();
//        for (int i = 0; i < dim(); ++i) {
//            for (int j = 0; j < dim(); ++j) {
//                for (int k = 0; k < dim(); ++k) {
//                    tmp[i][j] += data[i][k]*m.data[k][j];
//                }
//            }
//        }
//        *this = tmp;
//        return *this;
//    }

    mat2<T>& operator/=(const T& c) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] /= c;
            }
        }
        return *this;
    }

    mat2<T> inverse() const {
        mat2<T> adop(data[1][1], -data[0][1],
                    -data[1][0],  data[0][0]);
        adop /= determinant();
        return adop;
    }

    T determinant() const {
        return data[0][0]*data[1][1] - data[0][1]*data[1][0];
    }

    void transpose() {
        std::swap(data[1][0], data[0][1]);
    }

    void test_print() const {
        for(int i=0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                std::cout<<data[i][j]<<" ";
            }
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }
};

template <typename T>
inline mat2<T>& transpose(const mat2<T>& m) {
    return mat2<T>(m[0][0], m[1][0],
                   m[0][1], m[1][1]);
}

template <typename T>
inline mat2<T> operator+(const mat2<T>& m1, const mat2<T>& m2) {
    return mat2<T>(m1[0][0] + m2[0][0], m1[0][1] + m2[0][1],
                   m1[1][0] + m2[1][0], m1[1][1] + m2[1][1]);
}

template <typename T>
inline mat2<T> operator-(const mat2<T>& m1, const mat2<T>& m2) {
    return mat2<T>(m1[0][0] - m2[0][0], m1[0][1] - m2[0][1],
                   m1[1][0] - m2[1][0], m1[1][1] - m2[1][1]);
}

template <typename T>
inline vec2<T> operator*(const mat2<T>& m, const vec2<T>& v) {
    return vec2<T>(m[0][0]*v.x + m[0][1]*v.y,
                   m[1][0]*v.x + m[1][1]*v.y);
}

template <typename T>
inline vec2<T> operator*(const vec2<T>& v, const mat2<T>& m) {
    return vec2<T>(m[0][0]*v.x + m[1][0]*v.y,
                   m[0][1]*v.x + m[1][1]*v.y);
}

template <typename T>
inline mat2<T> operator*(const mat2<T>& m1, const mat2<T>& m2) {
    return mat2<T>(m1[0][0]*m2[0][0] + m1[0][1]*m2[1][0],
                   m1[0][0]*m2[0][1] + m1[0][1]*m2[1][1],

                   m1[1][0]*m2[0][0] + m1[1][1]*m2[1][0],
                   m1[1][0]*m2[0][1] + m1[1][1]*m2[1][1]);
}

template <typename T>
inline mat2<T> operator-(const mat2<T>& m) {
    return mat2<T>(-m[0][0], -m[0][1],
                   -m[1][0], -m[1][1]);
}

template<typename T>
inline std::ostream& operator <<(std::ostream& ostr, const mat2<T>& m) {
    for(int i=0; i < m.dim(); ++i) {
        for (int j = 0; j < m.dim(); ++j) {
            ostr<<m.data[i][j]<<" ";
        }
        ostr<<std::endl;
    }
    ostr<<std::endl;
    return ostr;
}

typedef mat2<float> mat2f;

} // namespace bacchus
