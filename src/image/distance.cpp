#include "image/distance.h"

#include <limits>
#include <iostream>
#include <iomanip>

#include "image.h"

namespace bacchus {

Image& Distance::transform_wave(Image& out, const Image& img1) {
    int width = img1.width;
    int height = img1.height;

    Image img(img1);
    const int r = 3;
    // eucledian distances x10
    uint kern1[] = {0,10,20, 10,14,22, 20,22,28};

    const uint UNDEF = std::numeric_limits<uint>::max();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // 128 - is threshod - use bin image as input
            if (img1.lum(j,i) > 128) {
                img.pix(j,i) = UNDEF;
            } else if (img1.lum(j,i) <= 128) {
                img.pix(j,i) = 0;
            }
        }
    }

    uint maxd = 0;// to convert to gray scale

    //top left to bottom right and reverse faster but top-right and bl smth wrong

    //keep their value (if it is known and smaller)
    //or sum mask-value and cell-value (if it is known)
    //from the cell below the mask-0-cell
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            uint base = img.pix(j,i);
            if (base != UNDEF) {
                int k = 0;
                for (int y = i; y < i+r; ++y) {
                    for (int x = j; x < j+r; ++x) {
                        if (x<width && y<height) {
                            uint l = img.pix(x,y);
                            if (l==UNDEF || (l > kern1[k] + base)) {
                                img.pix(x,y) = kern1[k] + base;
                                if (img.pix(x,y) > maxd) maxd = img.pix(x,y);
                            }
                        }
                        ++k;
                    }
                }
            }
        }
    }

    for (int i = height-1; i >= 0; --i) {
        for (int j = width-1; j >= 0; --j) {
            uint base = img.pix(j,i);
            if (base != UNDEF) {
                int k = 0;
                for (int y = i; y > i-r; --y) {
                    for (int x = j; x > j-r; --x) {
                        if (x>=0 && y>=0) {
                            uint l = img.pix(x,y);
                            if (l==UNDEF || (l > kern1[k] + base)) {
                                img.pix(x,y) = kern1[k] + base;
                                if (img.pix(x,y) > maxd) maxd = img.pix(x,y);
                            }
                        }
                        ++k;
                    }
                }
            }
        }
    }

//        for (int i = 0; i < height; ++i) {
//            for (int j = 0; j < width; ++j) {
//                std::cout<<std::setw(2)<<img.pix(j,i)<<" ";
//            }
//            std::cout<<std::endl;
//        }
//        std::cout<<std::endl;

    // convert to gray scale
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (img.pix(j,i) == UNDEF) {
                // must not happen
                out.pix(j,i) = rgba2i(200,150,50);
            } else if (img.pix(j,i) == 0) {
                out.pix(j,i) = lum2i(0);
            } else {
                uint d = img.pix(j,i);
                uint l = d*255.0f/maxd;
                out.pix(j,i) = lum2i(l);
            }
        }
    }

    return out;
}

Image& Distance::transform(Image& out, const Image& img1) {
    // two variants:
    // 1. point causes all around to change - wave
    // 2. point changes under influence of others - how to call this
    int width = img1.width;
    int height = img1.height;

    Image img(img1);
    const uint UD = std::numeric_limits<uint>::max()/2;
    const int r = 2;
    uint maxd = 0;// for convert to gray scale

    // eucledian distances x10
    uint kern1[] = {
        28,22,20,22,28,
        22,14,10,14,22,
        20,10, 0,10,20,
        22,14,10,14,22,
        28,22,20,22,28
    };

    // make it binary
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // 128 - is threshod - use bin image as input
            if (img1.lum(j,i) > 128) {
                img.pix(j,i) = UD;
            } else if (img1.lum(j,i) <= 128) {
                img.pix(j,i) = 0;
            }
        }
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int k = 0;
            uint dmin = UD;
            for (int y = i-r; y <= i+r; ++y) {
                for (int x = j-r; x <= j+r; ++x) {
                    uint l = img.pix(x,y);
                    if (l!=UD) {
                        uint d = kern1[k] + l;
                        if (d < dmin) {
                            dmin = d;
                        }
                    }
                    ++k;
                }
            }
            img.pix(j,i) = dmin;
            if (dmin > maxd) maxd = dmin;
        }
    }

    for (int i = height-1; i >= 0; --i) {
        for (int j = width-1; j >= 0; --j) {
            int k = 0;
            uint dmin = UD;
            for (int y = i+r; y >= i-r; --y) {
                for (int x = j+r; x >= j-r; --x) {
                    uint l = img.pix(x,y);
                    if (l!=UD) {
                        uint d = kern1[k] + l;
                        if (d < dmin) {
                            dmin = d;
                        }
                    }
                    ++k;
                }
            }
            img.pix(j,i) = dmin;
            if (dmin > maxd) maxd = dmin;
        }
    }
    // convert to gray scale
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (img.pix(j,i) == UD) {
                // must not happen
                out.pix(j,i) = rgba2i(200,150,50);
            } else if (img.pix(j,i) == 0) {
                out.pix(j,i) = lum2i(0);
            } else {
                uint d = img.pix(j,i);
                uint l = d*255.0f/maxd;
                out.pix(j,i) = lum2i(l);
            }
        }
    }

    return out;
}

