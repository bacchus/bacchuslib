#pragma once

#include "image/image.h"

namespace bacchus {

void read_png(const char* file_name, Image& dst);
void read_png_mirrored(const char* file_name, Image& dst);

void write_png(const char* file_name, const Image& src);
void write_png_mirrored(const char* file_name, const Image& src);

} // namespace bacchus
