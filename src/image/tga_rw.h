#pragma once

#include "image/image.h"

namespace bacchus {

void read_tga(const char* file_name, Image& dst);
void read_tga_mirrored(const char* file_name, Image& dst);

void write_tga(const char* file_name, const Image& src);
void write_tga_mirrored(const char* file_name, const Image& src);

} // namespace bacchus
