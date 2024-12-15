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

void parse_flight_data(char *data, Flight *flights) {
    int count = 0;
    char *line = strtok(data, "\n");

    while (line != NULL) {
        Flight flight;
        if (strncmp(line, "Id ", 3) == 0) {
            strcpy(flight.flight_id, line + 3);
        }

        line = strtok(NULL, "\n");
        if (strncmp(line, "Time ", 5) == 0) {
            strcpy(flight.departure_time, line + 5);
        }

        line = strtok(NULL, "\n");
        if (strncmp(line, "Duration ", 9) == 0) {
            // strcpy(flight.duration_minutes, line + 9);
            flight.duration_minutes = atoi(line + 9);
        }

        line = strtok(NULL, "\n");
        if (strncmp(line, "Capacity ", 9) == 0) {
            // strcpy(flight.capacity, line + 9);
            flight.capacity = atoi(line + 9);
        }

         line = strtok(NULL, "\n");
        if (strncmp(line, "Price ", 6) == 0) {
            // strcpy(flight.price, line + 6);
            flight.price = atoi(line + 6);
        }

        line = strtok(NULL, "\n");
        if (strncmp(line, "Name ", 5) == 0) {
            strcpy(flight.airplane_name, line + 5);
        }

        line = strtok(NULL, "\n");
        if (strncmp(line, "Departure ", 10) == 0) {
            strcpy(flight.departure_airport, line + 10);
        }

        line = strtok(NULL, "\n");
        if (strncmp(line, "Arrival ", 8) == 0) {
            strcpy(flight.arrival_airport, line + 8);
        }

        line = strtok(NULL, "\n");
        if (strncmp(line, "Economy ", 8) == 0) {
            // strcpy(flight.available_economy, line + 8);
            flight.available_economy = atoi(line + 8);
        }

        line = strtok(NULL, "\n");
        if (strncmp(line, "Business ", 9) == 0) {
            // strcpy(flight.available_business, line + 9);
            flight.available_business = atoi(line + 9);
        }

        line = strtok(NULL, "\n");
        if (strncmp(line, "First ", 6) == 0) {
            // strcpy(flight.available_first_class, line + 6);
            flight.available_first_class = atoi(line + 6);
        }
        flights[count++] = flight; 
        line = strtok(NULL, "\n"); 
    }

}
