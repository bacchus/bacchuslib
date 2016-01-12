#include "setting.h"

#include "utils/logger.h"
#include "math/linear_prog.h"

TEST(linear_prog, simple) {
    using namespace bacchus;

    // simple test for linear program
    float fa[] = {1,1,3, 2,2,5, 4,1,2};
    std::vector<float> res = {8,4,0,18,0,0};
    matxf A(3,3,fa);

    std::vector<float> c = {3, 1, 2};
    std::vector<float> b = {30, 24, 36};
    std::vector<float> x;

    print_equatoins(A,b,c);
    simplex(A,b,c,x);
    std::cout << "res: " << x << std::endl;

    EXPECT_TRUE(std::equal(res.begin(), res.end(), x.begin()));
}

TEST(linear_prog, degenerade) {
    using namespace bacchus;

    // degenerade test for linear program
    float fa[] = {1,1,0, 0,-1,1};
    matxf A(2,3,fa);

    std::vector<float> c = {1,1,1};
    std::vector<float> b = {8,0};
    std::vector<float> x;

    print_equatoins(A,b,c);
    simplex(A,b,c,x);
    std::cout << "res: " << x << std::endl;
}

TEST(linear_prog, nonzero_base) {
    using namespace bacchus;

    // nonzero base test for linear program
    float fa[] = {2,-1, 1,-5};
    matxf A(2,2,fa);

    std::vector<float> c = {2,-1};
    std::vector<float> b = {2,-4};
    std::vector<float> x;

    print_equatoins(A,b,c);
    simplex(A,b,c,x);
    std::cout << "res: " << x << std::endl;

    EXPECT_FLOAT_EQ(2.f, 2.f*x[0]-x[1]);
    EXPECT_FLOAT_EQ(0.f, x[2]);
    EXPECT_FLOAT_EQ(0.f, x[3]);
}