void Distance::transform_test(const Image& img1) {
    int width = img1.width;
    int height = img1.height;

    const uint UD = std::numeric_limits<uint>::max()/2;
    const int im_cnt = 3;
    const int r = 2;
    uint maxd = 0;

    Image img[im_cnt] = {img1, img1, img1};

    const char* names[im_cnt] = {"eucled", "square", "cross"};

    uint kern1[] = {
        28,22,20,22,28,
        22,14,10,14,22,
        20,10, 0,10,20,
        22,14,10,14,22,
        28,22,20,22,28
    };

    uint kern2[] = {
        20,20,20,20,20,
        20,10,10,10,20,
        20,10, 0,10,20,
        20,10,10,10,20,
        20,20,20,20,20
    };

    uint kern3[] = {
        UD,UD,20,UD,UD,
        UD,UD,10,UD,UD,
        20,10, 0,10,20,
        UD,UD,10,UD,UD,
        UD,UD,20,UD,UD
    };

    uint* kern[im_cnt] = {kern1, kern2, kern3};

    // make it binary
    for (int im = 0; im < im_cnt; ++im) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                // 128 - is threshod - use bin image as input
                if (img1.lum(j,i) > 128) {
                    img[im].pix(j,i) = UD;
                } else if (img1.lum(j,i) <= 128) {
                    img[im].pix(j,i) = 0;
                }
            }
        }
    }

    for (uint i = 0; i < img[0].height; ++i) {
        for (uint j = 0; j < img[0].width; ++j) {
            std::cout<<std::setw(2)<<(img[0].pix(j,i)==UD?1:0)<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;

    for (int im = 0; im < im_cnt; ++im) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int k = 0;
                uint dmin = UD;
                for (int y = i-r; y <= i+r; ++y) {
                    for (int x = j-r; x <= j+r; ++x) {
                        uint l = img[im].pix(x,y);
                        if (l!=UD) {
                            uint d = kern[im][k] + l;
                            if (d < dmin) {
                                dmin = d;
                            }
                        }
                        ++k;
                    }
                }
                img[im].pix(j,i) = dmin;
                if (dmin > maxd) maxd = dmin;
            }
        }

        for (int i = height-1; i >= 0; --i) {
            for (int j = width-1; j >= 0; --j) {
                int k = 0;
                uint dmin = UD;
                for (int y = i+r; y >= i-r; --y) {
                    for (int x = j+r; x >= j-r; --x) {
                        uint l = img[im].pix(x,y);
                        if (l!=UD) {
                            uint d = kern[im][k] + l;
                            if (d < dmin) {
                                dmin = d;
                            }
                        }
                        ++k;
                    }
                }
                img[im].pix(j,i) = dmin;
                if (dmin > maxd) maxd = dmin;
            }
        }
    }

    for (int im = 0; im < im_cnt; ++im) {
        std::cout<<names[im]<<std::endl;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                  std::cout<<std::setw(2)<<img[im].pix(j,i)<<" ";
            }
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }

    // show difs
//        for (int i = 0; i < height; ++i) {
//            for (int j = 0; j < width; ++j) {
//                std::cout<<std::setw(2)<<(img[0].pix(j,i)-img[1].pix(j,i))<<" ";
//            }
//            std::cout<<std::endl;
//        }
//        std::cout<<std::endl;

}

} // namespace bacchus
