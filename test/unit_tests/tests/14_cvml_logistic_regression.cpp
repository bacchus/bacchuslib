#include "setting.h"

#include <cmath>

#include "math/matalg.h"
#include "cvml/logistic_regression.h"
#include "cvml/fmincg.h"
#include "utils/except.h"

using namespace bacchus;

TEST(LogisticRegression, Sigmoid) {
    matxf x = {1.f, 2.f, 3.f};
    matxf y = sigmoid(x);
    EXPECT_EQ(matxf({0.73106f, 0.88080f, 0.95257f}), y);
}

TEST(LogisticRegression, CostFunction) {
    matxf x;
    matxf y;
    load_data("ex2data1.txt", x,y);
    matxf theta = matxf::zero(x.n);
    matxf grad(x.n);
    //matxf mu(x.n);
    //matxf sigma(x.n);
    //x = feature_normalize(x,mu,sigma);// normalize
    float cost = logistic_cost(x,y,theta, grad);
    std::cout << std::setprecision(5);
    EXPECT_TRUE(std::fabs(0.693147f - cost) < 0.1f);
    //EXPECT_EQ(matxf({-0.100000, -0.279819, -0.249728}), grad);// normalized
    EXPECT_EQ(matxf({-0.100000, -12.009217, -11.262842}), grad);// not normalized
}

TEST(LogisticRegression, Fminunc) {
    matxf x;
    matxf y;
    load_data("ex2data1.txt", x,y);
    matxf theta = matxf::zero(x.n);
    matxf mu(x.n);
    matxf sigma(x.n);

    float cost = logistic_cost(x,y,theta,mu);
    std::cout << std::setprecision(5);
    PRINT(cost);

    x = feature_normalize(x,mu,sigma);
    cost = fminunc(x,y,theta, 10.f,400,0.00001f);

    //PRINT(theta);
    //EXPECT_TRUE(std::fabs(0.203498f - cost) < 0.1f);
    //EXPECT_EQ(matxf({-25.161272, 0.206233, 0.201470}), theta);

    matxf prob = sigmoid(transpose(theta)*div((matxf({1,45,85}) - mu),sigma));
    PRINT(prob[0]);
    //EXPECT_EQ(0.776289f, prob[0]);

    PRINT(train_accuracy(x,y,theta));
}

TEST(LogisticRegression, Fmincg) {
    matxf x;
    matxf y;
    load_data("ex2data1.txt", x,y);
    matxf init_theta = matxf::zero(x.n);
//    matxf mu(x.n);
//    matxf sigma(x.n);
    matxf grad(x.n);

//    float cost = logistic_cost(x,y,theta,grad);
//    std::cout << std::setprecision(5);
//    PRINT(cost);

//    x = feature_normalize(x,mu,sigma);

    //fminunc(x,y,theta, 10.f,400,0.00001f);
    fmincg_res_t res = fmincg([x,y](const matxf& theta, matxf& grad){ return logistic_cost(x,y,theta,grad); }, init_theta, fmincg_opt_t(50));


    //PRINT(theta);
    //EXPECT_TRUE(std::fabs(0.203498f - cost) < 0.1f);
    //EXPECT_EQ(matxf({-25.161272, 0.206233, 0.201470}), theta);

//    matxf prob = sigmoid(transpose(theta)*div((matxf({1,45,85}) - mu),sigma));
    float prob = sigmoid(transpose(res.X)*matxf({1,45,85}));
    PRINT(prob);
    //EXPECT_EQ(0.776289f, prob[0]);

    PRINT(train_accuracy(x,y,res.X));
}


TEST(LogisticRegression, Regularization) {
    matxf x;
    matxf y;
    load_data("ex2data2.txt", x,y);
    //PRINT(x);

    x = add_poly_features(ext_col_get(x,1),ext_col_get(x,2));
    //PRINT(x);

    matxf theta = matxf::zero(x.n);
    matxf grad(x.n);

    PRINT(logistic_cost_reg(x,y,1,theta,grad));

    matxf mu(x.n);
    matxf sigma(x.n);
    x = feature_normalize(x,mu,sigma);
    float cost = fminunc_reg(x,y,theta, 10.f,100000,0.00001f);

    std::cout << std::setprecision(5);
    PRINT(cost);
    //PRINT(theta);
    //EXPECT_TRUE(std::fabs(0.203498f - cost) < 0.1f);
    //EXPECT_EQ(matxf({-25.161272, 0.206233, 0.201470}), theta);

    //PRINT(logistic_cost_reg(x,y,1.f,matxf(theta.size()),mu));

//    matxf prob = sigmoid(transpose(theta)*((matxf({1,45,85}) - mu)/=sigma));
//    PRINT(prob[0]);
    //EXPECT_EQ(0.776289f, prob[0]);

    PRINT(train_accuracy(x,y,theta));
}
