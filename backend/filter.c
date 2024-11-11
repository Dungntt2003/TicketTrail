#include <stdio.h>
#include <string.h>

#define MAX_FLIGHTS 100
#define MAX_STRING_LENGTH 50

typedef struct {
    char departure[MAX_STRING_LENGTH];
    char destination[MAX_STRING_LENGTH];
    char departureDate[MAX_STRING_LENGTH];
    char returnDate[MAX_STRING_LENGTH];
    int passengers;
    char seatClass[MAX_STRING_LENGTH];
    float price;
} Flight;

void filterFlights(Flight flights[], int flightCount, const char *departure,
                   const char *destination, const char *departureDate,
                   const char *returnDate, int passengers, const char *seatClass) {
    for (int i = 0; i < flightCount; i++) {
        Flight flight = flights[i];
        int matches = 1;

        // Kiểm tra điều kiện từng tiêu chí
        if (departure != NULL && strcmp(flight.departure, departure) != 0) {
            matches = 0;
        }
        if (destination != NULL && strcmp(flight.destination, destination) != 0) {
            matches = 0;
        }
        if (departureDate != NULL && strcmp(flight.departureDate, departureDate) != 0) {
            matches = 0;
        }
        if (returnDate != NULL && strcmp(flight.returnDate, returnDate) != 0) {
            matches = 0;
        }
        if (passengers >= 0 && flight.passengers != passengers) {
            matches = 0;
        }
        if (seatClass != NULL && strcmp(flight.seatClass, seatClass) != 0) {
            matches = 0;
        }

        // Nếu tất cả các điều kiện đều thỏa mãn, in thông tin chuyến bay
        if (matches) {
            printf("Flight from %s to %s on %s returning on %s with %d passengers in %s class, Price: %.2f\n",
                   flight.departure, flight.destination, flight.departureDate, flight.returnDate,
                   flight.passengers, flight.seatClass, flight.price);
        }
    }
}

int main() {
    Flight flights[MAX_FLIGHTS] = {
        {"Hanoi", "Ho Chi Minh", "2023-12-01", "2023-12-10", 2, "Economy", 100.50},
        {"Hanoi", "Da Nang", "2023-12-05", "2023-12-12", 1, "Business", 150.75},
        {"Hanoi", "Ho Chi Minh", "2023-12-02", "2023-12-09", 3, "Economy", 90.00},
        {"Hanoi", "Nha Trang", "2023-12-03", "2023-12-08", 1, "First", 200.00}
    };

    int flightCount = 4;

    // gọi hàm
    filterFlights(flights, flightCount, "Hanoi", NULL, NULL, NULL, -1, NULL);

    return 0;
}