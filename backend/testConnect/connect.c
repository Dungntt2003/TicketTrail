#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../airline/airline.h"
#include "../user/user.h"

#define MAX_LENGTH 255

int main() {
    Airline *airlines = NULL;
    int count = 0;

    if (fetch_airlines(&airlines, &count) != 0) {
        fprintf(stderr, "Failed to fetch airlines.\n");
        return 1;
    }

    printf("Fetched %d airlines:\n", count);
    for (int i = 0; i < count; i++) {
        printf("ID: %d, Name: %s, Description: %s, Contact email: %s, Contact phone: %s, Logo url: %s\n", 
            airlines[i].id, airlines[i].name, airlines[i].description, 
            airlines[i].contact_email, airlines[i].contact_phone, airlines[i].logo_url);
    }

    free(airlines);

    User *user= NULL;
    int countUser = 0;

    if (fetch_users(&user, &countUser) != 0) {
        fprintf(stderr, "Failed to fetch users.\n");
        return 1;
    }

    printf("Fetched %d users:\n", count);
    for (int i = 0; i < countUser; i++) {
        printf("ID: %d, Name: %s, Phone: %s, Contact email: %s, Password: %s\n", 
            user[i].user_id, user[i].name, user[i].phone, 
            user[i].email, user[i].password);
    }

    free(user);

    User newUser;
    strcpy(newUser.name, "John Doe");
    strcpy(newUser.phone, "123456789");
    strcpy(newUser.email, "john@example.com");
    strcpy(newUser.password, "securepassword");

    if (add_user(&newUser) != 0) {
        fprintf(stderr, "Failed to add user.\n");
        return 1;
    }

    printf("User added successfully.\n");

    return 0;
}