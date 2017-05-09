#pragma once

#include <stdio.h>
#include <dirent.h>

int selector(const dirent*) {
    return 1; // always confirm
}

const char* get_type_str(int t) {
    switch (t) {
    case 1 : return "p"; // pipe
    case 2 : return "c"; // char dev
    case 4 : return "d"; // dir
    case 6 : return "b"; // block dev
    case 8 : return "f"; // file
    case 10: return "l"; // link
    case 12: return "s"; // socket
    default:return "xz";
    }
}

int scan_dir(const char* dir_name) {
    dirent** entry;
    int n = scandir(dir_name, &entry, selector, alphasort);
    if (n < 0) {
        printf("scandir error: %s\n", dir_name);
        return 1;
    }
    printf("%40s\t%s\t%s\n", "filename", "type", "inode");
    for (int i = 0; i < n; i++) {
        printf("%40s\t%s\t%lu\n", entry[i]->d_name, get_type_str(entry[i]->d_type), entry[i]->d_ino);
    }
    return 0;
}

int open_dir(const char* dir_name) {
    DIR* d = opendir(dir_name);
    if (d == NULL) {
        printf("opendir error: %s\n", dir_name);
        return 1;
    }
    printf("%40s\t%s\t%s\n", "filename", "type", "inode");
    while (dirent* entry = readdir(d)) {
        printf("%40s\t%s\t%lu\n", entry->d_name, get_type_str(entry->d_type), entry->d_ino);
    }
    closedir(d);
    return 0;
}
