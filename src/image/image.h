#pragma once

#include <string.h>

#include "math/mdefs.h"
#include "math/vec2.h"
#include "math/vec4.h"

#include "utils/logger.h"

namespace bacchus {

inline int stride(int x, int a) { return (x + (a-1)) & (~(a-1)); }
// img_stride = stride(w, 64);

struct Image {
    // bin, gray, rgb, int
    enum Format {
        FMT_LUM     = 1,
        FMT_LUMA    = 2,
        FMT_RGB     = 3,
        FMT_RGBA    = 4,
        FMT_BGR     ,
        FMT_BGRA    ,
//        FMT_BIN     ,// binary image
//        FMT_INT     ,// store int
//        FMT_FLOAT    // store float
    };

    uint width, height;
    uchar bytespp;
    Format format;

    union {
        uchar* dat8;
        uint* dat32;
        int* dat32s;
        vec4i* dat4v;
        vec3i* dat3v;
    };

    uchar* data() { return dat8; }
    const uchar* data() const { return dat8; }

    Image(): width(0), height(0), bytespp(FMT_RGBA), format(FMT_RGBA), dat8(nullptr) { }

    Image(uint w, uint h, const uchar* idata=nullptr)
        : width(w), height(h), bytespp(FMT_RGBA), format(FMT_RGBA), dat8(nullptr)
    {
        assert(width>0 && height>0);
        dat8 = new uchar[memsize()];
        if (idata) {
            memcpy(dat8, idata, memsize()*sizeof(uchar));//TODO: what if idata not long enough
        } else {
            memset(dat8, 0, memsize()*sizeof(uchar));
        }
    }

    Image(const Image& img)
        : width(img.width), height(img.height), bytespp(img.bytespp), format(img.format), dat8(nullptr)
    {
        assert(width>0 && height>0);// no copy 0x0 data
        dat8 = new uchar[memsize()];
        memcpy(dat8, img.dat8, memsize()*sizeof(uchar));//TODO: must give guaranties for data long enough
    }

    Image& operator=(const Image& img) {
        // no copy self
        if (this == &img)
            return *this;

        assert(img.width>0 && img.height>0);// no copy zero data
        format = img.format;
        if (width != img.width || height != img.height || bytespp != img.bytespp) {
            width = img.width;
            height = img.height;
            bytespp = img.bytespp;
            if (dat8)
                delete[] dat8;
            dat8 = new uchar[memsize()];
        }
        memcpy(dat8, img.dat8, memsize()*sizeof(uchar));
        return *this;
    }

    ~Image() {
        if (dat8) delete[] dat8;
    }

    bool valid() const { return (width && height); }
    void invalidate() { width = 0; height = 0; }
    vec2i resolution() const { return vec2i(width,height); }

    int size() const { return width*height; }
    int memsize() const { return size()*bytespp; }

    // GL_CLAMP_TO_EDGE
    uint idx32(int x, int y) const {
        int x1 = clamp(x,0,width-1);
        int y1 = clamp(y,0,height-1);
        return y1*width + x1;
    }

    uint idx8(int x, int y) const { return bytespp*idx32(x,y); }
    uint idx32(const vec2i& p) const { return idx32(p.x, p.y); }
    uint idx8(const vec2i& p) const { return idx8(p.x, p.y); }

    Image& resize(uint w, uint h) {
        assert(w>0 && w>0);
        if (width != w || height != h) {
            width = w;
            height = h;
            if (dat8)
                delete[] dat8;
            dat8 = new uchar[memsize()];
        }
        memset(dat8, 0, memsize()*sizeof(uchar));
        return *this;
    }

    Image& clear() {
        memset(dat8, 0, memsize()*sizeof(uchar));
        return *this;
    }

    Image& to_a1() {
        for (int i = 0; i < memsize(); i+=bytespp) {
            dat8[i+3] = 255;
        }
        return *this;
    }

    Image& to_gray() {
        for (int i = 0; i < memsize(); i+=bytespp) {
            uchar tmp = luminance(dat8[i], dat8[i+1], dat8[i+2]);
            dat8[i] = tmp;
            dat8[i+1] = tmp;
            dat8[i+2] = tmp;
        }
        return *this;
    }

    // pixel getters seters
    uchar pix(int x, int y, int p) const { return dat8[idx8(x,y)+p]; }
    uint pix(int x, int y) const { return dat32[idx32(x,y)]; }
    uchar lum(int x, int y) const { return pix(x,y, 0); }

    uchar pix(const vec2i& pt, int p) const { return pix(pt.x, pt.y, p); }
    uint pix(const vec2i& pt) const { return pix(pt.x, pt.y); }
    uchar lum(const vec2i& pt) const { return lum(pt.x, pt.y); }

