#include "histogram.h"
#include "image.h"

namespace bacchus {

void Histogram::histogram(const Image& img1, int* h, int chanel) {
    memset(h, 0, 256*sizeof(int));
    for (uint i = 0; i < img1.height; ++i) {
        for (uint j = 0; j < img1.width; ++j) {
            ++h[img1.rgb(vec2i(j,i))[chanel]];
        }
    }
//    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
//        //++h[img1.dat8[i+chanel]];
//    }
}

Image& Histogram::threshold_constant(Image& out, const Image& img1, uchar t) {
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        out.dat8[i  ] = img1.dat8[i  ] > t ? 255 : 0;
        out.dat8[i+1] = img1.dat8[i+1] > t ? 255 : 0;
        out.dat8[i+2] = img1.dat8[i+2] > t ? 255 : 0;
        out.dat8[i+3] = img1.dat8[i+3];
    }
    return out;
}

Image& Histogram::threshold_global(Image& out, const Image& img1, int threshold_rise) {
    int h[256];
    histogram(img1, h);
    int sz = img1.size();
    int sum = h[0];
    int t = 0;
    while (2*sum < sz) {
        ++t;
        sum += h[t];
    }
    t += threshold_rise;
    for (uint i = 0; i < img1.height; ++i) {
        for (uint j = 0; j < img1.width; ++j) {
            vec2i p(j,i);
            out.pix(p) = lum2i(img1.lum(p) > t ? 255 : 0);
            //bvec3 thr = gtv(img1.rgb(p), vec3i(t));
            //out.pix(p) = rgb2i( ifv(thr, vec3i(255), vec3i(0)) );
        }
    }

//    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
//        out.dat8[i  ] = img1.dat8[i  ] > t ? 255 : 0;
//        out.dat8[i+1] = img1.dat8[i+1] > t ? 255 : 0;
//        out.dat8[i+2] = img1.dat8[i+2] > t ? 255 : 0;
//        out.dat8[i+3] = img1.dat8[i+3];
//    }
    return out;
}

// methods: histogram, mean, median, (max-min)/2
Image& Histogram::threshold_adaptive(Image& out, const Image& img1, int r, int c) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

//    Image img_tmp(width, height);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {

            int h[256];
            memset(h, 0, 256*sizeof(int));
//                for (int i = 0; i < 256; ++i) { h[i] = 0; }

            int sz = 0;
            int sum = 0;
            for (int y = i-r; y <= i+r; ++y) {
                for (int x = j-r; x <= j+r; ++x) {
                    if (y>=0 && y<height && x>=0 && x<width) {
                        ++h[img1.lum(x,y)];
//                            ++h[pix(x,y,0)];
//                            sum += pix(x,y,0);
                        ++sz;
                    }
                }
            }
//                int t = sum/sz + c;

            int t = 0;
            sum = h[0];
            while (2*sum < sz) {
                ++t;
                sum += h[t];
            }
            t += c;

            uchar tmp = img1.lum(j,i) > t ? 255 : 0;
            out.pix(j,i) = lum2i(tmp);
//            img_tmp.pix(j,i) = lum2i(tmp);

        }
    }

    return out;
}

Image& Histogram::contrast_stretch(Image& out, const Image& img1, uchar a, uchar b) {
    int h[256];
    histogram(img1, h);
    int sz = img1.size();
    int sum = h[0];
    int t = 0;
    while (sum < 0.05f*sz) {
        ++t;
        sum += h[t];
    }
    int c = t;
    while (sum < 0.95f*sz) {
        ++t;
        sum += h[t];
    }
    int d = t;

    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        uchar tmp = clamp_color((img1.dat8[i] - c)*(b - a)/(d - c) + a);
        out.dat32[i/4] = lum2i(tmp);
    }

    return out;
}

Image& Histogram::histogram_equalize(Image& out, const Image& img1) {
    float h[256];
    memset(h, 0, 256*sizeof(float));
    //const float dh = 1.0f/(width*height);
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        ++h[img1.dat8[i]];// += dh
    }

    float c = 255.0f/(float)img1.size();
    for (int i = 0; i < 256; ++i) {
        h[i] *= c;
    }

    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        float sum = 0;
        for (int j = 0; j < img1.dat8[i]; ++j) {
            sum += h[j];
        }
        out.dat32[i/4] = lum2i(sum);
    }

    return out;
}

Image& Histogram::histogram_equalize_local(Image& out, const Image& img1, int r) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

//    Image img_tmp(width, height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float h[256];
            memset(h, 0, 256*sizeof(float));

            float cur_sum = 0;
            for (int i = y-r; i <= y+r; ++i) {
                for (int j = x-r; j <= x+r; ++j) {
                    if (i>=0 && i<height && j>=0 && j<width) {
                        ++h[img1.lum(j,i)];
                        //++h[pix(j,i,0)];
                        ++cur_sum;
                    }
                }
            }

            float c = 255.0f/cur_sum;
            for (int i = 0; i < 256; ++i) {
                h[i] *= c;
            }

            float sum = 0;
            for (int j = 0; j < img1.lum(x,y); ++j) {
                sum += h[j];
            }
            //img_tmp.pix(x,y) = lum2i(sum);
            out.pix(x,y) = lum2i(sum);
        }
    }

//    for (int i = 0; i < img1.size8(); i+=img1.fmt) {
//        out.dat32[i/4] = lum2i(img_tmp.dat8[i]);// some magic very bad
//    }

    return out;
}

} // namespace bacchus
