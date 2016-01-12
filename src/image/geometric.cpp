#include "geometric.h"
#include "image.h"

#include "math/mat3.h"

namespace bacchus {

Image& Geometric::scale_up(Image& out, const Image& img1, int w, int h, Filter filter) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    assert(w >= width);
    assert(h >= height);

    float x = w/(float)width;
    float y = h/(float)height;

    switch (filter) {
    case FILTER_NEAREST:
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                int a = j/x, b = i/y;
                out.pix(j,i) = img1.pix(a,b);
            }
        }
        break;

    case FILTER_LINEAR:
        if (w > width && h > height) {
            Image img(w, h);
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    int a = j/x;
                    float da = j/x-a;
                    for (int f = 0; f < img1.bytespp; ++f) {
                        img.pix(j,i,f) = (1.0f - da)*img1.pix(a,i,f) + da*img1.pix(a+1,i,f);
                    }
                }
            }
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    int b = i/y;
                    float db = i/y-b;
                    for (int f = 0; f < img1.bytespp; ++f) {
                        out.pix(j,i,f) = (1.0f - db)*img.pix(j,b,f) + db*img.pix(j,b+1,f);
                    }
                }
            }
        } else if (w > width && h == height) {
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    int a = j/x;
                    float da = j/x-a;
                    for (int f = 0; f < img1.bytespp; ++f) {
                        out.pix(j,i,f) = (1.0f - da)*img1.pix(a,i,f) + da*img1.pix(a+1,i,f);
                    }
                }
            }
        } else if (w == width && h > height) {
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    int b = i/y;
                    float db = i/y-b;
                    for (int f = 0; f < img1.bytespp; ++f) {
                        out.pix(j,i,f) = (1.0f - db)*img1.pix(j,b,f) + db*img1.pix(j,b+1,f);
                    }
                }
            }
        }
        break;
    default:
        // square artifacts
        // for (int i = 0; i < img.height; ++i) {
        //     for (int j = 0; j < img.width; ++j) {
        //         int a = j/x;int b = i/y;
        //         float da = j/x-a;float db = i/y-b;
        //         for (int f = 0; f < fmt; ++f) {
        //             img.pix(j,i,f)
        //                     = (1.0f - db)*(1.0f - da)*pix(a, b, f)
        //                     + (1.0f - db)*da*pix(a+1, b, f)
        //                     + db*pix(a, b+1, f);
        //         }
        //     }
        // }
        // *this = img;
        break;
    }

    return out;
}

Image& Geometric::scale_up_size(Image& out, const Image& img1, int w, int h, int dx, int dy, Filter filter) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    assert(w >= width);
    assert(h >= height);

    float x = w/(float)width;
    float y = h/(float)height;

    switch (filter) {
    case FILTER_NEAREST:
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int a = j/x+dx, b = i/y+dy;
                out.pix(j,i) = img1.pix(a,b);
            }
        }
        break;

    case FILTER_LINEAR:
        if (w > width && h > height) {
            Image img(w, h);
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    int a = j/x + dx;
                    float da = j/x - a + dx;
                    for (int f = 0; f < img1.bytespp; ++f) {
                        img.pix(j,i,f) = (1.0f - da)*img1.pix(a,i,f) + da*img1.pix(a+1,i,f);
                    }
                }
            }
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    int b = i/y + dy;
                    float db = i/y - b + dy;
                    for (int f = 0; f < img1.bytespp; ++f) {
                        out.pix(j,i,f) = (1.0f - db)*img.pix(j,b,f) + db*img.pix(j,b+1,f);
                    }
                }
            }
        } else if (w > width && h == height) {
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    int a = j/x + dx;
                    float da = j/x - a + dx;
                    for (int f = 0; f < img1.bytespp; ++f) {
                        out.pix(j,i,f) = (1.0f - da)*img1.pix(a,i,f) + da*img1.pix(a+1,i,f);
                    }
                }
            }
        } else if (w == width && h > height) {
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    int b = i/y;
                    float db = i/y-b;
                    for (int f = 0; f < img1.bytespp; ++f) {
                        out.pix(j,i,f) = (1.0f - db)*img1.pix(j,b,f) + db*img1.pix(j,b+1,f);
                    }
                }
            }
        }
        break;
    default:
        break;
    }

    return out;
}

Image& Geometric::scale_down(Image& out, const Image& img1, int w, int h, Filter filter) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    assert(w <= width);
    assert(h <= height);

    float x = (float)width/w;
    float y = (float)height/h;

    switch (filter) {
    case FILTER_NEAREST:
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                int a = j*x, b = i*y;
                out.pix(j,i) = img1.pix(a,b);
            }
        }
        break;

    case FILTER_LINEAR:
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                int a0 = j*x, a1 = (j+1)*x;
                int b0 = i*y, b1 = (i+1)*y;
                for (int f = 0; f < img1.bytespp; ++f) {
                    float tmp = 0;
                    int n = 0;
                    for (int ky = b0; ky < b1; ++ky) {
                        for (int kx = a0; kx < a1; ++kx) {
                            tmp += img1.pix(kx,ky,f);
                            ++n;
                        }
                        out.pix(j,i,f) = tmp/n;
                    }
                }
            }
        }
        break;

    default:
        break;
    }

    return out;
}

Image& Geometric::scale(Image& out, const Image& img1, int w, int h, Filter filter) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    if ((w == width) && (h == height)) {
        out = img1;
    } else if ((w >= width) && (h >= height)) {
        scale_up(out, img1, w, h, filter);
    } else if ((w <= width) && (h <= height)) {
        scale_down(out, img1, w, h, filter);
    } else if ((w >= width) && (h <= height)) {
        Image img_tmp(w, height);
        scale_up(img_tmp, img1, w, height, filter);
        scale_down(out, img_tmp, w, h, filter);
    } else if ((w <= width) && (h >= height)) {
        Image img_tmp(width, h);
        scale_up(img_tmp, img1, width, h, filter);
        scale_down(out, img_tmp, w, h, filter);
    }

    return out;
}

