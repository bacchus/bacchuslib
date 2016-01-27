#pragma once

#include <cmath>

namespace bacchus {

class Matrix {
public:
    Matrix() {
        identity();
    }

    void identity() {
        for (int i=0 ; i < 16 ; i++) {
            m[i] = 0.0f;
        }
        for (int i = 0; i < 16; i += 5) {
            m[i] = 1.0f;
        }
    }

    void rotate(float a) {
        a *= G2RAD;
        float s = sin(a);
        float c = cos(a);
        float m0 = m[0];
        float m1 = m[1];
        float m4 = m[4];
        float m5 = m[5];
        m[0] = m0 * c + m4 * s;
        m[1] = m1 * c + m5 * s;
        m[4] = -m0 * s + m4 * c;
        m[5] = -m1 * s + m5 * c;
    }

    void skewX(float a) {
        float t = tan(a * G2RAD);
        m[4] += -m[0] * t;
        m[5] += -m[1] * t;
    }

    void skewY(float a) {
        float t = tan(a * G2RAD);
        m[0] += m[4] * t;
        m[1] += m[5] * t;
    }

    void scale(float x, float y) {
        m[0] *= x;
        m[1] *= x;
        m[2] *= x;
        m[3] *= x;
        m[4] *= y;
        m[5] *= y;
        m[6] *= y;
        m[7] *= y;
    }

    void translate(float x, float y) {
        m[12] += m[0] * x + m[4] * y;
        m[13] += m[1] * x + m[5] * y;
    }

    float* data() {
        return m;
    }

private:
    static const float G2RAD = 0.01745329252f;
    float m[16];
};

} // bacchus
