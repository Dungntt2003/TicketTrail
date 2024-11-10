#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int calculate_checksum(const char *data) {
    int checksum = 0;
    while (*data) {
        checksum += *data++;
    }
    return checksum;
}


void *handle_client(void *client_socket) {
    int sock = *(int*)client_socket;
    char buffer[BUFFER_SIZE];
    int read_size;

    while ((read_size = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[read_size] = '\0'; 

        const char *header = "RESPONSE_HEADER";
        int checksum = calculate_checksum(buffer);

        char response[BUFFER_SIZE];
        snprintf(response, BUFFER_SIZE, "%s|%s|%d", header, buffer, checksum);

        send(sock, response, strlen(response), 0);
        printf("Sent to client: %s\n", response);
    }

    close(sock);
    free(client_socket);
    return 0;
}

int main() {
    int server_socket, client_socket, *new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    pthread_t thread_id;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    listen(server_socket, 3);
    printf("Waiting for connections...\n");

    while ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size))) {
        new_sock = malloc(1);
        *new_sock = client_socket;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)new_sock) < 0) {
            perror("Could not create thread");
            return 1;
        }
    }

    close(server_socket);
    return 0;
}
