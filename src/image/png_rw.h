#pragma once

#include "image/image.h"

namespace bacchus {

int read_png(const char* file_name, Image& dst);
int read_png_mirrored(const char* file_name, Image& dst);

int write_png(const char* file_name, const Image& src);
int write_png_mirrored(const char* file_name, const Image& src);

} // namespace bacchus
