#include "filters_smooth.h"

#include <iostream>
#include <iomanip>

#include "image.h"

namespace bacchus {

void print_kernel(float* kern, int r) {
    int k = 0;
    for (int i = -r; i <= r; ++i) {
        for (int j = -r; j <= r; ++j) {
            std::cout<<std::setw(7)<<std::setprecision(2)<<kern[k++]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

Image& Smooth::mean(Image& out, const Image& img1, int r) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    int n = sq(2*r+1);
    for (int f = 0; f < 3; ++f) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int sum = 0;
                for (int y = i-r; y <= i+r; ++y) {
                    for (int x = j-r; x <= j+r; ++x) {
                        sum += img1.pix(x, y, f);
                    }
                }
                out.pix(j,i,f) = sum/n;
            }
        }
    }

    return out;
}

Image& Smooth::mean_lum(Image& out, const Image& img1, int r) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    int n = sq(2*r+1);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int sum = 0;
            for (int y = i-r; y <= i+r; ++y) {
                for (int x = j-r; x <= j+r; ++x) {
                    sum += img1.lum(x,y);
                }
            }
            out.pix(j,i) = lum2i(sum/n);
        }

    }

    return out;
}

Image& Smooth::median(Image& out, const Image& img1, int r) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    uchar* vals = new uchar[sq(2*r+1)];
    for (int f = 0; f < 3; ++f) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int n = 0;
                for (int y = i-r; y <= i+r; ++y) {
                    for (int x = j-r; x <= j+r; ++x) {
                        vals[n] = img1.pix(x,y,f);
                        ++n;
                    }
                }
                out.pix(j,i,f) = median_slow(vals,n);
            }
        }
    }

    return out;
}

Image& Smooth::median_lum(Image& out, const Image& img1, int r) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    uchar* vals = new uchar[sq(2*r+1)];
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int n = 0;
            for (int y = i-r; y <= i+r; ++y) {
                for (int x = j-r; x <= j+r; ++x) {
                    vals[n] = img1.lum(x,y);
                    ++n;
                }
            }
            out.pix(j,i) = lum2i(median_slow(vals,n));
        }
    }

    return out;
}

Image& Smooth::sharpen3(Image& out, const Image& img1) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    const int r = 3;
    const float a = -0.1f;
    const float b = 2.f;
    const float kern[] = {
        0, 0, 0, a, 0, 0, 0,
        0, a, 0, 0, 0, a, 0,
        0, 0, 0, 0, 0, 0, 0,
        a, 0, 0, b, 0, 0, a,
        0, 0, 0, 0, 0, 0, 0,
        0, a, 0, 0, 0, a, 0,
        0, 0, 0, a, 0, 0, 0
    };
    const float ksum = 1.2f;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            float sum = 0;
            int n = 0;
            for (int y = i-r; y <= i+r; ++y) {
                for (int x = j-r; x <= j+r; ++x) {
                    sum += kern[n]*img1.lum(x,y);
                    ++n;
                }
            }
            out.pix(j,i) = lum2i(clamp_color(sum/ksum));
        }
    }

    return out;
}

Image& Smooth::gauss_2r(Image& out, const Image& img1) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    const int r = 2;
    const float kern[] = {
        2, 4, 5, 4, 2,
        4, 9,12, 9, 4,
        5,12,15,12, 5,
        4, 9,12, 9, 4,
        2, 4, 5, 4, 2
    };
    const float ksum = 159.f;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            float sum = 0;
            int n = 0;
            for (int y = i-r; y <= i+r; ++y) {
                for (int x = j-r; x <= j+r; ++x) {
                    sum += kern[n]*img1.lum(x,y);
                    ++n;
                }
            }
            out.pix(j,i) = lum2i(sum/ksum);
        }
    }

    return out;
}

