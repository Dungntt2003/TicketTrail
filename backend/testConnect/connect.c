#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../flight/flight.h"

int main() {
    int seat_count = 0;
    char **seats = get_seat_codes_by_flight_id("VN007", &seat_count);
    if (seats != NULL) {
        for (int i = 0; i < seat_count; i++) {
            printf("Seat code: %s\n", seats[i]);
            free(seats[i]); 
        }
        free(seats);  
    } else {
        printf("No seats found or error occurred.\n");
    }
    return 0;
}