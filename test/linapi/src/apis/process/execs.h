#pragma once

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int exec(char* argv[]) {
    printf("Starting %s...\n", argv[0]);

    int pid = fork();
    if (pid == 0) {
        execvp(argv[0], &argv[0]);//execve putenv
        perror("execvp");
        throw 1;
        return EXIT_FAILURE; // Never get there normally
    } else {
        printf("Child %i\n", pid);
        int status;
        if (wait(&status) == -1) { //waitpid waitid
            perror("wait");
            return EXIT_FAILURE;
        }
        if (WIFEXITED(status))
            printf("Child terminated normally with exit code %i\n", WEXITSTATUS(status));
        if (WIFSIGNALED(status))
            printf("Child was terminated by a signal #%i\n", WTERMSIG(status));
        if (WCOREDUMP(status))
            printf("Child dumped core\n");
        if (WIFSTOPPED(status))
            printf("Child was stopped by a signal #%i\n", WSTOPSIG(status));
    }
    return EXIT_SUCCESS;
}
