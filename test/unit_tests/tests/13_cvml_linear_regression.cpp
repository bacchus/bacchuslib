#include "setting.h"

#include <cmath>

#include "math/matalg.h"
#include "cvml/linear_regression.h"
#include "utils/except.h"

using namespace bacchus;

TEST(LinearRegression, Load1d) {
    matxf x;
    matxf y;
    load_data("ex1data1_10.txt", x,y);
    matxf resx = {
        {1,6.1101}, {1,5.5277}, {1,8.5186}, {1,7.0032}, {1,5.8598},
        {1,8.3829}, {1,7.4764}, {1,8.5781}, {1,6.4862}, {1,5.0546}
    };
    matxf resy = {
        17.592 ,9.1302 ,13.662 ,11.854 ,6.8233 ,11.886 ,4.3483 , 12, 6.5987 ,3.8166
    };
    EXPECT_EQ(resx, x);
    EXPECT_EQ(resy, y);
}

TEST(LinearRegression, Load2d) {
    matxf x;
    matxf y;
    load_data("ex1data2_10.txt", x,y);
    matxf resx = {
        {1,2104,3}, {1,1600,3}, {1,2400,3}, {1,1416,2}, {1,3000,4},
        {1,1985,4}, {1,1534,3}, {1,1427,3}, {1,1380,3}, {1,1494,3}
    };
    matxf resy = {
        399900, 329900, 369000, 232000, 539900, 299900, 314900, 198999, 212000, 242500
    };
    EXPECT_EQ(resx, x);
    EXPECT_EQ(resy, y);
}

TEST(LinearRegression, ComputeCost) {
    matxf x;
    matxf y;
    load_data("ex1data1.txt", x,y);
    matxf theta = matxf::zero(x.n);
    float cost = compute_cost(x,y,theta);
    EXPECT_TRUE(std::fabs(32.07f - cost) < 0.01f);
}

TEST(LinearRegression, FeatureNormalize) {
    matxf x;
    matxf y;
    load_data("ex1data1_10.txt", x,y);
    matxf mu(x.n);
    matxf sigma(x.n);
    matxf xn = feature_normalize(x,mu,sigma);

    matxf resx = {
        { 1.0000, -0.6088, }, { 1.0000, -1.0577, }, { 1.0000,  1.2480, },
        { 1.0000,  0.0797, }, { 1.0000, -0.8017, }, { 1.0000,  1.1434, },
        { 1.0000,  0.4445, }, { 1.0000,  1.2939, }, { 1.0000, -0.3188, },
        { 1.0000, -1.4225, },
    };

    matxf resmu = { 0.00f ,  6.8998f };

    matxf ressigma = {
        1.00, 1.2972
    };

    EXPECT_EQ(resx, xn);
    EXPECT_EQ(resmu, mu);
    EXPECT_EQ(ressigma, sigma);
}

TEST(LinearRegression, GradientDescent) {
    matxf x;
    matxf y;
    load_data("ex1data1.txt", x,y);
    //matxf mu(x.n);
    //matxf sigma(x.n);
    //x = feature_normalize(x,mu,sigma);
    matxf restheta = { -3.6303f, 1.1664f };
    matxf theta = gradient_descent(x,y);
    //PRINT(theta);
    EXPECT_EQ(restheta, theta);
}

TEST(LinearRegression, GradientDescentMulti) {
    matxf x;
    matxf y;
    load_data("ex1data2.txt", x,y);
    matxf mu(x.n);
    matxf sigma(x.n);
    x = feature_normalize(x,mu,sigma);

    matxf restheta = { 340411.1562f, 110540.5859f, -6558.9917f };

    matxf theta = gradient_descent(x,y);
    //PRINT(theta);
    EXPECT_EQ(restheta, theta);

    matxf price = transpose(theta)*div((matxf({1,1650,3}) - mu),sigma);
    EXPECT_EQ(1, price.size());
    EXPECT_EQ(293099.625f, price[0]);
}

TEST(LinearRegression, Prediction) {
    matxf x;
    matxf y;
    matxf restheta = { 340411.1562f, 110540.5859f, -6558.9917f };
    load_data("ex1data2.txt", x,y);
    matxf mu(x.n);
    matxf sigma(x.n);
    x = feature_normalize(x,mu,sigma);
    EXPECT_EQ(restheta, gradient_descent(x,y));
}

TEST(LinearRegression, NormalEquation) {
    matxf x;
    matxf y;
    load_data("ex1data2.txt", x,y);
    matxf restheta = {
        89597.3516,
        139.2108,
        -8738.0674,
    };
    matxf theta = normal_equation(x,y);
    //PRINT(theta);
    EXPECT_EQ(restheta, theta);
    matxf price = transpose(theta)*matxf({1,1650,3});
    EXPECT_EQ(1, price.size());
    EXPECT_EQ(293081, price[0]);
}
