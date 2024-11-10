#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>
#include <time.h>

#define MAX_NAME_LENGTH 50
#define MAX_EMAIL_LENGTH 100
#define MAX_PHONE_LENGTH 15
#define MAX_PASSWORD_LENGTH 65

typedef struct {
    char email[MAX_EMAIL_LENGTH];
    char name[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char hashed_password[MAX_PASSWORD_LENGTH];
    char created_at[30];
} User;

bool register_user(const char *email, const char *name, const char *phone, const char *password);
bool login_user(const char *email, const char *password);
bool validate_email(const char *email);
bool validate_phone(const char *phone);
bool validate_password(const char *password);
void hash_password(const char *password, char *hashed_password);
void get_current_time(char *buffer, size_t size);

#endif
