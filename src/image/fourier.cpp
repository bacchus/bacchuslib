#include "fourier.h"
#include "image.h"

#include "math/fft.h"

namespace bacchus {

std::vector< matxcx > Fourier::transform(Image& mag, Image& arg, float& ymin, float& ymax, const Image& img1) {
    int width = img1.width;
    int height = img1.height;

    std::vector< matxcx > yimg(height, matxcx(width));
    std::vector< matxcx > ximg(width, matxcx(height));

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            ximg[x][y] = img1.lum(x,y);
        }
        ximg[x] = fft(ximg[x]);
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            yimg[y][x] = ximg[x][y];
        }
        yimg[y] = fft(yimg[y]);
    }

    ymax = 0;
    ymin = BCC_M_FLOAT_MAX;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            //float val = yimg[y][x].mag();
            float val = std::abs(yimg[y][x]);
            if (val < ymin) ymin = val;
            if (val > ymax) ymax = val;
        }
    }

//        std::cout<<"fourier_transform: "<<ymin<<" .. "<<ymax<<std::endl;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
//                int i = (y + height/2) % height;
//                int j = (x + width/2) % width;
            //float mag = clamp_color(std::abs(yimg[y][x].mag()));
//            float ymag = 255.0f*(yimg[y][x].mag() - ymin)/(ymax - ymin);
//            float yarg = 255.0f*(yimg[y][x].arg() + M_PI)/(2.0f*M_PI);
            float ymag = 255.0f*(std::abs(yimg[y][x]) - ymin)/(ymax - ymin);
            float yarg = 255.0f*(std::arg(yimg[y][x]) + M_PI)/(2.0f*M_PI);

            mag.pix(x,y) = lum2i(ymag);// xz
            arg.pix(x,y) = lum2i(yarg);// 0..2pi -> 0..255
        }
    }

    return yimg;
}

Image& Fourier::inverse(Image& out, const std::vector< matxcx >& cplximg) {
    int height = cplximg.size();
    int width = cplximg[0].size();

    std::vector< matxcx > yimg(height, matxcx(width));
    std::vector< matxcx > ximg(width, matxcx(height));

    for (int y = 0; y < height; ++y) {
        yimg[y] = fftrev(cplximg[y]);
        for (int x = 0; x < width; ++x) {
            ximg[x][y] = yimg[y][x];
        }
    }

    for (int x = 0; x < width; ++x) {
        ximg[x] = fftrev(ximg[x]);
        for (int y = 0; y < height; ++y) {
            out.pix(x,y) = lum2i(ximg[x][y].real());
        }
    }

    return out;
}

Image& Fourier::inverse(Image& out, const Image& mag, const Image& arg, float ymin, float ymax) {
    int width = mag.width;
    int height = mag.height;

    std::vector< matxcx > yimg2(height, matxcx(width));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float r = ymin + (ymax - ymin)*mag.lum(x,y)/255.0f;
            float th = - M_PI + 2.0f*M_PI*arg.lum(x,y)/255.0f;
            //yimg2[y][x].set_rt(r,th);
            yimg2[y][x] = std::polar(r,th);
        }
    }
    return inverse(out, yimg2);
}

} // namespace bacchus
