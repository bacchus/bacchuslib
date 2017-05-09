#pragma once

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

void child_handler(int) {
    int status;
    wait(&status);
}

int nozombie() {
    struct sigaction sa;
    sa.sa_handler = child_handler;
    sigaction(SIGCHLD, &sa, 0);

    for (int i = 0; i < 10; i++) {
        int pid = fork();
        if (pid == 0) {
            printf("I will leave no zombie\n");
            exit(0);
        } else {
            printf("Created a process with the PID %i\n", pid);
        }
    }
    while (1) sleep(1);
    return EXIT_SUCCESS;
}
