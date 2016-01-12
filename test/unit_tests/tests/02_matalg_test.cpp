#include "setting.h"
#include "math/mat3.h"
#include "math/matalg.h"
#include "utils/logger.h"
#include "utils/except.h"

using namespace bacchus;

TEST(MatAlgs, LUP) {
    // decomposition_lup test and linear system solve
    matxf base_mlup(3,3, {5,6,3, 0.2,0.8,-0.6, 0.6,0.5,2.5});
    matxf m1(3,3, {1,2,0, 3,4,4, 5,6,3});

    //std::vector<int> base_t = {2,0,1};

    std::vector<int> t(3);
    decomposition_lup(m1,t);
    EXPECT_EQ(base_mlup, m1);

    std::vector<float> base_x = {-1.4, 2.2, 0.6};

    matxf x = solve_lup(m1, matxf(3,1, {3,7,8}), t);

    for (uint i = 0; i < x.size(); ++i) {
        EXPECT_FLOAT_EQ(base_x[i], x[i]);
    }
}

TEST(MatAlgs, INV_DET) {

    matxf base_one = matxf::one(3,3);
    matxf base_mt1(3,3, {1,4,5, 2,5,8, 3,6,7});
    matxf base_mi1(3,3, {
                       -1.08333, 0.833333, -0.25,
                       0.166667, -0.666667, 0.5,
                       0.583333, 0.166667, -0.25 }
                   );

    matxf m1(3,3, {1,2,3, 4,5,6, 5,8,7});
    matxf m2(3,3, {1,1,3, 2,4,5, 3,1,5});

    EXPECT_EQ(base_mt1, transpose(m1));

    EXPECT_FLOAT_EQ(12, determinant(m1));
    EXPECT_FLOAT_EQ(-10, determinant(m2));

    matxf mi1 = inverse(m1);
    EXPECT_EQ(base_mi1, mi1);

    matxf one = mi1*m1;
    EXPECT_EQ(base_one, one);

    matxf one2 = inverse(m2)*m2;
    EXPECT_EQ(base_one, one2);
}

TEST(MatAlgs, Degenerate) {
    // determinant degenerate test
    matxf m1(4,4, {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16});
    EXPECT_THROW(determinant(m1), MathError);
}

TEST(MatAlgs, mat3f_vs_matmn) {
    // test determinants
    float f1[] = {1,2,3, 4,5,6, 5,8,7};
    float f2[] = {1,1,3, 2,4,5, 3,1,5};
    float f3[] = {1,5,1,2,10,1,1,1,1};//inf test

    mat3f m31(f1);
    mat3f m32(f2);
    mat3f m33(f3);

    matxf mn1(3,3,f1);
    matxf mn2(3,3,f2);
    matxf mn3(3,3,f3);

    EXPECT_FLOAT_EQ(m31.determinant(), determinant(mn1));
    EXPECT_FLOAT_EQ(m32.determinant(), determinant(mn2));
    EXPECT_FLOAT_EQ(m33.determinant(), determinant(mn3));
}

TEST(MatAlgs, DetSpeedTest) {
    // determinant speed test
    const int imgw = 10;
    const int dim = imgw*imgw;
    matxf m1 = matxf::one(dim,dim);

    srand(1);
    for (int i = 0; i < 1000; ++i) {
        m1(rand()%dim,rand()%dim) = rand()%2;
    }

    std::cout << "det: " << determinant(m1) << std::endl;
}

TEST(MatAlgs, MinSquaresMethod) {
    // min squres test
    matxf x({-1,1,2,3,5});
    matxf y({2,1,1,0,3});

    //c1 + c2*x + c3*x^2
    matxf a(5,3);
    for (uint i = 0; i < x.m; ++i) {
        a(i,0) = 1;
        a(i,1) = x[i];
        a(i,2) = x[i]*x[i];
    }

    //At*A*c = At*y
    matxf at = transpose(a);
    matxf c = solve_system(at*a, at*y);

    float diff = 0;
    for (uint i = 0; i < x.m; ++i) {
        diff += fabs(y[i] - (c[0]+c[1]*x[i]+c[2]*x[i]*x[i]));
    }

    std::cout << "c   : " << transpose(c) <<std::endl;
    std::cout << "diff: " << diff <<std::endl;
}

TEST(MatAlgs, SolveLinearSystem) {
    // linear system solve
    const matxf b({3,7,8});
    const matxf m1(3, 3, {1,2,0, 3,4,4, 5,6,3});
    const matxf res({3,7,8});
    matxf a(m1);

    std::vector<int> t(3);
    decomposition_lup(a,t);
    const matxf x = solve_lup(a, b, t);

    EXPECT_TRUE(m1*x == res);
}

TEST(MatAlgs, CholeskyDecomposition) {
    const matxf a = {
        {  4, 12,-16},
        { 12, 37,-43},
        {-16,-43, 98}
    };
    const matxf res_ch = {
        { 2, 0, 0},
        { 6, 1, 0},
        {-8, 5, 3}
    };

    //PRINT(a);
    //PRINT(res_ch);
    EXPECT_EQ(a, res_ch*transpose(res_ch));

    const matxf ch = decomposition_cholesky(a);
    EXPECT_EQ(res_ch, ch);

    EXPECT_EQ(a, ch*transpose(ch));
}

TEST(MatAlgs, LDLDecomposition) {
    const matxf a = {
        {  4, 12,-16},
        { 12, 37,-43},
        {-16,-43, 98}
    };
    const matxf res_ch = {
        { 1, 0, 0},
        { 3, 1, 0},
        {-4, 5, 1}
    };
    const matxf res_d = {
        { 4, 0, 0},
        { 0, 1, 0},
        { 0, 0, 9}
    };

    //PRINT(a);
    //PRINT(res_ch);
    EXPECT_EQ(a, res_ch*res_d*transpose(res_ch));

    matxf d;
    const matxf ch = decomposition_ldl(a,d);
    EXPECT_EQ(res_ch, ch);
    EXPECT_EQ(res_d, d);

    EXPECT_EQ(a, ch*d*transpose(ch));
}

TEST(MatAlgs, CholeskyUpdatePlus) {
    const matxf a = {
        {  4, 12,-16},
        { 12, 37,-43},
        {-16,-43, 98}
    };
    const matxf x = {1,2,3};

    const matxf ch = decomposition_cholesky(a);
    const matxf chup = cholupdate(ch, x, '+');

    EXPECT_EQ(a + x*transpose(x), chup*transpose(chup));
}

TEST(MatAlgs, CholeskyUpdateMinus) {
    const matxf a = {
        {  4, 12,-16},
        { 12, 37,-43},
        {-16,-43, 98}
    };
    const matxf x = {0.1f,0.2f,0.3f};

    const matxf ch = decomposition_cholesky(a);
    const matxf chup = cholupdate(ch, x, '-');

    EXPECT_EQ(a - x*transpose(x), chup*transpose(chup));
}

