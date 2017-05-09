#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void term_handler(int sig) {
    printf("Signal %i - %s\n", sig, sys_siglist[sig]);
    printf("Terminating...\n");
    sleep(10);
    exit(EXIT_SUCCESS);
}

int sigdemo() {
    struct sigaction sa;
    sa.sa_handler = term_handler;
    sigaction(SIGTERM, &sa, 0);

    printf("My pid is %i\n", getpid());
    printf("Waiting...\n");

    sigset_t newset;
//    sigemptyset(&newset);
//    sigaddset(&newset, SIGHUP);
//    sigprocmask(SIG_BLOCK, &newset, 0);

    sigfillset(&newset);
    sigdelset(&newset, SIGTERM);
    sigprocmask(SIG_SETMASK, &newset, 0);

    //while(1) sleep(1);

    int sig;
    while(!sigwait(&newset, &sig)) {
        printf("Signal %i - %s\n", sig, sys_siglist[sig]);
        //printf("SIGHUP recieved\n");
    }

    //raise(sig);
    //kill(getpid(), sig);

    return EXIT_FAILURE;
}
