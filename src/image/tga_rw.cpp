#include "tga_rw.h"

#include <stdexcept>
#include <cstdio>
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>

namespace bacchus {

#pragma pack(push,1)
struct tga_header {
    char idlength;
    char colormaptype;
    char datatypecode;
    short colormaporigin;
    short colormaplength;
    char colormapdepth;
    short x_origin;
    short y_origin;
    short width;
    short height;
    char  bitsperpixel;
    char  imagedescriptor;
};
#pragma pack(pop)

// alternate
//struct tga_header
//{
//    unsigned char idLength;
//    unsigned char colorMapType;
//    unsigned char imageTypeCode;
//    unsigned char colorMapSpec[5];
//    unsigned short xOrigin;
//    unsigned short yOrigin;
//    unsigned short width;
//    unsigned short height;
//    unsigned char bpp;
//    unsigned char imageDesc;
//};

class tga_read_struct {
public:
    tga_read_struct(Image &img): m_img(img) {}
    bool read_tga_file(const char *file_name);
    bool flip_horizontally();
    bool flip_vertically();
    void clear();
protected:
    bool   load_rle_data(std::ifstream &in);

    //TODO: not tested
    //bool simple_read_tga_file(const char *file_name);
private:
    Image& m_img;
};

//TODO: not tested
// but uses bitsperpixel
//bool tga_read_struct::simple_read_tga_file(const char *file_name) {
//    std::fstream fs;
//    fs.open(file_name.c_str(), std::ios::in|std::ios::binary);
//    if (fs.is_open()) {
//        tga_header header;
//        fs.read((char*)&header, sizeof(tga_header));
//        int size = header.width*header.height*header.bitsperpixel;
//        if (m_img.dat8)
//            delete[] m_img.dat8;
//        m_img.dat8 = new unsigned char[size];
//        fs.read(reinterpret_cast<char*>(m_img.dat8), size);

//        if (header.bitsperpixel==24) {
//            // glPixelStorei(GL_UNPACK_ALIGNMENT, 1); GL_RGB
//            for (int i=0; i<size; i+=3) {
//                unsigned char c = m_img.dat8[i];
//                m_img.dat8[i] = m_img.dat8[i+2];
//                m_img.dat8[i+2] = c;
//            }
//        } else if (header.bitsperpixel==32) {
//            // glPixelStorei(GL_UNPACK_ALIGNMENT, 2); GL_RGBA
//            for (int i=0; i<size; i+=4) {
//                unsigned char c = m_img.dat8[i];
//                m_img.dat8[i] = m_img.dat8[i+2];
//                m_img.dat8[i+2] = c;
//            }
//        }

//        fs.close();

//    } else {
//        std::cerr << "can't open file " << file_name << "\n";
//        return false;
//    }
//}

class tga_write_struct {
public:
    tga_write_struct(const Image &img): m_img(img) {}
    bool write_tga_file(const char *file_name, bool rle=true);
protected:
    bool unload_rle_data(std::ofstream &out);
private:
    const Image& m_img;
};

bool tga_read_struct::read_tga_file(const char *file_name) {
    if (m_img.dat8)
        delete[] m_img.dat8;
    m_img.dat8 = nullptr;
    std::ifstream in;
    in.open (file_name, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "can't open file " << file_name << "\n";
        in.close();
        return false;
    }
    tga_header header;
    in.read((char*)&header, sizeof(header));
    if (!in.good()) {
        in.close();
        std::cerr << "an error occured while reading the header\n";
        return false;
    }
    m_img.width   = header.width;
    m_img.height  = header.height;
    m_img.bytespp = Image::Format(header.bitsperpixel>>3);
    //std::cout << "m_img.bytespp: " << (int)m_img.bytespp << std::endl;
    if (m_img.width<=0 || m_img.height<=0 || (m_img.bytespp!=Image::FMT_LUM && m_img.bytespp!=Image::FMT_RGB && m_img.bytespp!=Image::FMT_RGBA)) {
        in.close();
        std::cerr << "bad bpp (or width/height) value\n";
        return false;
    }

    // read in bgr format
    if (m_img.bytespp==Image::FMT_RGB) m_img.format = Image::FMT_BGR;
    if (m_img.bytespp==Image::FMT_RGBA) m_img.format = Image::FMT_BGRA;

    unsigned long nbytes = m_img.bytespp*m_img.width*m_img.height;
    m_img.dat8 = new unsigned char[nbytes];
    if (3==header.datatypecode || 2==header.datatypecode) {
        in.read((char *)m_img.dat8, nbytes);
        if (!in.good()) {
            in.close();
            std::cerr << "an error occured while reading the data\n";
            return false;
        }
    } else if (10==header.datatypecode||11==header.datatypecode) {
        if (!load_rle_data(in)) {
            in.close();
            std::cerr << "an error occured while reading the data\n";
            return false;
        }
    } else {
        in.close();
        std::cerr << "unknown file format " << (int)header.datatypecode << "\n";
        return false;
    }
    if (!(header.imagedescriptor & 0x20)) {
        flip_vertically();
    }
    if (header.imagedescriptor & 0x10) {
        flip_horizontally();
    }
    //std::cerr << m_img.width << "x" << m_img.height << "/" << m_img.bytespp*8 << "\n";
    in.close();
    return true;
}

bool tga_read_struct::load_rle_data(std::ifstream &in) {
    unsigned long pixelcount = m_img.width*m_img.height;
    unsigned long currentpixel = 0;
    unsigned long currentbyte  = 0;
    uchar colorbuffer[4];
    do {
        unsigned char chunkheader = 0;
        chunkheader = in.get();
        if (!in.good()) {
            std::cerr << "an error occured while reading the data\n";
            return false;
        }
        if (chunkheader<128) {
            chunkheader++;
            for (int i=0; i<chunkheader; i++) {
                in.read((char *)colorbuffer, m_img.bytespp);
                if (!in.good()) {
                    std::cerr << "an error occured while reading the header\n";
                    return false;
                }
                for (int t=0; t<m_img.bytespp; t++)
                    m_img.dat8[currentbyte++] = colorbuffer[t];
                currentpixel++;
                if (currentpixel>pixelcount) {
                    std::cerr << "Too many pixels read\n";
                    return false;
                }
            }
        } else {
            chunkheader -= 127;
            in.read((char *)colorbuffer, m_img.bytespp);
            if (!in.good()) {
                std::cerr << "an error occured while reading the header\n";
                return false;
            }
            for (int i=0; i<chunkheader; i++) {
                for (int t=0; t<m_img.bytespp; t++)
                    m_img.dat8[currentbyte++] = colorbuffer[t];
                currentpixel++;
                if (currentpixel>pixelcount) {
                    std::cerr << "Too many pixels read\n";
                    return false;
                }
            }
        }
    } while (currentpixel < pixelcount);
    return true;
}

bool tga_write_struct::write_tga_file(const char *file_name, bool rle) {
    unsigned char developer_area_ref[4] = {0, 0, 0, 0};
    unsigned char extension_area_ref[4] = {0, 0, 0, 0};
    unsigned char footer[18] = {'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0'};
    std::ofstream out;
    out.open (file_name, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "can't open file " << file_name << "\n";
        out.close();
        return false;
    }
    tga_header header;
    memset((void *)&header, 0, sizeof(header));
    header.bitsperpixel = m_img.bytespp<<3;
    header.width  = m_img.width;
    header.height = m_img.height;
    header.datatypecode = (m_img.bytespp==Image::FMT_LUM?(rle?11:3):(rle?10:2));
    header.imagedescriptor = 0x20; // top-left origin
    out.write((char *)&header, sizeof(header));
    if (!out.good()) {
        out.close();
        std::cerr << "can't dump the tga file\n";
        return false;
    }
    if (!rle) {
        out.write((char *)m_img.dat8, m_img.width*m_img.height*m_img.bytespp);
        if (!out.good()) {
            std::cerr << "can't unload raw data\n";
            out.close();
            return false;
        }
    } else {
        if (!unload_rle_data(out)) {
            out.close();
            std::cerr << "can't unload rle data\n";
            return false;
        }
    }
    out.write((char *)developer_area_ref, sizeof(developer_area_ref));
    if (!out.good()) {
        std::cerr << "can't dump the tga file\n";
        out.close();
        return false;
    }
    out.write((char *)extension_area_ref, sizeof(extension_area_ref));
    if (!out.good()) {
        std::cerr << "can't dump the tga file\n";
        out.close();
        return false;
    }
    out.write((char *)footer, sizeof(footer));
    if (!out.good()) {
        std::cerr << "can't dump the tga file\n";
        out.close();
        return false;
    }
    out.close();
    return true;
}

// TODO: it is not necessary to break a raw chunk for two equal pixels (for the matter of the resulting size)
bool tga_write_struct::unload_rle_data(std::ofstream &out) {
    const unsigned char max_chunk_length = 128;
    unsigned long npixels = m_img.width*m_img.height;
    unsigned long curpix = 0;
    while (curpix<npixels) {
        unsigned long chunkstart = curpix*m_img.bytespp;
        unsigned long curbyte = curpix*m_img.bytespp;
        unsigned char run_length = 1;
        bool raw = true;
        while (curpix+run_length<npixels && run_length<max_chunk_length) {
            bool succ_eq = true;
            for (int t=0; succ_eq && t<m_img.bytespp; t++) {
                succ_eq = (m_img.dat8[curbyte+t]==m_img.dat8[curbyte+t+m_img.bytespp]);
            }
            curbyte += m_img.bytespp;
            if (1==run_length) {
                raw = !succ_eq;
            }
            if (raw && succ_eq) {
                run_length--;
                break;
            }
            if (!raw && !succ_eq) {
                break;
            }
            run_length++;
        }
        curpix += run_length;
        out.put(raw?run_length-1:run_length+127);
        if (!out.good()) {
            std::cerr << "can't dump the tga file\n";
            return false;
        }
        out.write((char *)(m_img.dat8+chunkstart), (raw?run_length*m_img.bytespp:m_img.bytespp));
        if (!out.good()) {
            std::cerr << "can't dump the tga file\n";
            return false;
        }
    }
    return true;
}


bool tga_read_struct::flip_horizontally() {
    if (!m_img.dat8) return false;
    uint half = m_img.width>>1;
    for (uint i=0; i<half; i++) {
        for (uint j=0; j<m_img.height; j++) {
            uint c1 = m_img.pix(i, j);
            uint c2 = m_img.pix(m_img.width-1-i, j);
            m_img.pix(i, j) = c2;
            m_img.pix(m_img.width-1-i, j) = c1;
        }
    }
    return true;
}

bool tga_read_struct::flip_vertically() {
    if (!m_img.dat8) return false;
    unsigned long bytes_per_line = m_img.width*m_img.bytespp;
    unsigned char *line = new unsigned char[bytes_per_line];
    int half = m_img.height>>1;
    for (int j=0; j<half; j++) {
        unsigned long l1 = j*bytes_per_line;
        unsigned long l2 = (m_img.height-1-j)*bytes_per_line;
        memmove((void *)line,      (void *)(m_img.dat8+l1), bytes_per_line);
        memmove((void *)(m_img.dat8+l1), (void *)(m_img.dat8+l2), bytes_per_line);
        memmove((void *)(m_img.dat8+l2), (void *)line,      bytes_per_line);
    }
    delete [] line;
    return true;
}

void tga_read_struct::clear() {
    memset((void *)m_img.dat8, 0, m_img.width*m_img.height*m_img.bytespp);
}

void read_tga(const char *file_name, Image &dst) {
    tga_read_struct reader(dst);
    reader.read_tga_file(file_name);
}

void read_tga_mirrored(const char *file_name, Image &dst) {
    tga_read_struct reader(dst);
    reader.read_tga_file(file_name);
    reader.flip_vertically();
}

void write_tga(const char *file_name, const Image &src) {
    tga_write_struct writer(src);
    writer.write_tga_file(file_name);
}

void write_tga_mirrored(const char *file_name, const Image &src) {
    //TODO: not mirored
    tga_write_struct writer(src);
    writer.write_tga_file(file_name);
}



} // namespace bacchus
