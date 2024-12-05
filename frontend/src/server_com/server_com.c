#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "server_com.h"

#define BUFFER_SIZE 1024

int connect_to_server(const char *server_ip, int port) {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        exit(EXIT_FAILURE);
    }

    return sock;
}

void send_to_server(int sock, const void *data, size_t size) {
    if (send(sock, data, size, 0) < 0) {
        perror("Send to server failed");
        exit(EXIT_FAILURE);
    }
}

void recv_from_server(int sock, void *buffer, size_t size) {
    if (recv(sock, buffer, size, 0) < 0) {
        perror("Receive from server failed");
        exit(EXIT_FAILURE);
    }
}
