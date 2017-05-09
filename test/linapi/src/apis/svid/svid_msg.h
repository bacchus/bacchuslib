#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define KEY 1174 // magic number (uniq id) or use ftok
#define MAXLEN 512

struct msg_t {
    long mtype;
    int snd_pid;
    char body[MAXLEN];
};


typedef msg_t msg_1_t;
struct msg_2_t {
    long mtype;
    int snd_pid;
    int rcv_pid;
    char body[MAXLEN];
};

#define MSG_1_SIZE sizeof(msg_1_t) - sizeof(long)
#define MSG_2_SIZE sizeof(msg_2_t) - sizeof(long)

int msg_srv() {
    const char* response = "Ok!";
    int msgid = msgget(KEY, 0666 | IPC_CREAT);  // create msg queue
    msg_t message;
    msgrcv(msgid, &message, sizeof(message) - sizeof(message.mtype), 2, 0); // wait message
    // 2 - mtype, 0 - flags (IPC_NOWAIT - error if no message with mtype=2
    printf("Client (pid = %i) sent: %s", message.snd_pid, message.body);

    message.mtype = 1; // create respond
    message.snd_pid = getpid();
    strcpy(message.body, response);

    msgsnd(msgid, &message, sizeof(message) - sizeof(message.mtype), 0); // send respond
    msgrcv(msgid, &message, sizeof(message) - sizeof(message.mtype), 2, 0); // wait confirm
    msgctl(msgid, IPC_RMID, 0); // delete msg queue
    return 0;
}

int msg_cli() {
    int msgid = msgget(KEY, 0666);  // get msg queue id
    if (msgid == -1) {
        printf("Server is not running!\n");
        return 1;
    }

    msg_t message;// create message
    int i = 0;
    while ( (i < (MAXLEN - 1)) && ((message.body[i++] = getchar()) !=  '\n') );
    message.body[i] = '\0';
    message.mtype = 2;
    message.snd_pid = getpid();

    msgsnd(msgid, &message, sizeof(message) - sizeof(message.mtype), 0);  // send msg
    msgrcv(msgid, &message, sizeof(message) - sizeof(message.mtype), 1, 0);  // wait respond
    printf("Server (pid= %i) responded: %s\n", message.snd_pid, message.body);

    message.mtype = 2;
    msgsnd(msgid, &message, sizeof(message), 0);  // send confirm
    return 0;
}

int poly_msg_srv() {
    const char* response = "Ok!";
    int msgid = msgget(KEY, 0777 | IPC_CREAT);  // create msg queue
    msg_1_t message1;
    msgrcv(msgid, &message1, MSG_1_SIZE, 1, 0);  // wait message
    printf("Client (pid = %i) sent: %s", message1.snd_pid, message1.body);

    msg_2_t message2;
    message2.mtype = 2;
    message2.snd_pid = getpid();
    message2.rcv_pid = message1.snd_pid;
    strcpy(message2.body, response);

    msgsnd(msgid, &message2, MSG_2_SIZE, 0); // send respond
    msgrcv(msgid, &message1, MSG_1_SIZE, 1, 0);  // wait confirm
    msgctl(msgid, IPC_RMID, 0);  // delete msg queue
    return 0;
}

int poly_msg_cli() {
    int msgid = msgget(KEY, 0666);  // get msg queue id
    if (msgid == -1) {
        printf("Server is not running!\n");
        return 1;
    }

    int i = 0;
    msg_1_t message1;
    while ( (i < (MAXLEN - 1)) && ((message1.body[i++] = getchar()) !=  '\n') );
    message1.body[i] = '\0';
    message1.mtype = 1;
    message1.snd_pid = getpid();

    msgsnd(msgid, &message1, MSG_1_SIZE, 0);  // send msg
    struct msg_2_t message2;
    msgrcv(msgid, &message2, MSG_2_SIZE, 2, 0);  // wait respond
    printf("Server (pid= %i) responded: %s\n", message2.snd_pid, message2.body);

    message1.mtype = 1;
    msgsnd(msgid, &message1, MSG_1_SIZE, 0);  // // send confirm
    return 0;
}
