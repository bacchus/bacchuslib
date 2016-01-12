#include "morphology.h"

#include <iostream>

#include "image.h"

namespace bacchus {

Image& Morphology::dilate(Image& out, const Image& img1, int r) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    //const int bg = 0;
    const int fg = 255;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            uchar isfg = 0;
            for (int y = i-r; y <= i+r && isfg==0; ++y) {
                for (int x = j-r; x <= j+r && isfg==0; ++x) {
                    if (img1.lum(x,y)==fg) isfg=255;
                }
            }
            out.pix(j,i) = lum2i(isfg);
        }
    }

    return out;
}

Image& Morphology::erode(Image& out, const Image& img1, int r) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    const int bg = 0;
    //const int fg = 255;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            uchar isfg = 255;
            for (int y = i-r; y <= i+r && isfg; ++y) {
                for (int x = j-r; x <= j+r && isfg; ++x) {
                    if (img1.lum(x,y)==bg) isfg=0;
                }
            }
            out.pix(j,i) = lum2i(isfg);
        }
    }

    return out;
}

Image& Morphology::open(Image& out, const Image& img1, int r) {
    Image img_tmp(img1.width, img1.height);
    erode(img_tmp, img1, r);
    return dilate(out, img_tmp, r);
}

Image& Morphology::close(Image& out, const Image& img1, int r) {
    Image img_tmp(img1.width, img1.height);
    dilate(img_tmp, img1, r);
    return erode(out, img_tmp, r);
}

Image& Morphology::hit_miss_corners(Image& out, const Image& img1) {
    assert(&img1 != &out);
    int width = img1.width;
    int height = img1.height;

    const uchar fg = 255;
    const uchar bg = 0;
    const uchar dk = 1;

    uchar kere[9] = {
        0, 1, 0,
        1, 1, 1,
        1, 1, 0
    };

    uchar elem[9] = {
        dk, fg, dk,
        bg, fg, fg,
        bg, bg, dk
    };

    int d[9][2] = {{-1,-1}, { 0,-1}, {+1,-1},
                   {-1, 0}, { 0, 0}, {+1, 0},
                   {-1,+1}, { 0,+1}, {+1,+1}};

    for (int rot = 0; rot < 4; ++rot) {
//            int catched = 0;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int sum = 0;
                for (int k = 0; k < 9; ++k) {
                    sum += kere[k]*(elem[k]^img1.pix(j+d[k][0], i+d[k][1], 0));
                }
                if (sum==0) {
//                        ++catched;
                    out.pix(j,i) = lum2i(fg);
                }
            }
        }
//            std::cout<<"catched: "<<catched<<std::endl;
        elem9_rot90(kere);
        elem9_rot90(elem);
    }

    return out;
}

Image& Morphology::thinn(Image& out, const Image& img1) {
    int width = img1.width;
    int height = img1.height;

    const uchar fg = 255;
    const uchar bg = 0;
    const uchar dk = 1;

    uchar kere[2][9] = {
        {
            1, 1, 1,
            0, 1, 0,
            1, 1, 1
        },{
            0, 1, 0,
            1, 1, 1,
            1, 1, 0
        }
    };

    uchar elem[8][9] = {
        {
            fg, fg, fg,
            dk, fg, dk,
            bg, bg, bg
        },{
            dk, fg, dk,
            bg, fg, fg,
            bg, bg, dk
        }
    };

    int d[9][2] = {{-1,-1}, { 0,-1}, {+1,-1},
                   {-1, 0}, { 0, 0}, {+1, 0},
                   {-1,+1}, { 0,+1}, {+1,+1}};

    Image img(img1);
    int pass = 0;
    int catched = 1;
    while (catched && pass<1000) {
        catched = 0;
        for (int rot = 0; rot < 4; ++rot) {
            for (int el = 0; el < 2; ++el) {
                for (int i = 0; i < height; ++i) {
                    for (int j = 0; j < width; ++j) {
                        int sum = 0;
                        for (int k = 0; k < 9; ++k) {
                            sum += kere[el][k]*(elem[el][k]^img.pix(j+d[k][0], i+d[k][1], 0));
                        }
                        if (sum==0) {
                            ++catched;
                            img.pix(j,i) = lum2i(bg);
                        }
                    }
                }
            }
            elem9_rot90(kere[0]);
            elem9_rot90(kere[1]);
            elem9_rot90(elem[0]);
            elem9_rot90(elem[1]);
        }
        std::cout<<pass<<" catched: "<<catched<<std::endl;
        ++pass;
    }

    out = img;
    return out;
}

