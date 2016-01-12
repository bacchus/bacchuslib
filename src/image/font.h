#pragma once

#include "image.h"
#include "math/mdefs.h"

namespace bacchus {

class Font {
private:
    static const uint data_size = 51;
    uint data[data_size] = {
        0x0118fe2e, 0x00f8be2f, 0x01e0843e, 0x00f8c62f, 0x01f0bc3f,
        0x00109c3f, 0x00e8f43e, 0x0118fe31, 0x00421084, 0x00742108,
        0x00928ca9, 0x01f08421, 0x0118d6ab, 0x011cd671, 0x00e8c62e,
        0x0010be2f, 0x01ecc62e, 0x0092be2f, 0x00f8383e, 0x0042109f,
        0x01e8c631, 0x00452a31, 0x00aad631, 0x01151151, 0x00e87a31,
        0x01f1111f, 0x00e9d72e, 0x00e210c4, 0x01f1322e, 0x00f8320f,
        0x00847d21, 0x00f83c3f, 0x00e8bc3e, 0x0042221f, 0x00e8ba2e,
        0x00e87a2e, 0x00401084, 0x0040220e, 0x00401000, 0x00e03800,
        0x00440000, 0x00400000, 0x00003800, 0x00821088, 0x00442104,
        0x00000000, 0x00afabea, 0x00000084, 0x000abaa0, 0x00111110,
        0x00023880
    };
    uint transl[256];
public:
    static const uint width = 14;

    Font() {
        const char c[sizeof(data)+1] = "abcdefghijklmnopqrstuvwxyz0123456789!?:=,.-() #'*/+";
        for (uint i = 0; i < 256; ++i) transl[i] = 45;
        for (uint i = 0; i < sizeof(data); ++i) transl[(uchar)c[i]] = i;
    }

    void print(Image& img, const char* str, const vec2i& pt, uint color) {
        vec2i wd;
        for (uint i = 0; i < strlen(str); ++i) {
            int idx = 0;
            if (str[i] >= 'A' && str[i] <= 'Z')
                idx = transl[(uchar)str[i] - ('A'-'a')];
            else
                idx = transl[(uchar)str[i]];
            uint a = data[idx];
            vec2i d;
            for (d.y = 6; d.y > 0; --d.y) {
                for (d.x = 0; d.x < 5; ++d.x) {
                    if (a%2) {
                        img.pix(pt + 2*d + wd) = color;
                        img.pix(pt+vec2i(1,0) + 2*d + wd) = color;
                        img.pix(pt+vec2i(0,1) + 2*d + wd) = color;
                        img.pix(pt+vec2i(1,1) + 2*d + wd) = color;
                    }
                    a>>=1;
                }
            }
            wd.x += width;
        }
    }
};

} // namespace bacchus
