#include "arthops.h"
#include "image.h"

namespace bacchus {

Image& Arthops::add(Image& out, const Image& img1, const Image& img2) {
    assert(img1.width == img2.width);
    assert(img1.height == img2.height);
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        out.dat8[i  ] = std::min(img1.dat8[i  ] + img2.dat8[i  ], 255);
        out.dat8[i+1] = std::min(img1.dat8[i+1] + img2.dat8[i+1], 255);
        out.dat8[i+2] = std::min(img1.dat8[i+2] + img2.dat8[i+2], 255);
    }
    return out;
}

Image& Arthops::add(Image& out, const Image& img1, uchar c) {
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        out.dat8[i  ] = std::min(img1.dat8[i  ] + c, 255);
        out.dat8[i+1] = std::min(img1.dat8[i+1] + c, 255);
        out.dat8[i+2] = std::min(img1.dat8[i+2] + c, 255);
    }
    return out;
}

Image& Arthops::sub(Image& out, const Image& img1, const Image& img2) {
    assert(img1.width == img2.width);
    assert(img1.height == img2.height);
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        out.dat8[i  ] = std::max(img1.dat8[i  ] - img2.dat8[i  ], 0);
        out.dat8[i+1] = std::max(img1.dat8[i+1] - img2.dat8[i+1], 0);
        out.dat8[i+2] = std::max(img1.dat8[i+2] - img2.dat8[i+2], 0);
    }
    return out;
}

Image& Arthops::asub(Image& out, const Image& img1, const Image& img2) {
    assert(img1.width == img2.width);
    assert(img1.height == img2.height);
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        out.dat8[i  ] = std::abs(img1.dat8[i  ] - img2.dat8[i  ]);
        out.dat8[i+1] = std::abs(img1.dat8[i+1] - img2.dat8[i+1]);
        out.dat8[i+2] = std::abs(img1.dat8[i+2] - img2.dat8[i+2]);
    }
    return out;
}

Image& Arthops::sub(Image& out, const Image& img1, uchar c) {
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        out.dat8[i  ] = std::max(img1.dat8[i  ] - c, 0);
        out.dat8[i+1] = std::max(img1.dat8[i+1] - c, 0);
        out.dat8[i+2] = std::max(img1.dat8[i+2] - c, 0);
    }
    return out;
}

Image& Arthops::asub(Image& out, const Image& img1, uchar c) {
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        out.dat8[i  ] = std::abs(img1.dat8[i  ] - c);
        out.dat8[i+1] = std::abs(img1.dat8[i+1] - c);
        out.dat8[i+2] = std::abs(img1.dat8[i+2] - c);
    }
    return out;
}

Image& Arthops::mul(Image& out, const Image& img1, const Image& img2) {
    assert(img1.width == img2.width);
    assert(img1.height == img2.height);
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        out.dat8[i  ] = std::min(img1.dat8[i  ] * img2.dat8[i  ], 255);
        out.dat8[i+1] = std::min(img1.dat8[i+1] * img2.dat8[i+1], 255);
        out.dat8[i+2] = std::min(img1.dat8[i+2] * img2.dat8[i+2], 255);
    }
    return out;
}

Image& Arthops::mul(Image& out, const Image& img1, float c) {
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        out.dat8[i  ] = std::min(img1.dat8[i  ] * c, 255.0f);
        out.dat8[i+1] = std::min(img1.dat8[i+1] * c, 255.0f);
        out.dat8[i+2] = std::min(img1.dat8[i+2] * c, 255.0f);
    }
    return out;
}

Image& Arthops::div(Image& out, const Image& img1, const Image& img2) {
    assert(img1.width == img2.width);
    assert(img1.height == img2.height);
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        out.dat8[i  ] = (img2.dat8[i  ] > 0 ? std::min(img1.dat8[i  ] / img2.dat8[i  ], 255) : 255);
        out.dat8[i+1] = (img2.dat8[i+1] > 0 ? std::min(img1.dat8[i+1] / img2.dat8[i+1], 255) : 255);
        out.dat8[i+2] = (img2.dat8[i+2] > 0 ? std::min(img1.dat8[i+2] / img2.dat8[i+2], 255) : 255);
    }
    return out;
}

