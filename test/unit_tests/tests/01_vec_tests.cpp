#include "setting.h"

#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat2.h"
#include "math/mat3.h"
#include "math/mat4.h"
#include "math/matmxn.h"
#include "math/matalg.h"
#include "utils/except.h"

using namespace bacchus;

TEST(vec_tests, mat_mul_vec) {
    mat3f base_m(1,2,3, 4,5,6, 7,8,9);
    mat3f base_mt(1,4,7, 2,5,8, 3,6,9);
    vec3f base_mtv(30, 36, 42);
    vec3f base_vmt(14, 32, 50);

    mat3f t = transpose(base_m);
    EXPECT_EQ(base_mt, t);
    //    t.test_print();

    vec3f v(1,2,3);
    //    std::cout << v << std::endl;

    vec3f mtv = t*v;
    EXPECT_EQ(base_mtv, mtv);
    //    std::cout << mtv << std::endl;

    vec3f vmt = v*t;
    EXPECT_EQ(base_vmt, vmt);
    //    std::cout << vmt << std::endl;
}

TEST(vec_tests, mat_mul_mat) {
    // mat3f mult test
    mat3f m1(1,2,3, 4,5,6, 7,8,9);
    mat3f m2(0,1,3, 2,4,5, 3,1,5);
    mat3f base_m12(
                13, 12, 28,
                28, 30, 67,
                43, 48, 106);
    mat3f m3 = m1*m2;
    EXPECT_EQ(base_m12, m3);
    //m3.test_print();
}

TEST(vec_tests, mat_inv) {
    mat3f m1(1,2,3, 4,5,6, 5,8,7);
    mat3f m2(1,5,1,2,10,1,1,1,1);
    mat3f base_mi1(
                -1.08333, 0.833333, -0.25,
                0.166667, -0.666667, 0.5,
                0.583333, 0.166667, -0.25 );
    mat3f base_one(1,0,0, 0,1,0, 0,0,1);

    EXPECT_FLOAT_EQ(12, m1.determinant());
    EXPECT_FLOAT_EQ(-4, m2.determinant());
    //std::cout << m1.determinant() << " "
    //  << m2.determinant() << std::endl << std::endl;

    mat3f mi1 = m1.inverse();
    EXPECT_EQ(base_mi1, mi1);
    //mi1.test_print();

    mat3f one = mi1*m1;
    EXPECT_EQ(base_one, one);
    //one.test_print();

    mat3f one2 = m2.inverse()*m2;
    EXPECT_EQ(base_one, one2);
    //one2.test_print();

    float f3[] = {1,1,3, 2,4,5, 3,1,5};
    mat3f m3(f3);
    EXPECT_EQ(base_one, m3.inverse()*m3);
}
