#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "./auth/auth.h"
#include "./filter/filter.h"

#define PORT 8080
#define BUFFER_SIZE 1024
char *token;
  Flight flights[MAX_FLIGHTS] = {
        {"Hanoi", "Ho Chi Minh", "2023-12-01", 2, "Economy", 100.50},
        {"Hanoi", "Da Nang", "2023-12-05", 1, "Business", 150.75},
        {"Hanoi", "Ho Chi Minh", "2023-12-02", 3, "Economy", 90.00},
        {"Hanoi", "Nha Trang", "2023-12-03", 1, "First", 200.00}
    };

    int flightCount = 4;



int calculate_checksum(const char *data) {
    int checksum = 0;
    while (*data) {
        checksum += *data++;
    }
    return checksum;
}


char* returnMsg (char* message){
    const char *header = "RESPONSE_HEADER";
    int checksum = calculate_checksum(message);

    char *response = (char*)malloc(BUFFER_SIZE);
    if (response == NULL) {
        return NULL;
    }
    snprintf(response, BUFFER_SIZE, "%s|%s|%d", header, message, checksum);
    return response; 
}

void *handle_client(void *client_socket) {
    bool is_logged_in = false;
    int option=10;
    char int_str[12];
    char email[BUFFER_SIZE];
    char name[BUFFER_SIZE];
    char phone[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    int sock = *(int*)client_socket;
    char buffer[BUFFER_SIZE];
    int read_size;

    while (1) {
        if (!is_logged_in){
           recv(sock, buffer, sizeof(buffer), 0);
           buffer[strcspn(buffer, "\n")] = 0;
           if (strncmp(buffer, "REGISTER", strlen("REGISTER")) == 0){
                printf("%s\n", buffer);      
                token = strtok(buffer, " ");
    
                token = strtok(NULL, ":");
                strcpy(name, token);

                token = strtok(NULL, ":");
                strcpy(phone, token);
                
                token = strtok(NULL, ":");
                strcpy(email, token);
                
                token = strtok(NULL, ":");
                strcpy(password, token);
                
                printf("Check name: %s\n", name);
                printf("Check phone: %s\n", phone);
                printf("Check email: %s\n", email);
                printf("Check password: %s\n", password);
                if (checkEmailExist(email)){
                    send(sock, "EMAIL_EXISTED", strlen("EMAIL_EXISTED") + 1, 0);
                }else {
                    if (register_user(email, name, phone, password)){
                        send(sock, "SUCCESS", strlen("SUCCESS") + 1, 0);
                    }
                    else send(sock, "FAILED", strlen("FAILED") + 1, 0);
                }
            } else if (strncmp(buffer, "LOGIN", strlen("LOGIN")) == 0){
                printf("%s\n", buffer);      
                token = strtok(buffer, " ");
                
                token = strtok(NULL, ":");
                strcpy(email, token);
                
                token = strtok(NULL, ":");
                strcpy(password, token);

                printf("Check email: %s\n", email);
                printf("Check password: %s\n", password);
                if (!checkEmailExist(email)){
                    send(sock, "EMAIL_NOT_FOUND", strlen("EMAIL_NOT_FOUND") + 1, 0);
                }else {
                    if (login_user(email, password)){
                        send(sock, "SUCCESS", strlen("SUCCESS") + 1, 0);
                        is_logged_in = true;
                    }
                    else send(sock, "FAILED", strlen("FAILED") + 1, 0);
                }
            }
        }
        else {
           
        }
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