    vec4i rgba(const vec2i& pt) const {
        return dat4v[idx32(pt)];
//        int t = idx8(pt);
//        if (bytespp == FMT_LUM)
//            return vec4i(dat8[t+0], dat8[t+0], dat8[t+0], 255);
//        else if (bytespp == FMT_LUMA)
//            return vec4i(dat8[t+0], dat8[t+0], dat8[t+0], dat8[t+1]);
//        else if (bytespp == FMT_RGB && format == FMT_BGR)
//            return vec4i(dat8[t+2], dat8[t+1], dat8[t+0], 255);
//        else if (bytespp == FMT_RGBA && format == FMT_BGRA)
//            return vec4i(dat8[t+2], dat8[t+1], dat8[t+0], dat8[t+3]);
//        else if (bytespp == FMT_RGB)
//            return vec4i(dat8[t+0], dat8[t+1], dat8[t+2], 255);
//        else if (bytespp == FMT_RGBA)
//            return vec4i(dat8[t+0], dat8[t+1], dat8[t+2], dat8[t+3]);
//        else
//            assert("wrong bytespp");
//        return vec4i(0);
    }

    vec3i rgb(const vec2i& pt) const {
//        int t = idx32(pt);
//        int sz = size();
//        if (bytespp == FMT_RGB)
//            return dat3v[idx32(pt)];
//        else if (bytespp == FMT_RGBA)
//            //return *((vec3i*)(&dat4v[idx32(pt)]));
//            return dat4v[idx32(pt)].xyz();
//        return vec3i(0);

        int t = idx8(pt);
        if (bytespp == FMT_LUM)
            return vec3i(dat8[t+0], dat8[t+0], dat8[t+0]);
        else if (bytespp == FMT_LUMA)
            return vec3i(dat8[t+0], dat8[t+0], dat8[t+0]);
        else if (bytespp == FMT_RGB && format == FMT_BGR)
            return vec3i(dat8[t+2], dat8[t+1], dat8[t+0]);
        else if (bytespp == FMT_RGBA && format == FMT_BGRA)
            return vec3i(dat8[t+2], dat8[t+1], dat8[t+0]);
        else if (bytespp == FMT_RGB)
            return vec3i(dat8[t+0], dat8[t+1], dat8[t+2]);
        else if (bytespp == FMT_RGBA)
            return vec3i(dat8[t+0], dat8[t+1], dat8[t+2]);
        else
            assert("wrong bytespp");
        return vec3i(0);
    }

    vec4i& rgba(const vec2i& pt) {
        return dat4v[idx32(pt)];
    }

    uchar& pix(int x, int y, int p) { return dat8[idx8(x,y)+p]; }
    uint& pix(int x, int y) { return dat32[idx32(x,y)]; }

    uchar& pix(const vec2i& pt, int p) { return pix(pt.x, pt.y, p); }
    uint& pix(const vec2i& pt) { return pix(pt.x, pt.y); }

    int& spix(int x, int y) { return dat32s[idx32(x,y)]; }
    int spix(int x, int y) const { return dat32s[idx32(x,y)]; }

    vec3f rgb_uvf(const vec2f& uv) const {
        vec2f xy = uv*vec2f(resolution());
        vec3f rgbuc = rgb(xy);
        return rgbuc/255.f;

        /// interpolated
//        vec2f fuv = (uv + vec2f(1000.f) + vec2f(0.5f,0.f))*vec2f(sizewh());
//        vec2f fracuv[2];
//        fracuv[0] = frac(fuv);
//        fracuv[1] = vec2f(1.f) - fracuv[0];
//        vec2i iuv[2];
//        iuv[0] = mod(fuv, sizewh());
//        iuv[1] = (iuv[0] + vec2i(1)) % sizewh();
//        vec3f sum(0.f);
//        for (int i = 0; i < 4; ++i) {
//            int a = i%2; int b = i/2;
//            sum += fracuv[a].y * fracuv[b].x * rgb(vec2i(iuv[a].x, iuv[b].y));
//        }
//        return sum/255.f;
    }
};

inline int read_raw(const char* file_name, uint8_t* buff, int w, int h) {
    (void)w; (void)h;
    FILE* fp = fopen(file_name, "rb");
    if (fp == nullptr) {
        LOGE("Fail open file: %s\n", file_name);
        return -1;
    }

    // obtain file size
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    rewind(fp);

    // read buffer
    fread(buff, 1, sz, fp);

    // terminate
    fclose(fp);
    return 0;
}

} // namespace bacchus
