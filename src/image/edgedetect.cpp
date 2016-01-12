#include "edgedetect.h"
#include "image.h"

#include "math/mdefs.h"

namespace bacchus {

Image& Edgedetect::robcross(Image& out, const Image& img1) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int g1 = img1.lum(j,i) - img1.lum(j+1,i+1);
            int g2 = img1.lum(j,i) - img1.lum(j-1,i+1);
            out.pix(j,i) = lum2i(sqrt(g1*g1+g2*g2));
            //out.pix(j,i) = lum2i(abs(atan2(g1,g2))*128.0f/M_PI);
        }
    }

    return out;
}

uint rad2color(float rad) {
    if (rad > M_PI*3.f/4.f) return rgba2i(0,0,255);
    else if (rad > M_PI/4.f) return rgba2i(255,0,0);
    else if (rad > -M_PI/4.f) return rgba2i(0,255,0);
    else if (rad > -M_PI*3.f/4.f) return rgba2i(255,255,255);
    else return rgba2i(0,0,255);
}

Image& Edgedetect::sobel_2wrap(Image& out, const Image& img1) {
    //, Image& phi
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;
    // Gx = [1,2,1]T * [1,0,-1]; Gy = [1,0,-1]T * [1,2,1]
    // Gx = [3,10,3]T * [1,0,-1]; Gy = [1,0,-1]T * [3,10,3]
    Image tmp1(width, height);
    Image tmp2(width, height);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tmp1.spix(j,i) = img1.lum(j-1,i) - img1.lum(j+1,i);
            tmp2.spix(j,i) = img1.lum(j-1,i) + 2*img1.lum(j,i) + img1.lum(j+1,i);
            //tmp2.spix(j,i) = 3*img1.lum(j-1,i) + 10*img1.lum(j,i) + 3*img1.lum(j+1,i);
        }
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            //int gx = 3*tmp1.spix(j,i-1) + 10*tmp1.spix(j,i) + 3*tmp1.spix(j,i+1);
            int gx = tmp1.spix(j,i-1) + 2*tmp1.spix(j,i) + tmp1.spix(j,i+1);
            int gy = tmp2.spix(j,i-1) - tmp2.spix(j,i+1);
            out.pix(j,i) = lum2i(clamp_color( sqrt(sq(gx)+sq(gy)) ));
//            phi.pix(j,i) = lum2i(127.5f + 127.5f*atan2(gy,gx)/M_PI);
//            if (eq(gx,0) && eq(gy,0))
//                phi.pix(j,i) = lum2i(0);
//            else
//                phi.pix(j,i) = rad2color(atan2(gy,gx));
        }
    }

    //out.pix(j,i) = lum2i(sqrt(g1*g1+g2*g2));
    //pix(j,i) = lum2i(abs(atan2(g1,g2))*255.0f/M_PI);
    //pix(j,i) = lum2i(127.5f + 127.5f*atan2(g1,g2)/M_PI);

    return out;
}

Image& Edgedetect::sobel(Image& out, const Image& img1) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int g1 = img1.lum(j+1,i-1) + 2*img1.lum(j+1,i) + img1.lum(j+1,i+1)
                    -img1.lum(j-1,i-1) - 2*img1.lum(j-1,i) - img1.lum(j-1,i+1);

            int g2 = img1.lum(j-1,i+1) + 2*img1.lum(j,i+1) + img1.lum(j+1,i+1)
                    -img1.lum(j-1,i-1) - 2*img1.lum(j,i-1) - img1.lum(j+1,i-1);

            out.pix(j,i) = lum2i(clamp_color( sqrt(g1*g1+g2*g2) ));
            //pix(j,i) = lum2i(abs(atan2(g1,g2))*128.0f/M_PI);
        }
    }

    return out;
}

//TODO: Image& edgedetect_canny() {

Image& Edgedetect::compass(Image& out, const Image& img1) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    int d[9][2] = {{-1,-1}, { 0,-1}, {+1,-1},
                   {-1, 0}, { 0, 0}, {+1, 0},
                   {-1,+1}, { 0,+1}, {+1,+1}};
    float kern[8][9] = {
        {
            -1,+1,+1,
            -1,-2,+1,
            -1,+1,+1
        }
    };
    for (int i = 1; i < 8; ++i)
        kern9_rot45(kern[i-1], kern[i]);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            for (int rot = 0; rot < 8; ++rot) {
                float elm[9];
                memset(elm, 0, 9*sizeof(float));
                for (int k = 0; k < 9; ++k) {
                    elm[rot] += kern[rot][k]*img1.lum(j+d[k][0], i+d[k][1]);
                }
                out.pix(j,i) = lum2i(find_max_clamp(elm,9));
            }
        }
    }

    return out;
}

Image& Edgedetect::zerocross(Image& out, const Image& img1, float s, int r) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    float* kern = new float[sq(2*r+1)];
    float cnorm = 10.0f/func_laplac_of_gauss(0,0,s);
    float ksum = 0;
    int k = 0;
    for (int i = -r; i <= r; ++i) {
        for (int j = -r; j <= r; ++j) {
            kern[k] = cnorm*func_laplac_of_gauss(j,i,s);
            ksum += kern[k];
            ++k;
        }
    }

//        for (int f = 0; f < 3; ++f) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                float sum = 0.0f;
                int k = 0;
                for (int y = i-r; y <= i+r; ++y) {
                    for (int x = j-r; x <= j+r; ++x) {
                        //sum += kern[n]*pix(x, y, f);
                        sum += kern[k]*img1.lum(x, y);
                        ++k;
                    }
                }
                //img.pix(j,i,f) = clamp_color(sum/ksum);
                if (sum/ksum<20) {
                    out.pix(j,i) = lum2i(255);
                } else {
                    out.pix(j,i) = lum2i(0);
                }
            }
        }
//        }

    return out;
}

Image& Edgedetect::detect_lines(Image& out, const Image& img1) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    const int r = 1;
    float kern1[] = {-1,-1,-1, 2,2,2, -1,-1,-1};
    float kern2[] = {-1,2,-1, -1,2,-1, -1,2,-1};
    float kern3[] = {-1,-1,2, -1,2,-1, 2,-1,-1};
    float kern4[] = {2,-1,-1, -1,2,-1, -1,-1,2};
    float* kern[] = {kern1, kern2, kern3, kern4};
    uint colors[] = {rgba2i(200,0,0), rgba2i(0,200,0), rgba2i(200,200,0), rgba2i(0,200,200), rgba2i(0,0,0)};

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            float sum[] = {0,0,0,0,70};//last value - threshold
            for (int l = 0; l < 4; ++l) {
                int k = 0;
                for (int y = i-r; y <= i+r; ++y) {
                    for (int x = j-r; x <= j+r; ++x) {
                        sum[l] += kern[l][k]*img1.lum(x, y);
                        ++k;
                    }
                }
            }
            out.pix(j,i) = colors[arr_maxi(sum,5)];
        }
    }

    return out;
}

} // namespace bacchus
