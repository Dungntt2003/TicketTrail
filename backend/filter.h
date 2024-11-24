#ifndef FILTER_H
#define FILTER_H

#include <stdio.h>
#include <string.h>

#define MAX_FLIGHTS 100
#define MAX_STRING_LENGTH 50

typedef struct {
    char departure[MAX_STRING_LENGTH];
    char destination[MAX_STRING_LENGTH];
    char departureDate[MAX_STRING_LENGTH];
    int passengers;
    char seatClass[MAX_STRING_LENGTH];
    float price;
} Flight;

void printFlights(const Flight flights[], int count);
int filterFlight(Flight flights[], int flightCount, const char *departure,
                   const char *destination, const char *departureDate,
                   const char *seatClass, Flight filterFlights[]);
void printCheapestFlight(const Flight flights[], int count);
void sortFlightsByPriceAsc(Flight flights[], int count);
void sortFlightsByPriceDesc(Flight flights[], int count);

#endif