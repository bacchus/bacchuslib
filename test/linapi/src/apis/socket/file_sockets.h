#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SOCK_NAME "socket.soc"

void fssoc_srv() {
    // open socket
    int sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket failed");
        return;
    }

    // bind socket
    sockaddr srvr_name;
    srvr_name.sa_family = AF_UNIX;
    strcpy(srvr_name.sa_data, SOCK_NAME);
    int err = bind(sock, &srvr_name, strlen(srvr_name.sa_data) + sizeof(srvr_name.sa_family));
    if (err < 0) {
        perror("bind failed");
        return;
    }

    // receive message, block program until smth received
    sockaddr rcvr_name;
    const int BUF_SIZE = 256;
    char buf[BUF_SIZE];
    unsigned int namelen;
    int bytes = recvfrom(sock, buf, sizeof(buf),  0, &rcvr_name, &namelen);
    if (bytes < 0) {
        perror("recvfrom failed");
        return;
    }
    buf[bytes] = 0;
    rcvr_name.sa_data[namelen] = 0;

    // print message and clean
    printf("Client sent: %s\n", buf);
    close(sock);
    unlink(SOCK_NAME);
}

void fssoc_cli() {
    // open socket
    int sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket failed");
        return;
    }

    // send message
    const int BUF_SIZE = 256;
    char buf[BUF_SIZE];
    sockaddr srvr_name;
    srvr_name.sa_family = AF_UNIX;
    strcpy(srvr_name.sa_data, SOCK_NAME);
    strcpy(buf, "Hello, Unix sockets!");
    sendto(sock, buf, strlen(buf), 0, &srvr_name, strlen(srvr_name.sa_data) + sizeof(srvr_name.sa_family));
}
