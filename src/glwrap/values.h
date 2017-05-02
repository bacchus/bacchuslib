#pragma once

#include <cmath>
#include <cassert>
#include <cstring>

#include "uniform.h"

#include "math/mdefs.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat4.h"

namespace bacchus {

class Value {
public:
    virtual ~Value() {}
    virtual void set_uniform(Uniform*) {}
};

class uvec1i: public Value {
public:
    int x;
    uvec1i(): x(0) {}
    explicit uvec1i(int v): x(v) {}
    virtual void set_uniform(Uniform* u) { u->value(*this); }
};

class uvec1f: public Value {
public:
    float x;
    uvec1f(): x(0) {}
    explicit uvec1f(float v): x(v) {}
    virtual void set_uniform(Uniform* u) { u->value(*this); }
};

// good try
class uvec2f: public Value, public vec2f {
public:
    //    float x,y;
    //    uvec2f(): x(0), y(0) {}
    //    uvec2f(float x, float y): x(x), y(y) {}
    //    explicit uvec2f(float v): x(v), y(v) {}
    virtual void set_uniform(Uniform* u) { u->value(*this); }
};

class uvec4f: public Value, public vec4f {
public:
    //    float x,y,z,w;
    //    uvec4f(): x(0), y(0), z(0), w(0) {}
    //    uvec4f(float x, float y, float z, float w): x(x), y(y), z(z), w(w) {}
    //    explicit uvec4f(float v): x(v), y(v), z(v), w(v) {}
    virtual void set_uniform(Uniform* u) { u->value(*this); }
};

class umat4f: public Value, public mat4f {
public:

    umat4f() {
        one();
    }

    umat4f(const mat4f& m)
        : mat4f(m)
    {}

    virtual void set_uniform(Uniform* u) { u->value(*this); }

    void set_scale(float x, float y) {
        data[0][0] = x;
        data[1][1] = y;
    }
};

/*
class umat4f: public Value {
private:
    float m[16];
public:
    umat4f() {
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
        a = deg2rad(a);
        float s = std::sin(a);
        float c = std::cos(a);
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
        float t = tan(deg2rad(a));
        m[4] += -m[0] * t;
        m[5] += -m[1] * t;
    }

    void skewY(float a) {
        float t = tan(deg2rad(a));
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

    void set_scale(float x, float y) {
        m[0] = x;
        //m[1] = x;
        //m[2] = x;
        //m[3] = x;
        //m[4] = y;
        m[5] = y;
        //m[6] = y;
        //m[7] = y;
    }

    void translate(float x, float y) {
        m[12] += m[0] * x + m[4] * y;
        m[13] += m[1] * x + m[5] * y;
    }

    const float* data() const {
        return m;
    }

    virtual void set_uniform(Uniform* u) { u->value(*this); }
};
*/


class uvec1fv: public Value {
public:
    uint n;
    float* x;

    uvec1fv(): n(0), x(nullptr) {}

    uvec1fv(uint nn, const float* data = nullptr): n(nn), x(nullptr) {
        x = new float[n];
        if (data) {
            memcpy(x, data, n*sizeof(float));
        } else {
            memset(x, 0, n*sizeof(float));
        }
    }

    ~uvec1fv() {
        if (x) delete[] x;
    }

    void set(uint nn, const float* data = nullptr) {
        if (n != nn) {
            n = nn;
            if (x) delete[] x;
            x = new float[n];
        }
        if (data) {
            memcpy(x, data, n*sizeof(float));
        } else {
            memset(x, 0, n*sizeof(float));
        }
    }

    const float* data() const {
        return x;
    }

    uint size() const {
        return n;
    }

    virtual void set_uniform(Uniform* u) { u->value(*this); }
};

} // namespace bacchus
