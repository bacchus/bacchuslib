#pragma once

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int pipe_create_writer_and_log(const char* argv) {
    FILE* f = popen(argv, "r");
    if (f == NULL) {
        perror("error:\n");
        return -1;
    }
    const int BUF_SIZE = 0x100;
    char buf[BUF_SIZE];
    FILE* o = fopen("log.txt", "w");
    while (int len = fread(buf, 1, BUF_SIZE, f)) {
        write(1, buf, len);
        fwrite(buf, 1, len, o);
    }
    pclose(f);
    fclose(o);
    return 0;
}

int pipe_anonim() {
    int pipedes[2]; // pipe read, write descriptors
    pipe(pipedes); // create pipe

    const int pipe_buffsize = fpathconf(pipedes[0], _PC_PIPE_BUF);// get pipe buff size (4KB up to 16 bufeers)
    printf("pipe buffsize: %i B\n", pipe_buffsize);

    pid_t pid = fork(); // forks

    if (pid > 0) { // we are in parent proc; pid of child
        close(pipedes[0]); // close for read
        const char* str = "String passed via pipe\n";
        write(pipedes[1], str, strlen(str) + 1); // write to pipe
        close(pipedes[1]); // we are done, close write
        wait(NULL); // wait for child
    } else {  // we are in child proc
        close(pipedes[1]); // close for write
        char buf[1024];
        while (int len = read(pipedes[0], buf, pipe_buffsize)) // read from pipe
            write(2, buf, len-1); // and to cerr - (2)
        close(pipedes[0]); // we are done, close write
    }
    return 0;
}


int pipe_create_writer_noenv(const char* argv) {
    int pipedes[2];
    pipe(pipedes);
    pid_t pid = fork();
    if (pid > 0) {
        close(pipedes[1]);
        const int BUF_SIZE = 0x100;
        char buf[BUF_SIZE];
        while (int len = read(pipedes[0], buf, BUF_SIZE))
            write(1, buf, len);
        close(pipedes[0]);
    } else {
        close(pipedes[0]);
        dup2(pipedes[1], 1);
        execve(argv, NULL, NULL);
    }
    return 0;
}

#define FIFO_NAME "./fifofile"

int typeserver() {
    char ch;
    mkfifo(FIFO_NAME, 0600);
    //mknod(FIFO_NAME, S_IFIFO, 0);// same
    //mknod fifofile p - in terminal
    FILE* f = fopen(FIFO_NAME, "w");
    if (f == NULL) {
        printf("couldnt open fifo\n");
        return -1;
    }

    do {
        ch = getchar();
        fputc(ch, f);
        if (ch == 10) fflush(f);
    } while (ch != 'q');

    fclose(f);
    unlink(FIFO_NAME);
    return 0;
}

int typeclient() {
    char ch;
    FILE* f = fopen(FIFO_NAME, "r");
    do {
        ch = fgetc(f);
        putchar(ch);
    } while (ch != 'q');

    fclose(f);
    unlink(FIFO_NAME);
    return 0;
}
