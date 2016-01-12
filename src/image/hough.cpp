#include "hough.h"

#include <iomanip>

#include "image.h"
#include "geometric.h"

namespace bacchus {

std::vector<vec2f> Hough::transform(Image &out, const Image &img1) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    //out w - 0..pi
    // h 0..sqrt(a*a+b*b)
    const int FG = 255;

    const int ro_res = sqrt(sq(width)+sq(height));
    const float ro_kfc = 10.0f;
    const int ro_imgsz = ro_kfc*ro_res;
    // angle cant be -90..180
    const int dangle = 1;
    const int angle_res = 270/dangle;

    // pre compute cos and sin
    std::vector<float> arr_co(angle_res);
    std::vector<float> arr_si(angle_res);
    for (int a = 0; a < angle_res; ++a) {
        float th = deg2rad(a*dangle - 89.99f);// dont use points -90 and 180
        arr_co[a] = std::cos(th);
        arr_si[a] = std::sin(th);
    }

    // hough accumulate
    Image acc(angle_res, ro_imgsz);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (img1.lum(x,y)==FG) {
                for (int a = 0; a < angle_res; ++a) {
                    float ro = (float)x*arr_co[a] + (float)y*arr_si[a];
                    if (ro > 0.0f) {
                        acc.pix(a, ro_kfc*ro) += 1;
                    }
                }
            }
        }
    }

//    std::cout<<"img:"<<std::endl;
//    for (int y = 0; y < height; ++y) {
//        for (int x = 0; x < width; ++x) {
//            std::cout<<(img1.lum(x,y)==255?1:0);
//        }
//        std::cout<<std::endl;
//    }
//    std::cout<<std::endl;

//    std::cout<<"acc:"<<std::endl;
//    for (int ro = 0; ro < ro_imgsz; ++ro) {
//        for (int a = 0; a < angle_res; ++a) {
//            std::cout<<acc.pix(a,ro);
//        }
//        std::cout<<std::endl;
//    }
//    std::cout<<std::endl;

    std::vector<vec2f> res;
    // find max value for grayscale
    uint ro_max = 0;
    for (int ro = 0; ro < ro_imgsz; ++ro) {
        for (int a = 0; a < angle_res; ++a) {
            if (acc.pix(a,ro) > ro_max) {
                ro_max = acc.pix(a,ro);
            }
        }
    }

    std::cout<<"ro_max: "<<ro_max<<std::endl;

//    // out is grayscale test image
    float norm = 255.0f/(float)ro_max;
    for (int ro = 0; ro < ro_imgsz; ++ro) {
        for (int a = 0; a < angle_res; ++a) {
            if (acc.pix(a,ro) > 0.5f*ro_max) {
                res.push_back(vec2f(deg2rad(a*dangle - 89.99f), ro/ro_kfc));//
            }
            acc.pix(a,ro) = lum2i(norm*acc.pix(a,ro));
        }
    }

    Geometric::scale(out, acc, width, height, Geometric::FILTER_NEAREST);

    return res;
}


} // namespace bacchus
