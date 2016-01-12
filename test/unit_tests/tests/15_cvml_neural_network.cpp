#include "setting.h"

#include <cmath>

#include "math/matalg.h"
#include "cvml/logistic_regression.h"
#include "cvml/fmincg.h"
#include "utils/except.h"

using namespace bacchus;

matxf one_vs_all(const matxf& x, const matxf& y, int num_labels, float lambda) {
    uint n = x.n;
    matxf all_theta(num_labels, n);
    for (int c = 1; c <= num_labels; ++c) {
        matxf initial_theta(n);
        fmincg_res_t res =
                fmincg([x,y,lambda](const matxf& theta, matxf& grad) -> float {
                    return logistic_cost_reg(x,y,lambda,theta,grad);
                }, initial_theta, fmincg_opt_t(50));
        std::cout << "Iteration " << res.i << " | Cost: " << res.fX.back();
        all_theta.set_row(c-1, res.X);
    }
    return all_theta;
}

TEST(NeuralNet, lrCostFunc) {
//    matxf x;
//    matxf y;
//    load_data("ex3data1.txt", x,y);
//    matxf theta = matxf::zero(x.n);
//    matxf grad(x.n);
//    float cost = logistic_cost_reg(x,y,0.1f,theta, grad);
//    std::cout << std::setprecision(6);
//    //PRINT(cost);
//    EXPECT_LE(ulp_difference(0.693147f, cost, 1.0e-6f), 1024);
//    matxf grad_10 = {
//        -5.000000f, 0.000000f, 0.000000f, -0.000000f, 0.000003f, 0.000019f, -0.000706f,
//        -0.000897f, -0.000373f, -0.000111f,
//    };
//    for (int i = 0; i < 10; ++i) {
//        EXPECT_LE(ulp_difference(grad_10[i],grad[i], 1.0e-6f), 10);
//    }

//    int num_labels = 10;
//    float lambda = 0.1f;
//    matxf all_theta = one_vs_all(x,y,num_labels,lambda);
//    for (int i = 0; i < 5; ++i) {
//        for (int j = 0; j < 5; ++j) {
//            std::cout << all_theta(i,j) << " ";
//        }
//        std::cout << std::endl;
//    }
//    std::cout << std::endl;
}
