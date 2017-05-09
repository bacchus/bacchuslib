#pragma once

#include <stdio.h>

void print_env() {
    FILE* f = fopen("/proc/self/environ", "r");
    const int BUF_SIZE = 0x100;
    char buff[BUF_SIZE];
    int len;
    while ((len = fread(buff, 1, BUF_SIZE-1, f)) > 0 ) {
        for (int i = 0; i < len; ++i) {
            if (buff[i]==0)
                buff[i] = 10;
        }

        buff[len] = 0;
        printf("%s", buff);
    }
    fclose(f);
}
