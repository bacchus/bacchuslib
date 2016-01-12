#include "setting.h"

#include "math/geometric.h"

using namespace bacchus;

/*
 *  p1------p2
 *   \          direction > 0
 *    \
 *      p0
 */
TEST(geometric_test, segments_intersection) {
    vec2f p0(5,5);
    vec2f p1(2,10);
    vec2f p2(8,10);

    EXPECT_TRUE(direction(p0,p1,p2) > 0.f);
    EXPECT_TRUE(direction(p0,p2,p1) < 0.f);

    EXPECT_TRUE(onsegment(p1,p2,vec2f(5,10)));

    EXPECT_TRUE(segments_intersect(p1,p2,p0,vec2f(5,15)));
    EXPECT_FALSE(segments_intersect(p1,p2,p0,vec2f(10,5)));
    EXPECT_TRUE(segments_intersect(p1,p2,p0,p1));
}
