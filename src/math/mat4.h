#pragma once

#include <iostream>

#include "mdefs.h"
#include "vec4.h"
#include "mat3.h"

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
struct mat4 {
    T data[4][4];

    int dim() const { return 4; }

    mat4() {
        zero();
    }

    template<typename U>
    mat4(const mat4<U>& m) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] = m.data[i][j];
            }
        }
    }

    explicit mat4(const T* rawp) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] = rawp[i*dim()+j];
            }
        }
    }

    mat4(const T& x0, const T& y0, const T& z0, const T& w0,
         const T& x1, const T& y1, const T& z1, const T& w1,
         const T& x2, const T& y2, const T& z2, const T& w2,
         const T& x3, const T& y3, const T& z3, const T& w3 )
    {
        data[0][0] = x0; data[0][1] = y0; data[0][2] = z0; data[0][3] = w0;
        data[1][0] = x1; data[1][1] = y1; data[1][2] = z1; data[1][3] = w1;
        data[2][0] = x2; data[2][1] = y2; data[2][2] = z2; data[2][3] = w2;
        data[3][0] = x3; data[3][1] = y3; data[3][2] = z3; data[3][3] = w3;
    }

    mat4(const vec4<T>& v0, const vec4<T>& v1, const vec4<T>& v2, const vec4<T>& v3) {
        data[0][0] = v0.x; data[0][1] = v0.y; data[0][2] = v0.z; data[0][3] = v0.w;
        data[1][0] = v1.x; data[1][1] = v1.y; data[1][2] = v1.z; data[1][3] = v1.w;
        data[2][0] = v2.x; data[2][1] = v2.y; data[2][2] = v2.z; data[2][3] = v2.w;
        data[3][0] = v3.x; data[3][1] = v3.y; data[3][2] = v3.z; data[3][3] = v3.w;
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

    T* operator[](int i) {
        return data[i];
    }

    const T* operator[](int i) const {
        return data[i];
    }

    mat4<T>& operator=(const mat4<T>& m) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] = m.data[i][j];
            }
        }
        return *this;
    }

    bool operator==(const mat4<T>& m) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                if (fabs(data[i][j] - m.data[i][j]) > BCC_M_FLOAT_EPSILON)
                    return false;
            }
        }
        return true;
    }

    mat4<T>& operator+=(const mat4<T>& m) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] += m.data[i][j];
            }
        }
        return *this;
    }

    mat4<T>& operator-=(const mat4<T>& m) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] -= m.data[i][j];
            }
        }
        return *this;
    }

    mat4<T>& operator*=(const T& c) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] *= c;
            }
        }
        return *this;
    }

    mat4<T>& operator*=(const mat4<T>& m) {
        mat4<T> tmp;
        tmp.zero();
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                for (int k = 0; k < dim(); ++k) {
                    tmp[i][j] += data[i][k]*m.data[k][j];
                }
            }
        }
        *this = tmp;
        return *this;

    }

    mat4<T>& operator/=(const T& c) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                data[i][j] /= c;
            }
        }
        return *this;
    }

    mat4<T> inverse() const {
        T coef00 = data[2][2] * data[3][3] - data[3][2] * data[2][3];
        T coef02 = data[1][2] * data[3][3] - data[3][2] * data[1][3];
        T coef03 = data[1][2] * data[2][3] - data[2][2] * data[1][3];

        T coef04 = data[2][1] * data[3][3] - data[3][1] * data[2][3];
        T coef06 = data[1][1] * data[3][3] - data[3][1] * data[1][3];
        T coef07 = data[1][1] * data[2][3] - data[2][1] * data[1][3];

        T coef08 = data[2][1] * data[3][2] - data[3][1] * data[2][2];
        T coef10 = data[1][1] * data[3][2] - data[3][1] * data[1][2];
        T coef11 = data[1][1] * data[2][2] - data[2][1] * data[1][2];

        T coef12 = data[2][0] * data[3][3] - data[3][0] * data[2][3];
        T coef14 = data[1][0] * data[3][3] - data[3][0] * data[1][3];
        T coef15 = data[1][0] * data[2][3] - data[2][0] * data[1][3];

        T coef16 = data[2][0] * data[3][2] - data[3][0] * data[2][2];
        T coef18 = data[1][0] * data[3][2] - data[3][0] * data[1][2];
        T coef19 = data[1][0] * data[2][2] - data[2][0] * data[1][2];

        T coef20 = data[2][0] * data[3][1] - data[3][0] * data[2][1];
        T coef22 = data[1][0] * data[3][1] - data[3][0] * data[1][1];
        T coef23 = data[1][0] * data[2][1] - data[2][0] * data[1][1];

        const vec4<T> signa(+1, -1, +1, -1);
        const vec4<T> signb(-1, +1, -1, +1);

        vec4<T> fac0(coef00, coef00, coef02, coef03);
        vec4<T> fac1(coef04, coef04, coef06, coef07);
        vec4<T> fac2(coef08, coef08, coef10, coef11);
        vec4<T> fac3(coef12, coef12, coef14, coef15);
        vec4<T> fac4(coef16, coef16, coef18, coef19);
        vec4<T> fac5(coef20, coef20, coef22, coef23);

        vec4<T> vec0(data[1][0], data[0][0], data[0][0], data[0][0]);
        vec4<T> vec1(data[1][1], data[0][1], data[0][1], data[0][1]);
        vec4<T> vec2(data[1][2], data[0][2], data[0][2], data[0][2]);
        vec4<T> vec3(data[1][3], data[0][3], data[0][3], data[0][3]);

        mat4<T> inv(
                    signa * (vec1 * fac0 - vec2 * fac1 + vec3 * fac2),
                    signb * (vec0 * fac0 - vec2 * fac3 + vec3 * fac4),
                    signa * (vec0 * fac1 - vec1 * fac3 + vec3 * fac5),
                    signb * (vec0 * fac2 - vec1 * fac4 + vec2 * fac5) );

        inv /= dot(vec4<T>(data[0]), vec4<T>(inv[0][0], inv[1][0], inv[2][0], inv[3][0]));
        return inv;
    }

    mat3<T> minor_mat(int y, int x) const {
        mat3<T> res; res.zero();
        int dy = 0;
        for (int i = 0; i < dim(); ++i) {
            if (i==y) { ++i; dy = 1; }
            int dx = 0;
            for (int j = 0; j < dim(); ++j) {
                if (j==x) { ++j; dx = 1; }
                res[i-dy][j-dx] = data[i][j];
            }
        }
        return res;
    }

    T determinant() const {
        T determ =
                + data[0][0]*minor_mat(0,0).determinant()
                - data[1][0]*minor_mat(1,0).determinant()
                + data[2][0]*minor_mat(2,0).determinant()
                - data[3][0]*minor_mat(3,0).determinant();
        return determ;
    }

    mat4<T>& transpose() {
        std::swap(data[1][0], data[0][1]);
        std::swap(data[2][0], data[0][2]);
        std::swap(data[2][1], data[1][2]);
        std::swap(data[3][0], data[0][3]);
        std::swap(data[3][1], data[1][3]);
        std::swap(data[3][2], data[2][3]);
        return *this;
//        return mat4<T>(data[0][0], data[1][0], data[2][0],
//                data[0][1], data[1][1], data[2][1],
//                data[0][2], data[1][2], data[2][2]);
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
inline mat4<T>& transpose(const mat4<T>& m) {
        return mat4<T>(
                m[0][0], m[1][0], m[2][0], m[3][0],
                m[0][1], m[1][1], m[2][1], m[3][1],
                m[0][2], m[1][2], m[2][2], m[3][2],
                m[0][3], m[1][3], m[2][3], m[3][3] );
}

template <typename T>
inline mat4<T> operator+(const mat4<T>& m1, const mat4<T>& m2) {
    mat4<T> m_res(m1);
    m_res += m2;
    return m_res;
}

template <typename T>
inline mat4<T> operator-(const mat4<T>& m1, const mat4<T>& m2) {
    mat4<T> m_res(m1);
    m_res -= m2;
    return m_res;
}

template <typename T>
inline vec4<T> operator*(const mat4<T>& m, const vec4<T>& v) {
    return vec4<T>(
            m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3]*v.w,
            m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + m[1][3]*v.w,
            m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + m[2][3]*v.w,
            m[3][0]*v.x + m[3][1]*v.y + m[3][2]*v.z + m[3][3]*v.w );
}

