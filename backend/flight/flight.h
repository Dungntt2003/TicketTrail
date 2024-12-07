#ifndef FLIGHT_H
#define FLIGHT_H

#include "../db/db.h"
#define MAX_LENGTH 255

typedef struct {
    int id;
    char flight_number[MAX_LENGTH];
    char departure_time[MAX_LENGTH];
    char name[MAX_LENGTH];
} Flight;

int fetch_flights(Flight **flight, int *count);

#endif 