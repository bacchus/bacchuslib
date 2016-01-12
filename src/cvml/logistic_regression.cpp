#include "logistic_regression.h"

#include <fstream>
#include <sstream>

#include "math/matalg.h"

#include "utils/except.h"
#include "utils/logger.h"

namespace bacchus {

matxf sigmoid(const matxf& z) {
    return std::move(z.transform([](float x){ return 1.f/(1.f + std::exp(-x)); }));
}

//h = sigmoid(X*theta);
//J = (1.0/m)*sum(-y.*log(h) - (1.0 .- y).*log(1.0 .- h));
//grad = (1.0/m)*sum((h .- y).*X);
float logistic_cost(const matxf& x, const matxf& y, const matxf& theta, matxf& grad) {
    grad.resize(x.n, 1);
    int m = x.m;
    float rm = 1.f/(float)m;
    matxf h = sigmoid(x*theta);
    float J = rm*sum(
                - mul(y,
                      h.transform([](float f){return std::log(f);}))
                - mul(y.transform([](float f){return 1.f - f;}),
                      h.transform([](float f){return std::log(1.f - f);}))
                );
    grad = rm*ext_col_sum( ext_col_mul(x, h - y) );
    return J;
}

float logistic_cost_reg(const matxf& x, const matxf& y, float lambda, const matxf& theta, matxf& grad) {
    grad.resize(x.n, 1);
    int m = x.m;
    float rm = 1.f/(float)m;
    matxf h = sigmoid(x*theta);
    float J = rm*sum(
                - mul(y,
                      h.transform([](float f){return std::log(f);}))
                - mul(y.transform([](float f){return 1.f - f;}),
                      h.transform([](float f){return std::log(1.f - f);}))
                )
            + 0.5f*rm*(lambda*sum(mul(theta,theta)) - sq(theta[0]));
    grad = rm*ext_col_sum(ext_col_mul(x, h - y));
    for (uint i = 1; i < grad.size(); ++i) {
        grad[i] += rm*lambda*sum(theta);
    }
    return J;
}

float fminunc(
        const matxf &x
        , const matxf &y
        , matxf &theta
        , float a
        , uint max_steps
        , float presicion)
{
    uint steps = 0;
    float curn_cost = 10000.f;
    float prev_cost = curn_cost + presicion + 1.f;
    matxf grad(x.n);

    while (prev_cost - curn_cost > presicion) {
        prev_cost = curn_cost;

        curn_cost = logistic_cost(x,y,theta,grad);
        theta -= a*grad;

        if (curn_cost > prev_cost) {
            std::cout << "J is increasing: a is too large" << std::endl;
            break;
        }

        ++steps;
        if (steps > max_steps) {
            std::cout << "slow converge: a is too small" << std::endl;
            break;
        }
    }
    return curn_cost;
}

float fminunc_reg(
        const matxf &x
        , const matxf &y
        , matxf &theta
        , float a
        , float lambda
        , uint max_steps
        , float presicion)
{
    uint steps = 0;
    float curn_cost = 10000.f;
    float prev_cost = curn_cost + presicion + 1.f;
    matxf grad(x.n);

    while (prev_cost - curn_cost > presicion) {
        prev_cost = curn_cost;

        curn_cost = logistic_cost_reg(x,y,lambda,theta,grad);
        theta -= a*grad;

        if (curn_cost > prev_cost) {
            std::cout << "J is increasing: a is too large" << std::endl;
            break;
        }

        ++steps;
        if (steps > max_steps) {
            std::cout << "slow converge: a is too small" << std::endl;
            break;
        }
    }
    return curn_cost;
}

matxf predict(const matxf &X, const matxf &theta) {
    //matxf prob = sigmoid(ext_row_sum(ext_row_mul((ext_row_div(ext_row_sub(X, mu),sigma)), theta)));
    return sigmoid(ext_row_sum(ext_row_mul(X, theta)));
}

float train_accuracy(const matxf &x, const matxf &y, const matxf &theta) {
    matxf p = predict(x,theta);
    float res = 0;
    for (uint i = 0; i < x.m; ++i) {
        if ((p[i]>=0.5f && y[i]>=0.5f) || (p[i]<0.5f && y[i]<0.5f))
            res += 1.f;
    }
    return res/(float)x.m;
}

matxf add_poly_features(const matxf &x1, const matxf &x2) {
    uint degree = 6;
    uint m = x1.m;
    assert(x2.m == m);
    assert(x1.n == 1);
    assert(x2.n == 1);

    uint nnew = 1;
    for (uint i = 1; i <= degree; ++i) {
        for (uint j = 0; j <= i; ++j) {
            ++nnew;
        }
    }

    matxf res(m,nnew);
    for (uint i = 0; i < m; ++i) {
        res(i,0) = 1.f;
    }

    uint cur = 1;
    for (uint i = 1; i <= degree; ++i) {
        for (uint j = 0; j <= i; ++j) {
            res.set_col(cur,
                        mul(x1.transform([i,j](float xij){return fpown(xij,i-j);}),
                            x2.transform([j](float xij){return fpown(xij,j);}) ));
            ++cur;
        }
    }
    return res;
}

} // namespace bacchus
