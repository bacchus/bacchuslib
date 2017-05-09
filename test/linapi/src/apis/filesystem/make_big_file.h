#pragma once

#include <stdio.h>
#include <string.h>

#include <fstream>

// 0x1000000        16777216

// file_size        16777232
// ls -al big_file  16777232    16.8MB
// du -b big_file   16777232

// du -h big_file       8.0K
// du -b -h big_file    17M

inline long file_size(const std::string& fname) {
    std::ifstream f(fname);
    if (f.is_open()) {
        long beg = f.tellg();
        f.seekg(0, std::ios::end);
        long end = f.tellg();
        f.close();
        return end-beg;
    } else std::cout << "unable to open: " << fname << std::endl;
    return 0;
}

inline int make_big_file(const char* fname, const int size = 0x1000000) {
    FILE* f = fopen(fname, "w");
    if (f == NULL) {
        printf("create file error: %s", fname);
        return 1;
    }
    fwrite(fname, 1, strlen(fname), f);
    fseek(f, size, SEEK_CUR);
    fwrite(fname, 1, strlen(fname), f);
    fclose(f);
    return 0;
}
