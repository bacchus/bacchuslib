#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FTOK_FILE "./semserv"
#define MAXLEN 512

struct memory_block {
    char string[MAXLEN];
};

int sem_srv() {
    key_t key;
    int shmid;
    int pid, semid;
    sembuf buf[2];
    memory_block * mblock;

    key = ftok(FTOK_FILE, 1); // генерация ключа
    if (key == -1) {
        printf("Failed to generate unique key. Server compiler with a wrong name?\n");
        return EXIT_FAILURE;
    }

    semid = semget(key, 3, 0666|IPC_CREAT);
    buf[0].sem_num = 0;
    buf[0].sem_flg = SEM_UNDO;
    buf[1].sem_num = 1;
    buf[1].sem_flg = SEM_UNDO;
    semctl(semid, 0, SETVAL, 0);
    shmid = shmget(key, sizeof(memory_block), 0666 | IPC_CREAT);
    mblock = (memory_block *) shmat(shmid, 0, 0);
    strcpy(mblock->string, "Hello!");
    buf[0].sem_op = -1;
    buf[1].sem_op = 1;
    semop(semid, (struct sembuf*) &buf[1], 1);

    while (strcmp("q\n", mblock->string) != 0) {
        semop(semid, (sembuf*) &buf, 1);
        printf("String sent by the client is: %s", mblock->string);
        if (strcmp("q\n", mblock->string) != 0)
            strcpy(mblock->string, "Ok!");
        buf[0].sem_op = -1;
        buf[1].sem_op = 1;
        semop(semid, (sembuf*) &buf[1], 1);
    }

    printf("Server got q and exits\n");
    shmdt((void *) mblock);
    shmctl(shmid, IPC_RMID, 0);
    semctl(semid, 2, IPC_RMID);
    return EXIT_SUCCESS;
}

int sem_cli() {
    key_t key;
    int shmid;
    struct memory_block * mblock;
    int semid;
    struct sembuf buf[2];

    key = ftok(FTOK_FILE, 1); // генерация ключа
    if (key == -1) {
        printf("Failed to generate unique key. Server compiler with a wrong name?\n");
        return EXIT_FAILURE;
    }

    shmid = shmget(key, sizeof(memory_block), 0666);
    if (shmid == -1) {
        printf("Server is not running!\n");
        return EXIT_FAILURE;
    }

    semid = semget(key, 2, 0666);
    buf[0].sem_num = 0;
    buf[0].sem_flg = SEM_UNDO;
    buf[1].sem_num = 1;
    buf[1].sem_flg = SEM_UNDO;
    mblock = (memory_block *) shmat(shmid, 0, 0);
    buf[1].sem_op = -1;

    while (strcmp("q\n", mblock->string) != 0) {
        int i = 0;
        semop(semid, (sembuf*) &buf[1], 1);
        printf("Server sends %s\n", mblock->string);
        while ((i < (MAXLEN - 1)) && ((mblock->string[i++] = getchar()) !=  '\n') );
        mblock->string[i] = 0;
        buf[0].sem_op = 1;
        buf[1].sem_op = -1;
        semop(semid, (sembuf*) &buf, 1);
    }

    printf("Client exits\n");
    shmdt((void *) mblock);  // удаляем отображение
    return EXIT_SUCCESS;
}
