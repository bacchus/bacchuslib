#pragma once
#include "math/matmxn.h"

namespace bacchus {

void load_data(
        const std::string& filename
        , matxf& x
        , matxf& y
        , bool add_1 = true);

float compute_cost(
        const matxf& x
        , const matxf& y
        , const matxf& theta
        );

matxf feature_normalize(
        const matxf& x
        , matxf &mu
        , matxf &sigma);

matxf gradient_descent(
        const matxf &x
        , const matxf &y
        , float a = 0.01f               // 0.001 0.01 0.1 1 10 ...
        , uint max_steps = 1500         // 100 1,000 10,000
        , float presicion = 0.00001f    // 0.1 0.01 0.001 0.0001
        );

matxf normal_equation(
        const matxf& x,
        const matxf& y
        );

//matxf sigmoid(const matxf& z);
//float logistic_cost(const matxf& x, const matxf& y, const matxf& theta, matxf& grad);
//float fminunc(const matxf& x, const matxf& y, matxf& theta
//        , float a = 0.01f               // 0.001 0.01 0.1 1 10 ...
//        , uint max_steps = 1500         // 100 1,000 10,000
//        , float presicion = 0.00001f    // 0.1 0.01 0.001 0.0001
//        );

//float logistic_cost_reg(
//        const matxf& x
//        , const matxf& y
//        , float lambda
//        , const matxf& theta
//        , matxf& grad);

//float fminunc_reg(
//        const matxf &x
//        , const matxf &y
//        , matxf &theta
//        , float a = 0.01f               // 0.001 0.01 0.1 1 10 ...
//        , float lambda = 1.f            // 1
//        , uint max_steps = 1500         // 100 1,000 10,000
//        , float presicion = 0.00001f    // 0.1 0.01 0.001 0.0001
//        );

} // namespace bacchus
