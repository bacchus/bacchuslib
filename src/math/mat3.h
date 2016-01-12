#pragma once

#include <iostream>

#include "mdefs.h"
#include "vec3.h"

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
struct mat3 {
    T data[3][3];

    int dim() const { return 3; }

    mat3() {
        zero();
    }

    template<typename U>
    mat3(const mat3<U>& m) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] = m.data[i][j];
            }
        }
    }

    explicit mat3(const T* rawp) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] = rawp[i*dim()+j];
            }
        }
    }

//    explicit mat3(const std::vector<T>& vec) {
//        assert(vec.size() == dim()*dim());
//        for (int i = 0; i < dim(); ++i) {
//            for (int j = 0; j < dim(); ++j) {
//                data[i][j] = vec[i*dim()+j];
//            }
//        }
//    }

    mat3(const T& x0, const T& y0, const T& z0,
         const T& x1, const T& y1, const T& z1,
         const T& x2, const T& y2, const T& z2)
    {
        data[0][0] = x0; data[0][1] = y0; data[0][2] = z0;
        data[1][0] = x1; data[1][1] = y1; data[1][2] = z1;
        data[2][0] = x2; data[2][1] = y2; data[2][2] = z2;
    }

    mat3(const vec3<T>& v0,
         const vec3<T>& v1,
         const vec3<T>& v2)
    {
        data[0][0] = v0.x; data[0][1] = v0.y; data[0][2] = v0.z;
        data[1][0] = v1.x; data[1][1] = v1.y; data[1][2] = v1.z;
        data[2][0] = v2.x; data[2][1] = v2.y; data[2][2] = v2.z;
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

    void set_row(int i, const vec3<T>& v) {
        for (int j = 0; j < dim(); ++j) {
            data[i][j] = v[j];
        }
    }

    void set_col(int i, const vec3<T>& v) {
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

    mat3<T>& operator=(const mat3<T>& m) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] = m.data[i][j];
            }
        }
        return *this;
    }

    mat3<T>& operator+=(const mat3<T>& m) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] += m.data[i][j];
            }
        }
        return *this;
    }

    mat3<T>& operator-=(const mat3<T>& m) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] -= m.data[i][j];
            }
        }
        return *this;
    }

    mat3<T>& operator*=(const T& c) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] *= c;
            }
        }
        return *this;
    }

//    mat3<T>& operator*=(const mat3<T>& m) {
//        mat3<T> tmp;
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

    mat3<T>& operator/=(const T& c) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] /= c;
            }
        }
        return *this;
    }

    mat3<T> inverse() const {
        mat3<T> adop(
                + (data[1][1]*data[2][2] - data[1][2]*data[2][1]),	// 0 0
                - (data[0][1]*data[2][2] - data[0][2]*data[2][1]),	// 1 0
                + (data[0][1]*data[1][2] - data[0][2]*data[1][1]),	// 2 0

                - (data[1][0]*data[2][2] - data[1][2]*data[2][0]),	// 0 1
                + (data[0][0]*data[2][2] - data[0][2]*data[2][0]),	// 1 1
                - (data[0][0]*data[1][2] - data[0][2]*data[1][0]),	// 2 1

                + (data[1][0]*data[2][1] - data[1][1]*data[2][0]),	// 0 2
                - (data[0][0]*data[2][1] - data[0][1]*data[2][0]),	// 1 2
                + (data[0][0]*data[1][1] - data[0][1]*data[1][0])	// 2 2
                );

        adop /= determinant();
        return adop;
    }

    T determinant() const {
        T determ =
                + data[0][0]*(data[1][1]*data[2][2] - data[2][1]*data[1][2])
                - data[1][0]*(data[0][1]*data[2][2] - data[0][2]*data[2][1])
                + data[2][0]*(data[1][2]*data[0][1] - data[1][1]*data[0][2]);
        return determ;
    }

    void transpose() {
        std::swap(data[1][0], data[0][1]);
        std::swap(data[2][0], data[0][2]);
        std::swap(data[1][2], data[2][1]);
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
inline bool operator==(const mat3<T>& m1, const mat3<T>& m2) {
    const int dim = m1.dim();
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            if (!eq(m1[i][j], m2[i][j]))
                return false;
        }
    }
    return true;
}

