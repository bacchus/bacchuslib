#include "png_rw.h"

#include <cstdio>
#include <string>
#include <memory>
#include <vector>

#include <png.h>
#include "utils/logger.h"

namespace bacchus {

class png_read_struct {
public:
    png_read_struct()
        : png_ptr(nullptr), info_ptr(nullptr), end_info(nullptr) {
    }

    int init() {
        if (
            !(png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr)) ||
            !(info_ptr = png_create_info_struct(png_ptr)) ||
            !(end_info = png_create_info_struct(png_ptr))
        ) {
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            LOGE("Error bad alloc");
            return -1;
        }

        return 0;
    }

    int read(const char* file_name, unsigned char*& img_dat8, unsigned int& img_width, unsigned int& img_height, uchar& img_bpp, bool mirrored = false) {
        FILE* fp = fopen(file_name, "rb");

        if (!fp) {
            LOGE("Error cannot open file: %s", file_name);
            return -1;
        }

        const size_t HEADER_SIZE = 8;
        png_byte header[HEADER_SIZE];

        if (!fread(header, HEADER_SIZE, 1, fp) || png_sig_cmp(header, 0, HEADER_SIZE)) {
            LOGE("Not PNG file");
            return -1;
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            LOGE("Error while reading PNG file");
            return -1;
        }

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, HEADER_SIZE);
        png_read_info(png_ptr, info_ptr);
        png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
        png_byte bytespp = 4;//info_ptr->pixel_depth;
        //png_byte* img_buf = new png_byte[width*height*4];
        png_byte* img_buf = new png_byte[width * height * bytespp];
        png_bytep* row_pointers = new png_bytep[height];

        if (mirrored)
            for (size_t i = 0; i < height; ++i)
                row_pointers[i] = img_buf + (height - 1 - i) * width * bytespp;
        else
            for (size_t i = 0; i < height; ++i)
                row_pointers[i] = img_buf + i * width * bytespp;

        //png_set_transform
        png_set_packing(png_ptr);
        png_set_expand(png_ptr);
        png_set_gray_to_rgb(png_ptr);
        png_read_update_info(png_ptr, info_ptr);
        png_read_image(png_ptr, row_pointers);
        png_read_end(png_ptr, info_ptr);
        img_dat8 = img_buf;
        img_width = width;
        img_height = height;
        img_bpp = bytespp;
        delete[] row_pointers;
        fclose(fp);

        return 0;
    }

    ~png_read_struct() {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    }

private:
    png_structp png_ptr;
    png_infop info_ptr;
    png_infop end_info;
};

int read_png(const char* file_name, Image& img) {
    png_read_struct png;
    if (png.init() != 0)
        return -1;

    return png.read(file_name, img.dat8, img.width, img.height, img.bytespp);
}

int read_png_mirrored(const char* file_name, Image& img) {
    png_read_struct png;
    if (png.init() != 0)
        return -1;

    return png.read(file_name, img.dat8, img.width, img.height, img.bytespp, true);
}

///-----------------------------------------------------------------------------
static void makepng_warning(png_structp png_ptr, png_const_charp message) {
    const char** ep = (const char**)png_get_error_ptr(png_ptr);
    const char* name;

    if (ep != nullptr && *ep != nullptr)
        name = *ep;
    else
        name = "makepng";

    LOGW("%s: warning: %s\n", name, message);
}

static void makepng_error(png_structp png_ptr, png_const_charp message) {
    const char** ep = (const char**)png_get_error_ptr(png_ptr);
    const char* name;

    if (ep != nullptr && *ep != nullptr)
        name = *ep;
    else
        name = "makepng";

    LOGE("%s: error: %s\n", name, message);
}

class png_write_struct {
public:
    png_write_struct()
        : png_ptr(nullptr), info_ptr(nullptr) {
    }

    int init() {
        if (
            !(png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, makepng_error,
                                                makepng_warning)) ||
            !(info_ptr = png_create_info_struct(png_ptr))
        ) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            LOGE("Error bad alloc");
            return -1;
        }
        return 0;
    }

    int write(const char* file_name, unsigned char* img_dat8, unsigned int img_width, unsigned int img_height, uchar img_bpp = 4, bool mirrored = false) {
        FILE* fp = fopen(file_name, "wb");

        if (!fp) {
            LOGE("Error Cannot open file: %s", file_name);
            return -1;
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            LOGE("Error while writing PNG file: %s", file_name);
            return -1;
        }

        png_uint_32 width = img_width;
        png_uint_32 height = img_height;
        uint bytespp = img_bpp;// BCC: 4
        int depth = 8;

        if (width == 0 || height == 0) {
            LOGE("Error width == 0 || height == 0: %s", file_name);
            return -1;
        }

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
        png_bytep img_ptr = img_dat8;

        if (mirrored) {
            for (size_t i = 0; i < height; ++i) {
                row_pointers[i] = img_ptr + (height - 1 - i) * width * bytespp;
            }
        } else {
            for (size_t i = 0; i < height; ++i)
                row_pointers[i] = img_ptr + i * width * bytespp;
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            LOGE("Error png_init_io: %s", file_name);
            return -1;
        }

        png_init_io(png_ptr, fp);

        if (setjmp(png_jmpbuf(png_ptr))) {
            LOGE("Error png_set_rows: %s", file_name);
            return -1;
        }

        png_set_rows(png_ptr, info_ptr, row_pointers);

        if (setjmp(png_jmpbuf(png_ptr))) {
            LOGE("Error png_write_png: %s", file_name);
            return -1;
        }

        png_write_png(png_ptr, info_ptr, 0, nullptr);
        png_write_end(png_ptr, info_ptr);
        delete[] row_pointers;
        fclose(fp);
        return 0;
    }

    ~png_write_struct() {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }

private:
    png_structp png_ptr;
    png_infop info_ptr;
};

int write_png(const char* file_name, const Image& img) {
    png_write_struct png;
    if (png.init() != 0)
        return -1;

    return png.write(file_name, img.dat8, img.width, img.height, img.bytespp);
}

int write_png_mirrored(const char* file_name, const Image& img) {
    png_write_struct png;
    if (png.init() != 0)
        return -1;

    return png.write(file_name, img.dat8, img.width, img.height, img.bytespp, true);
}

} // namespace bacchus
