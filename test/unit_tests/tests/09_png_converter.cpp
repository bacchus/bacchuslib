#include "setting.h"

#include "image/histogram.h"
#include "image/image.h"
#include "image/png_rw.h"

TEST(PngConvert, Threshold) {
//    using namespace bacchus;
//    print_msg("---------- png converter");
//    char name[32], outname[32];
//    sprintf(name, "rgba/d%i.png", 1);
//    Image img;
//    read_png(name, img);
//    Image out = img;

//    int i = 1;
//    Image img1;
//    sprintf(name, "c%i.png", i);
//    sprintf(outname, "f%i.png", i);
//    read_png(name, img1);
//    Histogram::threshold_global(out, img1, 0);
//    //Histogram::threshold_constant(out, img1, 150);
//    write_png(outname, out);

//        for (int i = 1; i < 7; ++i) {
//            Image img1;
//            sprintf(name, "rgba/d%i.png", i);
//            sprintf(outname, "rgba/e%i.png", i);
//            read_png(name, img1);
//            Histogram::threshold_constant(out, img1, 150);
//            write_png(outname, out);
//        }
}

TEST(FontConvert, ToInt) {
    //uint a = 0b0000000
    //00000
    //00100
    //01110
    //00100
    //00000

//    uint a = 0b00000000000000100011100010000000;

//    std::cout << std::setbase(16);
//    std::cout << "0x" << a << std::endl;

//    for (int i = 0; i < 5; ++i) {
//        for (int j = 0; j < 5; ++j) {
//            if (a%2)    std::cout<<"o";
//            else        std::cout<<" ";
//            a>>=1;
//        }
//        std::cout<<std::endl;
//    }
//    std::cout<<std::endl;
}