template <typename T>
inline mat3<T> transpose(const mat3<T>& m1) {
    return mat3<T>(m1[0][0], m1[1][0], m1[2][0],
                   m1[0][1], m1[1][1], m1[2][1],
                   m1[0][2], m1[1][2], m1[2][2]);
}

template <typename T>
inline mat3<T> operator+(const mat3<T>& m1, const mat3<T>& m2) {
    return mat3<T>(m1[0][0] + m2[0][0], m1[0][1] + m2[0][1], m1[0][2] + m2[0][2],
                   m1[1][0] + m2[1][0], m1[1][1] + m2[1][1], m1[1][2] + m2[1][2],
                   m1[2][0] + m2[2][0], m1[2][1] + m2[2][1], m1[2][2] + m2[2][2]);
}

template <typename T>
inline mat3<T> operator-(const mat3<T>& m1, const mat3<T>& m2) {
    return mat3<T>(m1[0][0] - m2[0][0], m1[0][1] - m2[0][1], m1[0][2] - m2[0][2],
                   m1[1][0] - m2[1][0], m1[1][1] - m2[1][1], m1[1][2] - m2[1][2],
                   m1[2][0] - m2[2][0], m1[2][1] - m2[2][1], m1[2][2] - m2[2][2]);
}

template <typename T>
inline vec3<T> operator*(const mat3<T>& m1, const vec3<T>& v1) {
    return vec3<T>(m1[0][0]*v1.x + m1[0][1]*v1.y + m1[0][2]*v1.z,
                   m1[1][0]*v1.x + m1[1][1]*v1.y + m1[1][2]*v1.z,
                   m1[2][0]*v1.x + m1[2][1]*v1.y + m1[2][2]*v1.z);
}

template <typename T>
inline vec3<T> operator*(const vec3<T>& v1, const mat3<T>& m1) {
    return vec3<T>(m1[0][0]*v1.x + m1[1][0]*v1.y + m1[2][0]*v1.z,
                   m1[0][1]*v1.x + m1[1][1]*v1.y + m1[2][1]*v1.z,
                   m1[0][2]*v1.x + m1[1][2]*v1.y + m1[2][2]*v1.z );
}

template <typename T>
inline mat3<T> operator*(const mat3<T>& m1, const mat3<T>& m2) {
    return mat3<T>(
            m1[0][0]*m2[0][0] + m1[0][1]*m2[1][0] + m1[0][2]*m2[2][0],
            m1[0][0]*m2[0][1] + m1[0][1]*m2[1][1] + m1[0][2]*m2[2][1],
            m1[0][0]*m2[0][2] + m1[0][1]*m2[1][2] + m1[0][2]*m2[2][2],

            m1[1][0]*m2[0][0] + m1[1][1]*m2[1][0] + m1[1][2]*m2[2][0],
            m1[1][0]*m2[0][1] + m1[1][1]*m2[1][1] + m1[1][2]*m2[2][1],
            m1[1][0]*m2[0][2] + m1[1][1]*m2[1][2] + m1[1][2]*m2[2][2],

            m1[2][0]*m2[0][0] + m1[2][1]*m2[1][0] + m1[2][2]*m2[2][0],
            m1[2][0]*m2[0][1] + m1[2][1]*m2[1][1] + m1[2][2]*m2[2][1],
            m1[2][0]*m2[0][2] + m1[2][1]*m2[1][2] + m1[2][2]*m2[2][2]);
}

template <typename T>
inline mat3<T> operator-(const mat3<T>& m1) {
    return vec3<T>(-m1[0][0], -m1[0][1], -m1[0][2],
                   -m1[1][0], -m1[1][1], -m1[1][2],
                   -m1[2][0], -m1[2][1], -m1[2][2]);
}

template<typename T>
inline std::ostream& operator <<(std::ostream& ostr, const mat3<T>& m) {
    for(int i=0; i < m.dim(); ++i) {
        for (int j = 0; j < m.dim(); ++j) {
            ostr<<m.data[i][j]<<" ";
        }
        ostr<<std::endl;
    }
    ostr<<std::endl;
    return ostr;
}

typedef mat3<float> mat3f;

} // namespace bacchus
