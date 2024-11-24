#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "auth.h"
#include "filter.h"

#define PORT 8080
#define BUFFER_SIZE 1024

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
    recv(sock, &option, sizeof(option), 0);
        if (!is_logged_in){
            if (option==1){
                recv(sock, email, sizeof(email), 0);
                printf("Email: %s\n", email);
                recv(sock, name, sizeof(name), 0);
                printf("Name: %s\n", name);
                recv(sock, phone, sizeof(phone), 0);
                printf("Phone: %s\n", phone);
                recv(sock, password, sizeof(password), 0);
                printf("Address: %s\n", password);  
                if (register_user(email, name, phone, password)){
                    is_logged_in = true;
                    send(sock, "Success", sizeof("Success"), 0);
                } 
                else send(sock, "Fail", sizeof("Fail"), 0);           
            }
            else if (option == 2){
                recv(sock, email, sizeof(email), 0);
                printf("Email: %s\n", email);
                recv(sock, password, sizeof(password), 0);
                printf("Password: %s\n", password);
                if (login_user(email, password)){
                    is_logged_in = true;
                    send(sock, "Success", sizeof("Success"), 0);
                }
                else send(sock, "Fail", sizeof("Fail"), 0);  
            } else {
                printf("Client out\n");
                break;
            }
        }
        else {
            Flight filterFlights[MAX_FLIGHTS];
            char departure[BUFFER_SIZE];
            char destination[BUFFER_SIZE];
            char departureDate[BUFFER_SIZE];
            char seatClass[BUFFER_SIZE];
            send(sock, &flightCount, sizeof(flightCount), 0);
            send(sock, flights, sizeof(Flight) * flightCount, 0);
            if (option == 1){
                printf("Client log out\n");
                is_logged_in= false;
            }
            else if (option == 2){
                printf("Print list flights for client\n");
            }  else if (option == 3){
                recv(sock, departure, sizeof(departure), 0);
                printf("Departure: %s\n", departure);
                recv(sock, destination, sizeof(destination), 0);
                printf("Destination: %s\n", destination);
                recv(sock, departureDate, sizeof(departureDate), 0);
                printf("Departure Date: %s\n", departureDate);
                recv(sock, seatClass, sizeof(seatClass), 0);
                printf("Seat class: %s\n", seatClass);
                int filterCount = filterFlight(flights, flightCount, departure, destination, departureDate, seatClass, filterFlights);
                printf("Print list flight after filtering\n");
                send(sock, &filterCount, sizeof(filterCount), 0);
                send(sock, filterFlights, sizeof(Flight) * filterCount, 0);
            }
            else if (option == 6){
                printf("Client out!\n");
                break;
            }
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
