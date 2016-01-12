#include "png_rw.h"

#include <stdexcept>
#include <cstdio>
#include <string>
#include <memory>
#include <vector>

#include <png.h>

namespace bacchus {

class png_read_struct {
public:
    png_read_struct()
        : png_ptr(0), info_ptr(0), end_info(0)
    {
        if(
                !(png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0)) ||
                !(info_ptr = png_create_info_struct(png_ptr)) ||
                !(end_info = png_create_info_struct(png_ptr))
                )
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            throw std::bad_alloc();
        }

    }

    void read(const char* file_name, Image& img, bool mirrored = false) {
        FILE *fp = fopen(file_name, "rb");
        if (!fp) {
            throw std::runtime_error(std::string("Cannot open file: ")+std::string(file_name));
        }
        try {
            const size_t HEADER_SIZE = 8;
            png_byte header[HEADER_SIZE];

            if(!fread(header, HEADER_SIZE, 1, fp) || png_sig_cmp(header, 0, HEADER_SIZE)) {
                throw std::runtime_error("Not PNG file");
            }

            if (setjmp(png_jmpbuf(png_ptr))) {
                throw std::runtime_error("Error while reading PNG file");
            }

            png_init_io(png_ptr, fp);
            png_set_sig_bytes(png_ptr, HEADER_SIZE);

            png_read_info(png_ptr, info_ptr);
            png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
            png_uint_32 height = png_get_image_height(png_ptr, info_ptr);

            png_byte bytespp = 4;//info_ptr->pixel_depth;

            //png_byte* img_buf = new png_byte[width*height*4];
            png_byte* img_buf = new png_byte[width*height*bytespp];
            png_bytep* row_pointers = new png_bytep[height];

            if(mirrored)
                for(size_t i = 0; i<height; ++i)
                    row_pointers[i] = img_buf + (height-1-i)*width*bytespp;
            else
                for(size_t i = 0; i<height; ++i)
                    row_pointers[i] = img_buf + i*width*bytespp;

            //png_set_transform
            png_set_packing(png_ptr);
            png_set_expand(png_ptr);
            png_set_gray_to_rgb(png_ptr);

            png_read_update_info(png_ptr, info_ptr);
            png_read_image(png_ptr, row_pointers);

            png_read_end(png_ptr, info_ptr);

            img.dat8 = img_buf;
            img.width = width;
            img.height = height;
            //img.bytespp = Image::FMT_RGBA;
            img.bytespp = bytespp;

            delete[] row_pointers;

            fclose(fp);
        } catch(...) {
            fclose(fp);
            throw;
        }
    }

    ~png_read_struct() {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    }

private:
    png_structp png_ptr;
    png_infop info_ptr;
    png_infop end_info;
};

void read_png(const char* file_name, Image& dst) {
    png_read_struct png;
    png.read(file_name, dst);
}

void read_png_mirrored(const char* file_name, Image& dst) {
    png_read_struct png;
    png.read(file_name, dst, true);
}

static void makepng_warning(png_structp png_ptr, png_const_charp message) {
    const char **ep = (const char **)png_get_error_ptr(png_ptr);
    const char *name;

    if (ep != NULL && *ep != NULL)
        name = *ep;

    else
        name = "makepng";

    printf("%s: warning: %s\n", name, message);
}

static void makepng_error(png_structp png_ptr, png_const_charp message) {
    const char **ep = (const char **)png_get_error_ptr(png_ptr);
    const char *name;

    if (ep != NULL && *ep != NULL)
        name = *ep;

    else
        name = "makepng";

    char s[2048];
    sprintf(s, "%s: error: %s\n", name, message);
    throw std::runtime_error(s);
}

class png_write_struct {
public:
    png_write_struct()
        : png_ptr(0), info_ptr(0)
    {
        if(
                !(png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, makepng_error, makepng_warning)) ||
                !(info_ptr = png_create_info_struct(png_ptr))
                )
        {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            throw std::bad_alloc();
        }

    }

    void write(const char* file_name, const Image& img, bool mirrored = false) {
        FILE *fp = fopen(file_name, "wb");
        if (!fp) {
            throw std::runtime_error(std::string("Cannot open file: ")+std::string(file_name));
        }

        try {
            if (setjmp(png_jmpbuf(png_ptr))) {
                throw std::runtime_error(std::string("Error while writing PNG file ")+std::string(file_name));
            }

            png_uint_32 width = img.width;
            png_uint_32 height = img.height;
            uint bytespp = 4;//img.bytespp;
            int depth = 8;

            if (width == 0 || height == 0)
                throw std::runtime_error("width == 0 || height == 0");

            png_set_IHDR(png_ptr,
                         info_ptr,
                         width,
                         height,
                         depth,
                         PNG_COLOR_TYPE_RGBA,
                         PNG_INTERLACE_NONE,
                         PNG_COMPRESSION_TYPE_DEFAULT,
                         PNG_FILTER_TYPE_DEFAULT);

            png_bytep* row_pointers = new png_bytep[height];

            png_bytep img_ptr = img.dat8;
            if(mirrored) {
                for(size_t i = 0; i<height; ++i) {
                    row_pointers[i] = img_ptr + (height-1-i)*width*bytespp;
                }
            } else {
                for(size_t i = 0; i<height; ++i)
                    row_pointers[i] = img_ptr + i*width*bytespp;
            }

            if (setjmp(png_jmpbuf(png_ptr))) {
                throw std::runtime_error(std::string("Error png_init_io")+std::string(file_name));
            }

            png_init_io(png_ptr, fp);

            if (setjmp(png_jmpbuf(png_ptr))) {
                throw std::runtime_error(std::string("Error png_set_rows")+std::string(file_name));
            }

            png_set_rows(png_ptr, info_ptr, row_pointers);

            if (setjmp(png_jmpbuf(png_ptr))) {
                throw std::runtime_error(std::string("Error png_write_png")+std::string(file_name));
            }

            png_write_png(png_ptr, info_ptr, 0, 0);

            png_write_end(png_ptr, info_ptr);

            delete[] row_pointers;

            fclose(fp);
        } catch(...) {
            fclose(fp);
            throw;
        }
    }

    ~png_write_struct() {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }

private:
    png_structp png_ptr;
    png_infop info_ptr;
};

void write_png(const char* file_name, const Image& src) {
    png_write_struct png;
    png.write(file_name, src);
}

void write_png_mirrored(const char* file_name, const Image& src) {
    png_write_struct png;
    png.write(file_name, src, true);
}

} // namespace bacchus
