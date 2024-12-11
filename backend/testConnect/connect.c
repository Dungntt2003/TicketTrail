#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../flight/flight.h"

int main() {
    Flight *flights = NULL;
    int count = 0;

    if (fetch_flights(&flights, &count) != 0) {
        fprintf(stderr, "Failed to fetch flights.\n");
        return 1;
    }

    printf("Fetched %d flights:\n", count);
    for (int i = 0; i < count; i++) {
        printf("%s %s %d %d %d %s %s %s %d %d %d\n", flights[i].flight_id, flights[i].departure_time, 
        flights[i].duration_minutes, flights[i].capacity, flights[i].price, flights[i].airplane_name,
        flights[i].departure_airport, flights[i].arrival_airport, flights[i].available_economy,
        flights[i].available_business, flights[i].available_first_class);
    }

    free(flights);

    return 0;
}