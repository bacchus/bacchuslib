#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FTOK_FILE "./shmemserv"
#define MAXLEN 512

enum eBusy {
    BUSY = 0,
    FREE = 1
};

enum eRelation {
    CLIENT = 0,
    SERVER = 1
};

struct SharedMem {

    struct MemoryBlock {
        int lock[2];
        int turn;
        int readlast;
        char string[MAXLEN];
    };

    MemoryBlock* mblock;
    int shmid;
    eRelation is_serv;

    SharedMem(eRelation rel)
        : is_serv(rel)
    {
        key_t key = ftok(FTOK_FILE, 1); // generate key
        if (key == -1) {
            printf("Failed to generate unique key. Server compiled with a wrong name?\n");
            throw 1;
        }

        unsigned int create_flag = (is_serv==SERVER ? IPC_CREAT : 0);
        shmid = shmget(key, sizeof(MemoryBlock), 0666 | create_flag);
        if (shmid == -1) {
            printf("Server is not running!\n");
            throw 1;
        }

        // attach shmem
        mblock = (MemoryBlock*)shmat(shmid, 0, 0);
    }

    ~SharedMem() {
        shmdt((void *) mblock);  // detach shmem
        if (is_serv == SERVER) {
            printf("Server got q and exits\n");
            shmctl(shmid, IPC_RMID, 0);     // delete shmem
        } else {
            printf("Client exits\n");
        }
    }

    void wait_for() {
        mblock->lock[is_serv] = BUSY;
        mblock->turn = 1 - is_serv;
        while ((mblock->lock[1 - is_serv] == BUSY)
               && (mblock->turn == 1 - is_serv));
    }

};

void shmem_srv() {
    SharedMem sm(SERVER);

    sm.mblock->turn = CLIENT;
    sm.mblock->lock[SERVER] = FREE;
    sm.mblock->lock[CLIENT] = FREE;
    sm.mblock->readlast = SERVER;

    strcpy(sm.mblock->string, "Hello!");

    while (strcmp("q\n", sm.mblock->string) != 0) {
        sm.wait_for();

        if (sm.mblock->readlast == CLIENT) {
            sm.mblock->readlast = SERVER;

            printf("String sent by the client is: %s", sm.mblock->string);
            if (strcmp("q\n", sm.mblock->string) != 0)
                strcpy(sm.mblock->string, "Ok!");

            sm.mblock->lock[SERVER] = FREE;
        }
    }

    printf("Server got q and exits\n");
}

void shmem_cli() {
    SharedMem sm(CLIENT);

    while (strcmp("q\n", sm.mblock->string) != 0) {
        sm.wait_for();

        int i = 0;
        if (sm.mblock->readlast == SERVER) {
            sm.mblock->readlast = CLIENT;
            printf("Server sends %s\n", sm.mblock->string);
            while ((i < (MAXLEN - 1)) && ((sm.mblock->string[i++] = getchar()) !=  '\n') );
            sm.mblock->string[i] = 0;
            sm.mblock->lock[CLIENT] = FREE;
        }
    }

    printf("Client exits\n");
}
