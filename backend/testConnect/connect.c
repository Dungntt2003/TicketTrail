#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../booking/booking.h"
#include "../bookingDetail/detail.h"
int main() {
    time_t rawtime;
    struct tm *timeinfo;
    char booking_time[20];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(booking_time, sizeof(booking_time), "%Y-%m-%d %H:%M:%S", timeinfo);
    int total_price = 1000000;
    int user_id = 3;
    const char *flight_id = "VN007";
    int insert_id = 0;

    if (insert_booking(booking_time, total_price, user_id, flight_id, &insert_id) != 0) {
        fprintf(stderr, "Failed to insert booking\n");
        return EXIT_FAILURE;
    }

    printf("Booking inserted successfully with ID: %d\n", insert_id);

    const char *type = "Economy";
    const char *seat_code = "5A";
    if (insert_booking_detail(insert_id, type, seat_code) != 0) {
        fprintf(stderr, "Failed to insert booking detail\n");
        return EXIT_FAILURE;
    }
    printf("Insert done\n");

    return EXIT_SUCCESS;
}