Image& Smooth::gauss_slow(Image& out, const Image& img1, float s, int r) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    float* kern = new float[sq(2*r+1)];
    float ksc = func_gauss(0,0,s);
    int n = 0;
    float ksum = 0;
    const float center_val = 10.0f;
    for (int i = -r; i <= r; ++i) {
        for (int j = -r; j <= r; ++j) {
            kern[n] = center_val*func_gauss(j,i,s)/ksc;
            ksum += kern[n];
            ++n;
        }
    }

    for (int f = 0; f < 3; ++f) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                float sum = 0;
                int n = 0;
                for (int y = i-r; y <= i+r; ++y) {
                    for (int x = j-r; x <= j+r; ++x) {
                        sum += kern[n]*img1.pix(x,y,f);
                        ++n;
                    }
                }
                out.pix(j,i,f) = sum/ksum;
            }
        }
    }

    return out;
}

Image& Smooth::gauss_2wrap(Image& out, const Image& img1, float s, int r) {
    int width = img1.width;
    int height = img1.height;

    float* kern = new float[2*r+1];
    float ksc = func_gauss(0,0,s);
    int n = 0;
    float ksum = 0;
    const float center_val = 10.0f;
    for (int i = -r; i <= r; ++i) {
        kern[n] = center_val*func_gauss(i,s)/ksc;
        ksum += kern[n];
        ++n;
    }

    Image img_tmp(width, height);
    for (int f = 0; f < 3; ++f) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                float sum = 0;
                int n = 0;
                int y = i;
                for (int x = j-r; x <= j+r; ++x) {
                    sum += kern[n]*img1.pix(x,y,f);
                    ++n;
                }
                img_tmp.pix(j,i,f) = sum/ksum;
            }
        }

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                float sum = 0;
                int n = 0;
                int x = j;
                for (int y = i-r; y <= i+r; ++y) {
                    sum += kern[n]*img_tmp.pix(x,y,f);
                    ++n;
                }
                out.pix(j,i,f) = sum/ksum;
            }
        }
    }

    return out;
}

Image& Smooth::gauss_2wrap_lum(Image& out, const Image& img1, float s, int r) {
    int width = img1.width;
    int height = img1.height;

    float* kern = new float[2*r+1];
    float ksc = func_gauss(0,0,s);
    int n = 0;
    float ksum = 0;
    const float center_val = 10.0f;
    for (int i = -r; i <= r; ++i) {
        kern[n] = center_val*func_gauss(i,s)/ksc;
        ksum += kern[n];
        ++n;
    }

    Image img_tmp(width, height);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            float sum = 0;
            int n = 0;
            int y = i;
            for (int x = j-r; x <= j+r; ++x) {
                sum += kern[n]*img1.lum(x,y);
                ++n;
            }
            img_tmp.pix(j,i) = lum2i(sum/ksum);
        }
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            float sum = 0;
            int n = 0;
            int x = j;
            for (int y = i-r; y <= i+r; ++y) {
                sum += kern[n]*img_tmp.lum(x,y);
                ++n;
            }
            out.pix(j,i) = lum2i(sum/ksum);
        }
    }

    return out;
}

Image& Smooth::conservative(Image& out, const Image& img1, int r) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    int kern_size = sq(2*r+1);
    uchar* vals = new uchar[kern_size];
    for (int f = 0; f < 3; ++f) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int n = 0;
                for (int y = i-r; y <= i+r; ++y) {
                    for (int x = j-r; x <= j+r; ++x) {
                        if (!((x==j)&&(y==i))) {
                            vals[n] = img1.pix(x,y,f);
                            ++n;
                        }
                    }
                }
                uchar mini = arr_mini(vals,kern_size);
                uchar maxi = arr_maxi(vals,kern_size);
                out.pix(j,i,f) = clamp(img1.pix(j,i,f), vals[mini], vals[maxi]);
            }
        }
    }

    return out;
}

Image& Smooth::conservative_lum(Image& out, const Image& img1, int r) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    int kern_size = sq(2*r+1);
    uchar* vals = new uchar[kern_size];

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int n = 0;
            for (int y = i-r; y <= i+r; ++y) {
                for (int x = j-r; x <= j+r; ++x) {
                    if (!((x==j)&&(y==i))) {
                        vals[n] = img1.lum(x,y);
                        ++n;
                    }
                }
            }
            uchar mini = arr_mini(vals,kern_size);
            uchar maxi = arr_maxi(vals,kern_size);
            uchar gray = clamp(img1.lum(j,i), vals[mini], vals[maxi]);
            out.pix(j,i) = lum2i(gray);
        }
    }

    return out;
}

