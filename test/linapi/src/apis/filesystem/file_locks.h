#pragma once

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// run two programs parallel in bash
// ./linapi & ./linapi && fg
//This will:
//  - Start prog1.
//  - Send it to background, but keep printing its output.
//  - Start prog2, and keep it in foreground, so you can close it with ctrl-c.
//  - When you close prog2, you'll return to prog1's foreground, so you can also close it with ctrl-c.

inline void test_locks() {

    char str[64];
    memset(str, 32, 64);
    sprintf(str, "writed by %i\n", getpid());

    int fd = open("testlocks.txt", O_RDWR|O_CREAT);

    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 64;

    int offset = 0;
    while (fcntl(fd, F_SETLK, &fl) == -1) {
        fcntl(fd, F_GETLK, &fl);
        printf("bytes %i - %i locked by %i\n", offset, offset+64, fl.l_pid);
        offset += 64;
        fl.l_start = offset;
    }

    lseek(fd, offset, SEEK_SET);
    write(fd, str, strlen(str));
    //getchar();

    fl.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &fl) == -1) {
        printf("unlock error\n");
    }
    close(fd);
}
