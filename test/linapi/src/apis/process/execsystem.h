#pragma once

#include <stdlib.h>
#include <stdio.h>

int execsystem(char* argv) {
    printf("Starting %s...\n", argv);
    int result = system(argv);
    switch (result) {
    case 127 :
        printf("Failed to start a shell\n");
        return EXIT_FAILURE;
    case -1 :
        printf("Unknown failure\n");
        return EXIT_FAILURE;
    default :
        printf ("Done\n");
    }
    return EXIT_SUCCESS;
}