Image& Morphology::thick(Image& out, const Image& img1) {
    int width = img1.width;
    int height = img1.height;

    const uchar fg = 255;
    const uchar bg = 0;
    const uchar dk = 1;

    uchar kere[2][9] = {
        {
            1, 1, 0,
            1, 1, 0,
            1, 0, 1
        },{
            0, 1, 1,
            0, 1, 1,
            1, 0, 1
        }
    };

    uchar elem[2][9] = {
        {
            fg, fg, dk,
            fg, bg, dk,
            fg, dk, bg
        },{
            dk, fg, fg,
            dk, bg, fg,
            bg, dk, fg
        }
    };

    int d[9][2] = {{-1,-1}, { 0,-1}, {+1,-1},
                   {-1, 0}, { 0, 0}, {+1, 0},
                   {-1,+1}, { 0,+1}, {+1,+1}};

    Image img(img1);
    int pass = 0;
    int catched = 1;
    while (catched && pass<1000) {
        catched = 0;
        for (int rot = 0; rot < 4; ++rot) {
            for (int el = 0; el < 2; ++el) {
                for (int i = 0; i < height; ++i) {
                    for (int j = 0; j < width; ++j) {
                        int sum = 0;
                        for (int k = 0; k < 9; ++k) {
                            sum += kere[el][k]*(elem[el][k]^img.pix(j+d[k][0], i+d[k][1], 0));
                        }
                        if (sum==0) {
                            ++catched;
                            img.pix(j,i) = lum2i(fg);
                        }
                    }
                }
            }
            elem9_rot90(kere[0]);
            elem9_rot90(kere[1]);
            elem9_rot90(elem[0]);
            elem9_rot90(elem[1]);
        }
        std::cout<<pass<<" catched: "<<catched<<std::endl;
        ++pass;
    }
    out = img;
    return out;
}

Image& Morphology::skiz(Image& out, const Image& img1) {
    int width = img1.width;
    int height = img1.height;

    const uchar fg = 255;
    const uchar bg = 0;
    const uchar dk = 1;

    uchar kere[4][9] = {
        {
            1, 1, 1,
            0, 1, 0,
            1, 1, 1
        },{
            0, 1, 0,
            1, 1, 1,
            1, 1, 0
        },{
            1, 0, 0,
            1, 1, 1,
            1, 1, 1
        },{
            0, 0, 1,
            1, 1, 1,
            1, 1, 1
        }
    };

    uchar elem[4][9] = {
        {
            bg, bg, bg,
            dk, bg, dk,
            fg, fg, fg
        },{
            dk, bg, dk,
            fg, bg, bg,
            fg, fg, dk
        },{
            fg, dk, dk,
            fg, bg, fg,
            fg, fg, fg
        },{
            dk, dk, fg,
            fg, bg, fg,
            fg, fg, fg
        }
    };

    int d[9][2] = {{-1,-1}, { 0,-1}, {+1,-1},
                   {-1, 0}, { 0, 0}, {+1, 0},
                   {-1,+1}, { 0,+1}, {+1,+1}};

    Image img(img1);
    for (int iter = 0; iter < 2; ++iter) {
        int pass = 0;
        int catched = 1;
        while (catched) {
            catched = 0;
            for (int rot = 0; rot < 4; ++rot) {
                for (int el = 0; el < 2; ++el) {
                    for (int i = 0; i < height; ++i) {
                        for (int j = 0; j < width; ++j) {
                            int sum = 0;
                            for (int k = 0; k < 9; ++k) {
                                sum += kere[el+2*iter][k]*(elem[el+2*iter][k]^img.pix(j+d[k][0], i+d[k][1], 0));
                            }
                            if (sum==0) {
                                ++catched;
                                img.pix(j,i) = lum2i(fg);
                            }
                        }
                    }
                }
                elem9_rot90(kere[0+2*iter]);
                elem9_rot90(kere[1+2*iter]);
                elem9_rot90(elem[0+2*iter]);
                elem9_rot90(elem[1+2*iter]);
            }
            std::cout<<iter<<" "<<pass<<" catched: "<<catched<<std::endl;
            ++pass;
        }
    }
    out = img;
    return out;
}

} // namespace bacchus