Image& Arthops::blend(Image& out, const Image& img1, const Image& img2, float c) {
    assert(img1.width == img2.width);
    assert(img1.height == img2.height);
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        out.dat8[i  ] = std::min((1.0f-c)*img1.dat8[i  ] + c*img2.dat8[i  ], 255.0f);
        out.dat8[i+1] = std::min((1.0f-c)*img1.dat8[i+1] + c*img2.dat8[i+1], 255.0f);
        out.dat8[i+2] = std::min((1.0f-c)*img1.dat8[i+2] + c*img2.dat8[i+2], 255.0f);
    }
    return out;
}

Image& Arthops::blend(Image& out, const Image& img1, const Image& img2) {
    assert(img1.width == img2.width);
    assert(img1.height == img2.height);
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        float a = img2.dat8[i+3]/255.0f;
        out.dat8[i  ] = std::min((1.0f-a)*img1.dat8[i  ] + a*img2.dat8[i  ], 255.0f);
        out.dat8[i+1] = std::min((1.0f-a)*img1.dat8[i+1] + a*img2.dat8[i+1], 255.0f);
        out.dat8[i+2] = std::min((1.0f-a)*img1.dat8[i+2] + a*img2.dat8[i+2], 255.0f);
    }
    return out;
}

Image& Arthops::logic_not(Image& out, const Image& img1) {
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        out.dat8[i  ] = 255 - img1.dat8[i  ];
        out.dat8[i+1] = 255 - img1.dat8[i+1];
        out.dat8[i+2] = 255 - img1.dat8[i+2];
        out.dat8[i+3] = img1.dat8[i+3];
    }
    return out;
}

Image& Arthops::logic_and(Image& out, const Image& img1, const Image& img2) {
    assert(img1.width == img2.width);
    assert(img1.height == img2.height);
    for (int i = 0; i < img1.size(); ++i) {
        out.dat32[i] = img1.dat32[i] & img2.dat32[i];
    }
    return out;
}

Image& Arthops::logic_or(Image& out, const Image& img1, const Image& img2) {
    assert(img1.width == img2.width);
    assert(img1.height == img2.height);
    for (int i = 0; i < img1.size(); ++i) {
        out.dat32[i] = img1.dat32[i] | img2.dat32[i];
    }
    return out;
}

Image& Arthops::logic_xor(Image& out, const Image& img1, const Image& img2) {
    assert(img1.width == img2.width);
    assert(img1.height == img2.height);
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        out.dat8[i  ] = img1.dat8[i  ] ^ img2.dat8[i  ];
        out.dat8[i+1] = img1.dat8[i+1] ^ img2.dat8[i+1];
        out.dat8[i+2] = img1.dat8[i+2] ^ img2.dat8[i+2];
    }
    return out;
}

Image& Arthops::logarithm(Image& out, const Image& img1) {
    float c = 255.0f/log(1.0f + 255.0f);
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        uchar tmp = c*log(img1.dat8[i]);
        out.dat8[i  ] = tmp;
        out.dat8[i+1] = tmp;
        out.dat8[i+2] = tmp;
        out.dat8[i+3] = img1.dat8[i+3];
    }
    return out;
}

Image& Arthops::exponential(Image& out, const Image& img1, float b, float c) {
    for (int i = 0; i < img1.memsize(); i+=img1.bytespp) {
        uchar tmp = std::min(c*(fpown(b, img1.dat8[i]) - 1.0f), 255.0f);
        out.dat8[i  ] = tmp;
        out.dat8[i+1] = tmp;
        out.dat8[i+2] = tmp;
        out.dat8[i+3] = img1.dat8[i+3];
    }
    return out;
}

} // namespace bacchus