template <typename T>
inline vec4<T> operator*(const vec4<T>& v, const mat4<T>& m) {
    return vec4<T>(
            m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z + m[3][0]*v.w,
            m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z + m[3][1]*v.w,
            m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z + m[3][2]*v.w,
            m[0][3]*v.x + m[1][3]*v.y + m[2][3]*v.z + m[3][3]*v.w );
}

template <typename T>
inline mat4<T> operator*(const mat4<T>& m1, const mat4<T>& m2) {
    mat4<T> res(
            m1[0][0]*m2[0][0] + m1[0][1]*m2[1][0] + m1[0][2]*m2[2][0] + m1[0][3]*m2[3][0],
            m1[0][0]*m2[0][1] + m1[0][1]*m2[1][1] + m1[0][2]*m2[2][1] + m1[0][3]*m2[3][1],
            m1[0][0]*m2[0][2] + m1[0][1]*m2[1][2] + m1[0][2]*m2[2][2] + m1[0][3]*m2[3][2],
            m1[0][0]*m2[0][3] + m1[0][1]*m2[1][3] + m1[0][2]*m2[2][3] + m1[0][3]*m2[3][3],

            m1[1][0]*m2[0][0] + m1[1][1]*m2[1][0] + m1[1][2]*m2[2][0] + m1[1][3]*m2[3][0],
            m1[1][0]*m2[0][1] + m1[1][1]*m2[1][1] + m1[1][2]*m2[2][1] + m1[1][3]*m2[3][1],
            m1[1][0]*m2[0][2] + m1[1][1]*m2[1][2] + m1[1][2]*m2[2][2] + m1[1][3]*m2[3][2],
            m1[1][0]*m2[0][3] + m1[1][1]*m2[1][3] + m1[1][2]*m2[2][3] + m1[1][3]*m2[3][3],

            m1[2][0]*m2[0][0] + m1[2][1]*m2[1][0] + m1[2][2]*m2[2][0] + m1[2][3]*m2[3][0],
            m1[2][0]*m2[0][1] + m1[2][1]*m2[1][1] + m1[2][2]*m2[2][1] + m1[2][3]*m2[3][1],
            m1[2][0]*m2[0][2] + m1[2][1]*m2[1][2] + m1[2][2]*m2[2][2] + m1[2][3]*m2[3][2],
            m1[2][0]*m2[0][3] + m1[2][1]*m2[1][3] + m1[2][2]*m2[2][3] + m1[2][3]*m2[3][3],

            m1[3][0]*m2[0][0] + m1[3][1]*m2[1][0] + m1[3][2]*m2[2][0] + m1[3][3]*m2[3][0],
            m1[3][0]*m2[0][1] + m1[3][1]*m2[1][1] + m1[3][2]*m2[2][1] + m1[3][3]*m2[3][1],
            m1[3][0]*m2[0][2] + m1[3][1]*m2[1][2] + m1[3][2]*m2[2][2] + m1[3][3]*m2[3][2],
            m1[3][0]*m2[0][3] + m1[3][1]*m2[1][3] + m1[3][2]*m2[2][3] + m1[3][3]*m2[3][3] );
    return res;
}

template <typename T>
inline mat4<T> operator-(const mat4<T>& m) {
    mat4<T> res(m);
    res *= -1;
    return res;
}

template<typename T>
inline std::ostream& operator <<(std::ostream& ostr, const mat4<T>& m) {
    for(int i=0; i < m.dim(); ++i) {
        for (int j = 0; j < m.dim(); ++j) {
            ostr<<m.data[i][j]<<" ";
        }
        ostr<<std::endl;
    }
    ostr<<std::endl;
    return ostr;
}

typedef mat4<float> mat4f;

} // namespace bacchus
