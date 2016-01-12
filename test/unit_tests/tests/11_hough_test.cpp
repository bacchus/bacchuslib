#include "setting.h"

#include "image/image.h"
#include "image/hough.h"

using namespace bacchus;

TEST(HoughTransform, Test) {
    if (0) {
        Image img(5,5);
        Image out(5,5);
        img.clear().to_a1();


        img.pix(1,3) = lum2i(255);
        img.pix(3,1) = lum2i(255);
        img.pix(1,1) = lum2i(255);

        std::vector<vec2f> hough = Hough::transform(out, img);
        PRINT(hough);
    }
}