Image& Smooth::crimmins(Image& out, const Image& img1, int n) {
    int width = img1.width;
    int height = img1.height;

    //TODO: do we need - Image img(*this);
    Image img(img1);

    int dir[4][2][2] = {{{0,-1},{0,1}},
                        {{-1,0},{1,0}},
                        {{-1,-1},{1,1}},
                        {{1,-1},{-1,1}}};
    for (int iter = 0; iter < n; ++iter) {
        for (int f = 0; f < 3; ++f) {
            for (int d = 0; d < 4; ++d) {
                for (int cs = 0; cs < 8; ++cs) {
                    for (int i = 0; i < height; ++i) {
                        for (int j = 0; j < width; ++j) {
                            uchar& a = img.pix(j+dir[d][0][0], i+dir[d][0][1], f);
                            uchar& b = img.pix(j, i, f);
                            uchar& c = img.pix(j+dir[d][1][0], i+dir[d][1][1], f);
                            switch (cs) {
                            case 0: if (a>=b+2) ++b; break;
                            case 1: if (a>b && b<=c) ++b; break;
                            case 2: if (c>b && b<=a) ++b; break;
                            case 3: if (c>=b+2) ++b; break;

                            case 4: if (a<=b-2) --b; break;
                            case 5: if (a<b && b>=c) --b; break;
                            case 6: if (c<b && b>=a) --b; break;
                            case 7: if (c<=b-2) --b; break;
                            default:
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    out = img;
    return out;
}

Image& Smooth::crimmins_lum(Image& out, const Image& img1, int n) {
    int width = img1.width;
    int height = img1.height;

    //TODO: do we need - Image img(*this);
    Image img(img1);

    int dir[4][2][2] = {{{0,-1},{0,1}},
                        {{-1,0},{1,0}},
                        {{-1,-1},{1,1}},
                        {{1,-1},{-1,1}}};
    for (int iter = 0; iter < n; ++iter) {
        for (int d = 0; d < 4; ++d) {
            for (int cs = 0; cs < 8; ++cs) {
                for (int i = 0; i < height; ++i) {
                    for (int j = 0; j < width; ++j) {
                        uchar& a = img.pix(j+dir[d][0][0], i+dir[d][0][1], 0);
                        uchar& b = img.pix(j, i, 0);
                        uchar& c = img.pix(j+dir[d][1][0], i+dir[d][1][1], 0);
                        switch (cs) {
                        case 0: if (a>=b+2) ++b; break;
                        case 1: if (a>b && b<=c) ++b; break;
                        case 2: if (c>b && b<=a) ++b; break;
                        case 3: if (c>=b+2) ++b; break;

                        case 4: if (a<=b-2) --b; break;
                        case 5: if (a<b && b>=c) --b; break;
                        case 6: if (c<b && b>=a) --b; break;
                        case 7: if (c<=b-2) --b; break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            out.pix(j,i) = lum2i(img.pix(j,i,0));
        }
    }
    return out;
}

//TODO: Image& filter_frequency

Image& Smooth::laplac(Image& out, const Image& img1, int eges) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    int r = 1;
    float kern8[] = {1,1,1, 1,-8,1, 1,1,1};
    float kern4[] = {0,1,0, 1,-4,1, 0,1,0};
    float* kern = (eges ? kern8 : kern4);
//        for (int f = 0; f < 3; ++f) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                float sum = 0;
                int n = 0;
                for (int y = i-r; y <= i+r; ++y) {
                    for (int x = j-r; x <= j+r; ++x) {
                        //sum += kern[n]*pix(x, y, f);
                        sum += kern[n]*img1.lum(x, y);
                        ++n;
                    }
                }
                //out.pix(j,i,f) = std::abs(sum);
                //out.pix(j,i) = lum2i(std::abs(sum));
                out.pix(j,i) = lum2i(clamp_color(sum));
            }
        }
//        }

    return out;
}

Image& Smooth::laplac_of_gauss(Image& out, const Image& img1, float s, int r) {
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
            ksum += kern[k++];
        }
    }

    print_kernel(kern, r);

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
                out.pix(j,i) = lum2i(clamp_color(sum/ksum));
            }
        }
//        }

    return out;
}

} // namespace bacchus