Image &Geometric::flip_vert(Image &out, const Image &img1) {
    ulong line_memsize = img1.width*img1.bytespp;
    uchar* line = new uchar[line_memsize];
    uint half = img1.height/2;
    for (uint i = 0; i < half; ++i) {
        ulong l1 = i*line_memsize;
        ulong l2 = (img1.height-1-i)*line_memsize;
        memmove(line,           img1.dat8+l1, line_memsize);
        memmove(img1.dat8+l1,   img1.dat8+l2, line_memsize);
        memmove(img1.dat8+l2,           line, line_memsize);
    }
    delete[] line;
    return out;
}

Image& Geometric::rotate(Image& out, const Image& img1, int x, int y, float dg) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    float rd = deg2rad(dg);
    float co = std::cos(rd);
    float si = std::sin(rd);

    for (int y1 = 0; y1 < height; ++y1) {
        for (int x1 = 0; x1 < width; ++x1) {
            float x2 = co*(x1-x) - si*(y1-y) + x;// + 0.5f
            float y2 = si*(x1-x) + co*(y1-y) + y;// + 0.5f
            //float dx = x2 - floor(x2) - 0.5f;
            //float dy = y2 - floor(y2) - 0.5f;
            //for (int f = 0; f < fmt; ++f) {
            if (x2>=0 && x2<width && y2>=0 && y2<height) {
                out.pix(x1,y1) = img1.pix(x2,y2);

                // re-sampling same result
                //if (dx>0.5f && dy>0.5f)
                //    img.pix(x1,y1,f) = (1.0f-sqrt(dx*dx+dy*dy))*pix(x2,y2,f) + sqrt(dx*dx+dy*dy)*pix(x2+1,y2+1,f);
                //else if (dx>0.5f)
                //    img.pix(x1,y1,f) = (1.0f-dx)*pix(x2,y2,f) + dx*pix(x2+1,y2,f);
                //else if (dy>0.5f)
                //    img.pix(x1,y1,f) = (1.0f-dy)*pix(x2,y2,f) + dy*pix(x2,y2+1,f);
                //if (dx<-0.5f && dy<-0.5f)
                //    img.pix(x1,y1,f) = (1.0f-sqrt(dx*dx+dy*dy))*pix(x2,y2,f) + sqrt(dx*dx+dy*dy)*pix(x2-1,y2-1,f);
                //else if (dx<-0.5f)
                //    img.pix(x1,y1,f) = (1.0f+dx)*pix(x2,y2,f) - dx*pix(x2-1,y2,f);
                //else if (dy<-0.5f)
                //    img.pix(x1,y1,f) = (1.0f+dy)*pix(x2,y2,f) - dy*pix(x2,y2-1,f);
                //else
                //    img.pix(x1,y1,f) = pix(x2,y2,f);

            } else {
                out.pix(x1,y1) = lum2i(0);
            }
            //}
        }
    }

    return out;
}

Image& Geometric::reflect(Image& out, const Image& img1, int x, int y, float dg) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    float rd = deg2rad(dg);
    float co = std::cos(rd);
    float si = std::sin(rd);

    for (int y1 = 0; y1 < height; ++y1) {
        for (int x1 = 0; x1 < width; ++x1) {
            float dd = (x1-x)*si - (y1-y)*co;
            float x2 = x1 - 2.0f*dd*si;
            float y2 = y1 + 2.0f*dd*co;
            if (x2>=0 && x2<width && y2>=0 && y2<height) {
                out.pix(x1,y1) = img1.pix(x2,y2);
            } else {
                out.pix(x1,y1) = lum2i(0);
            }
        }
    }

    return out;
}

Image& Geometric::translate(Image& out, const Image& img1, int x, int y) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    for (int y1 = 0; y1 < height; ++y1) {
        for (int x1 = 0; x1 < width; ++x1) {
            int x2 = x1 + x;
            int y2 = y1 + y;
            if (x2>=0 && x2<width && y2>=0 && y2<height) {
                out.pix(x1,y1) = img1.pix(x2,y2);
            } else {
                out.pix(x1,y1) = lum2i(0);
            }
        }
    }

    return out;
}

Image& Geometric::affine(Image& out, const Image& img1, float a00, float a01, float a10, float a11, float b0, float b1) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;
    for (int y1 = 0; y1 < height; ++y1) {
        for (int x1 = 0; x1 < width; ++x1) {
            float x2 = a00*x1 + a01*y1 + b0;
            float y2 = a10*x1 + a11*y1 + b1;
            if (x2>=0 && x2<width && y2>=0 && y2<height) {
                out.pix(x1,y1) = img1.pix(x2,y2);
            } else {
                out.pix(x1,y1) = lum2i(0);
            }
        }
    }
    return out;
}

Image& Geometric::affine(Image& out, const Image& img1, int x10, int y10, int x11, int y11, int x12, int y12,
                         int x20, int y20, int x21, int y21, int x22, int y22)
{
    mat3f m0(x20,y20,1, x21,y21,1, x22,y22,1);
    vec3f v0(x10,x11,x12);
    vec3f a0 = m0.inverse()*v0;

    mat3f m1(x20,y20,1, x21,y21,1, x22,y22,1);
    vec3f v1(y10,y11,y12);
    vec3f a1 = m1.inverse()*v1;

    return affine(out, img1, a0.x, a0.y, a1.x, a1.y, a0.z, a1.z);
}

} // namespace bacchus
