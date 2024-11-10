#include "auth.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <openssl/sha.h>
#include <time.h>

void hash_password(const char *password, char *hashed_password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)password, strlen(password), hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hashed_password + (i * 2), "%02x", hash[i]);
    }
    hashed_password[64] = '\0';
}

void get_current_time(char *buffer, size_t size) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

bool validate_email(const char *email) {
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    return at && dot && at < dot;
}

bool validate_phone(const char *phone) {
    if (strlen(phone) < 10 || strlen(phone) > 15) return false;
    for (int i = 0; i < strlen(phone); i++) {
        if (!isdigit(phone[i])) return false;
    }
    return true;
}

bool validate_password(const char *password) {
    bool has_upper = false, has_lower = false, has_digit = false;
    if (strlen(password) < 8) return false;
    for (int i = 0; i < strlen(password); i++) {
        if (isupper(password[i])) has_upper = true;
        if (islower(password[i])) has_lower = true;
        if (isdigit(password[i])) has_digit = true;
    }
    return has_upper && has_lower && has_digit;
}

bool register_user(const char *email, const char *name, const char *phone, const char *password) {
    FILE *file = fopen("users.txt", "a");
    if (!file) {
        perror("Không thể mở file");
        return false;
    }

    char hashed_password[65];
    hash_password(password, hashed_password);

    char created_at[30];
    get_current_time(created_at, sizeof(created_at));

    fprintf(file, "%s,%s,%s,%s,%s\n", email, name, phone, hashed_password, created_at);
    fclose(file);

    printf("Đăng ký thành công!\n");
    return true;
}

bool login_user(const char *email, const char *password) {
    FILE *file = fopen("users.txt", "r");
    if (!file) {
        perror("Không thể mở file");
        return false;
    }

    char line[256];
    char hashed_password[65];
    hash_password(password, hashed_password);

    while (fgets(line, sizeof(line), file)) {
        User user;
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^\n]",
               user.email, user.name, user.phone, user.hashed_password, user.created_at);

        if (strcmp(user.email, email) == 0 && strcmp(user.hashed_password, hashed_password) == 0) {
            fclose(file);
            return true;
        }
    }
    fclose(file);
    printf("Thông tin đăng nhập không chính xác.\n");
    return false;
}
