#include <fstream>
#include <sstream>

#include "math/matalg.h"
#include "utils/except.h"
#include "utils/logger.h"

namespace bacchus {

void load_data(const std::string &filename, matxf &x, matxf &y, bool add_1) {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) {
        std::string err_msg = "could not load file: " + filename;
        LOGE(err_msg.c_str());
        throw LogicError(err_msg.c_str());
    }
    std::string line;
    bool first = true;
    while (!in.eof()) {
        std::getline(in, line);
        if (line.empty()) continue;
        std::istringstream iss(line.c_str());
        char trash;
        float xi;
        if (add_1) x.data.push_back(1.f);
        while (iss >> xi) {
            x.data.push_back(xi);
            iss >> trash;
        }
        y.data.push_back(x.data.back());
        x.data.pop_back();
        if (first) {
            x.n = x.data.size();
            first = false;
        }
    }
    y.n = 1;
    y.m = y.data.size();
    x.m = y.m;
}

matxf normal_equation(
        const matxf& x,
        const matxf& y)
{
    matxf tr = transpose(x);
    return inverse(tr*x)*tr*y;
}

float compute_cost(const matxf &x, const matxf &y, const matxf &theta) {
    matxf md = x*theta - y;
    return 0.5f*inprod(md,md)/(float)x.m;
}

void variance(const matxf &x, matxf &mu, matxf &sigma) {
    uint m = x.m;
    uint n = x.n;
    mu[0] = 0.f; sigma[0] = 1.f;
    for (uint j = 1; j < n; ++j) {
        float mui = 0.f; float sigmai = 0.f;
        for (uint i = 0; i < m; ++i) {
            float delta = x(i,j) - mui;
            mui += delta/float(i+1);
            sigmai += delta*(x(i,j) - mui);
        }
        mu[j] = mui; sigma[j] = std::sqrt(sigmai/float(m-1));
    }
}

matxf feature_normalize(const matxf &x, matxf &mu, matxf &sigma) {
    uint n = x.n;
    assert(mu.m = n);
    assert(sigma.m = n);
    variance(x, mu, sigma);
    return ext_row_div( ext_row_sub(x, mu), sigma );
}

matxf gradient_descent(const matxf &x, const matxf &y, float a, uint max_steps, float /*presicion*/) {
    uint m = x.m;
    uint n = x.n;
    matxf theta = matxf::zero(n);
    for (uint step = 0; step < max_steps; ++step) {
        matxf md = x*theta - y;
        theta -= a*ext_col_sum(ext_col_mul(x,md))/(float)m;
    }
    return theta;
}

///// Logistic regression

//matxf sigmoid(const matxf& z) {
//    return std::move(transform(z, [](float x){ return 1.f/(1.f + std::exp(-x)); }));
//}

////h = sigmoid(X*theta);
////J = (1.0/m)*sum(-y.*log(h) - (1.0 .- y).*log(1.0 .- h));
////grad = (1.0/m)*sum((h .- y).*X);
//float logistic_cost(const matxf& x, const matxf& y, const matxf& theta, matxf& grad) {
//    int m = x.m;
//    float rm = 1.f/(float)m;
//    matxf h = sigmoid(x*theta);
//    float J = rm*sum(
//                - mul(y,
//                      transform(h, [](float f){return std::log(f);}))
//                - mul(transform(y, [](float f){return 1.f - f;}),
//                      transform(h, [](float f){return std::log(1.f - f);}))
//                );
//    grad = rm*ext_col_sum( ext_col_mul(x, h - y) );
//    return J;
//}

//float logistic_cost_reg(const matxf& x, const matxf& y, float lambda, const matxf& theta, matxf& grad) {
//    int m = x.m;
//    float rm = 1.f/(float)m;
//    matxf h = sigmoid(x*theta);
//    float J = rm*sum(
//                - mul(y,
//                      transform(h, [](float f){return std::log(f);}))
//                - mul(transform(y, [](float f){return 1.f - f;}),
//                      transform(h, [](float f){return std::log(1.f - f);}))
//                )
//            + 0.5f*rm*(lambda*sum(mul(theta,theta)) - sq(theta[0]));
//    grad = rm*ext_col_sum(ext_col_mul(x, h - y));
//    for (uint i = 1; i < grad.size(); ++i) {
//        grad[i] += rm*lambda*sum(theta);
//    }

//    return J;
//}

//float fminunc(
//        const matxf &x
//        , const matxf &y
//        , matxf &theta
//        , float a
//        , uint max_steps
//        , float presicion)
//{
//    uint steps = 0;
//    float curn_cost = 10000.f;
//    float prev_cost = curn_cost + presicion + 1.f;
//    matxf grad(x.n);

//    while (prev_cost - curn_cost > presicion) {
//        prev_cost = curn_cost;

//        curn_cost = logistic_cost(x,y,theta,grad);
//        theta -= a*grad;

//        if (curn_cost > prev_cost) {
//            std::cout << "J is increasing: a is too large" << std::endl;
//            break;
//        }

//        ++steps;
//        if (steps > max_steps) {
//            std::cout << "slow converge: a is too small" << std::endl;
//            break;
//        }
//    }
//    return curn_cost;
//}

//float fminunc_reg(
//        const matxf &x
//        , const matxf &y
//        , matxf &theta
//        , float a
//        , float lambda
//        , uint max_steps
//        , float presicion)
//{
//    uint steps = 0;
//    float curn_cost = 10000.f;
//    float prev_cost = curn_cost + presicion + 1.f;
//    matxf grad(x.n);

//    while (prev_cost - curn_cost > presicion) {
//        prev_cost = curn_cost;

//        curn_cost = logistic_cost_reg(x,y,lambda,theta,grad);
//        theta -= a*grad;

//        if (curn_cost > prev_cost) {
//            std::cout << "J is increasing: a is too large" << std::endl;
//            break;
//        }

//        ++steps;
//        if (steps > max_steps) {
//            std::cout << "slow converge: a is too small" << std::endl;
//            break;
//        }
//    }
//    return curn_cost;
//}

} // namespace bacchus
