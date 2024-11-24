#include "filter.h"
#include <stdio.h>
#include <string.h>


void printFlights(const Flight flights[], int count){
     printf("Danh sách chuyến bay:\n");
    for (int i = 0; i < count; i++) {
         printf("Chuyến bay %d:\n", i + 1);
        printf("  Khởi hành: %s\n", flights[i].departure);
        printf("  Điểm đến: %s\n", flights[i].destination);
        printf("  Ngày khởi hành: %s\n", flights[i].departureDate);
        printf("  Số hành khách: %d\n", flights[i].passengers);
        printf("  Hạng ghế: %s\n", flights[i].seatClass);
        printf("  Giá: %.2f\n", flights[i].price);
        printf("\n");
    }
}

int filterFlight(Flight flights[], int flightCount, const char *departure,
                   const char *destination, const char *departureDate,
                   const char *seatClass, Flight filterFlights[]) {
    int filterCount = 0; 

    for (int i = 0; i < flightCount; i++) {
        Flight flight = flights[i];
        int matches = 1; 

        if (strcmp(departure,"NULL") != 0 && strcmp(flight.departure, departure) != 0) {
            matches = 0; 
        }
        if (strcmp(destination,"NULL") != 0 && strcmp(flight.destination, destination) != 0) {
            matches = 0; 
        }
        if (strcmp(departureDate,"NULL") != 0 && strcmp(flight.departureDate, departureDate) != 0) {
            matches = 0; 
        }
        if (strcmp(seatClass,"NULL") != 0 && strcmp(flight.seatClass, seatClass) != 0) {
            matches = 0;
        }

        if (matches) {
            filterFlights[filterCount] = flight; 
            filterCount++; 
        }
    }

    return filterCount;
}

void printCheapestFlight(const Flight flights[], int count) {
    if (count <= 0) {
        printf("Không có chuyến bay nào để hiển thị.\n");
        return;
    }

    int minIndex = 0; 
    float minPrice = flights[0].price; 

    for (int i = 1; i < count; i++) {
        if (flights[i].price < minPrice) {
            minPrice = flights[i].price;
            minIndex = i; 
        }
    }

    Flight cheapestFlight = flights[minIndex];
    printf("Chuyến bay có giá thấp nhất:\n");
    printf("Khởi hành: %s\n", cheapestFlight.departure);
    printf("Điểm đến: %s\n", cheapestFlight.destination);
    printf("Ngày khởi hành: %s\n", cheapestFlight.departureDate);
    printf("Số hành khách: %d\n", cheapestFlight.passengers);
    printf("Hạng ghế: %s\n", cheapestFlight.seatClass);
    printf("Giá: %.2f\n", cheapestFlight.price);
}

void sortFlightsByPriceAsc(Flight flights[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (flights[j].price > flights[j + 1].price) {
                Flight temp = flights[j];
                flights[j] = flights[j + 1];
                flights[j + 1] = temp;
            }
        }
    }
}

void sortFlightsByPriceDesc(Flight flights[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (flights[j].price < flights[j + 1].price) {
                Flight temp = flights[j];
                flights[j] = flights[j + 1];
                flights[j + 1] = temp;
            }
        }
    }
}