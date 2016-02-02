#include <stdio.h>

int bmp_read(const char* file_name, Image& img) {
    if (!file_name)
        throw std::runtime_error("Error: file_name = nullptr");

    FILE* file = fopen(file_name, "rb");
    if (!file)
        throw std::runtime_error(std::string("Cannot open file: ") + std::string(file_name));

    ushort wbuf = 0;
    ulong dwbuf = 0;
    uchar bbuf = 0;

    // tagBITMAPFILEHEADER
    fread(&wbuf, sizeof(ushort), 1, file);// word  bfType;

    //BM
    if (wbuf != 0x4D42)
        throw std::runtime_error(std::string("Error not BMP file: ") + std::string(file_name));

    fread(&dwbuf, sizeof(ulong), 1, file);//dword bfSize;

    fread(&wbuf, sizeof(ushort), 1, file);//word  bfReserved1;
    fread(&wbuf, sizeof(ushort), 1, file);//word  bfReserved2;

    fread(&dwbuf, sizeof(ulong), 1, file);//dword bfOffbits;

    // tagBITMAPINFO
    //      BITMAPINFOHEADER     bmiHeader;
    //      tagBITMAPINFOHEADER
    fread(&dwbuf, sizeof(ulong), 1, file);// dword   biSize;

    fread(&dwbuf, sizeof(ulong), 1, file);//dword   biWidth;
    ulong w = dwbuf;
    img.width = w;

    fread(&dwbuf, sizeof(ulong), 1, file);// dword   biHeight;
    ulong h = dwbuf;
    img.height = h;

    fread(&wbuf, sizeof(ushort), 1, file);// word    biPlanes;
    if (wbuf != 1)
        throw std::runtime_error(std::string("Error [biPlanes != 1] file: ") + std::string(file_name));

    fread(&wbuf, sizeof(ushort), 1, file);// word    biBitCount;
    ushort nb = wbuf;

    fread(&dwbuf, sizeof(ulong), 1, file);// dword   biCompression;
    //switch (dwbuf) {
    //    case BI_RLE4:
    //        //compressed 4b
    //        break;
    //    case BI_RLE8:
    //        //compressed 8b
    //        break;
    //    case BI_RGB:
    //        break;
    //    default: ; 
    //}

    fread(&dwbuf, sizeof(ulong), 1, file);// dword   biSizeImage;
    int sz = dwbuf;

    fread(&dwbuf, sizeof(ulong), 1, file);// dword   biXPelsPerMeter;
    fread(&dwbuf, sizeof(ulong), 1, file);// dword   biYPelPerMeter;

    fread(&dwbuf, sizeof(ulong), 1, file);// dword   biClrUsed;
    fread(&dwbuf, sizeof(ulong), 1, file);// dword   biClrImportant;

    // tagRGBQUAD
    fread(&bbuf, sizeof(uchar), 1, file);// byte    rgbRed;
    fread(&bbuf, sizeof(uchar), 1, file);// byte    rgbGreen;
    fread(&bbuf, sizeof(uchar), 1, file);// byte    rgbBlue;
    fread(&bbuf, sizeof(uchar), 1, file);// byte    rgbRserved;

    img.dat8 = new uchar[sz];

    int np = 0;
    uchar tmp;
    int n = 0;
    while (fread(&tmp, sizeof(uchar), 1, file) && np<sz) {
        *img.dat8++ = tmp;
        np++;
        n++;
        if ((n%(w*3) == 0) && (w*nb)%32 != 0) {
            int add = (32 - (w*nb)%32)/8;
            for (int i=0; i<add; i++)
                fread(&img.dat8, sizeof(uchar), 1, file);
            n = 0;
        }
    }

    fclose(file);
}
