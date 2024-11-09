#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char message[100];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    do {
        printf("Send to server: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0; 

        send(sock, message, strlen(message), 0);
        printf("Message sent to server: %s\n", message);

        int read_size = recv(sock, buffer, BUFFER_SIZE, 0);
        if (read_size > 0) {
            buffer[read_size] = '\0';
            printf("Received from server: %s\n", buffer);
        }

    } while (strcmp(message, "bye") != 0);


    close(sock);
    return 0;
}
