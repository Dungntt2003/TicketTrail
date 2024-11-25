#ifndef AIRLINE_H
#define AIRLINE_H

#include "../db/db.h"
#define MAX_LENGTH 255

typedef struct {
    int id;
    char name[MAX_LENGTH];
    char description[MAX_LENGTH];
    char contact_email[MAX_LENGTH];
    char contact_phone[MAX_LENGTH];
    char logo_url[MAX_LENGTH];
} Airline;

int fetch_airlines(Airline **airlines, int *count);

#endif 