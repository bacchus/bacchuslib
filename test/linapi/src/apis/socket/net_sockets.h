#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUF_SIZE 256

void netsoc_srv(const char* port_number) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("socket() failed: %d\n", errno);
        return;
    }

    sockaddr_in serv_addr;
    memset((char*) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    int port = atoi(port_number);
    serv_addr.sin_port = htons(port);
    int err = bind(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));
    if (err < 0) {
        printf("bind() failed: %d\n", errno);
        return;
    }

    sockaddr_in cli_addr;
    unsigned int clen = sizeof(cli_addr);
    listen(sock, 1);
    int newsock = accept(sock, (sockaddr*)&cli_addr, &clen);
    if (newsock < 0) {
        printf("accept() failed: %d\n", errno);
        return;
    }

    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);
    read(newsock, buf, BUF_SIZE-1);
    buf[BUF_SIZE] = 0;
    printf("MSG: %s\n", buf);
    write(newsock, "OK", 2);
    close(newsock);
    close(sock);
}

void netsoc_cli(const char* hostname, const char* port_number) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("socket() failed: %d", errno);
        return;
    }

    hostent* server = gethostbyname(hostname);
    if (server == NULL) {
        printf("Host not found\n");
        return;
    }

    sockaddr_in serv_addr;
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    int port = atoi(port_number);
    serv_addr.sin_port = htons(port);
    int err = connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));
    if (err < 0) {
        printf("connect() failed: %d", errno);
        return;
    }

    printf(">");
    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);
    fgets(buf, BUF_SIZE-1, stdin);
    write(sock, buf, strlen(buf));
    memset(buf, 0, BUF_SIZE);
    read(sock, buf, BUF_SIZE-1);
    printf("%s\n",buf);
    close(sock);
}
