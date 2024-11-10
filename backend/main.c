#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "auth.h"

void display_main_menu() {
    printf("\n1. Đăng ký\n");
    printf("2. Đăng nhập\n");
    printf("3. Thoát\n");
    printf("Chọn một tùy chọn: ");
}

void display_logged_in_menu() {
    printf("\n1. Đăng xuất\n");
    printf("2. Thoát\n");
    printf("Chọn một tùy chọn: ");
}

int main() {
    bool is_logged_in = false;
    int option;

    while (1) {
        if (!is_logged_in) {
            display_main_menu();
        } else {
            display_logged_in_menu();
        }

        scanf("%d", &option);
        getchar();

        if (!is_logged_in) {
            if (option == 1) {
                char email[MAX_EMAIL_LENGTH];
                char name[MAX_NAME_LENGTH];
                char phone[MAX_PHONE_LENGTH];
                char password[MAX_PASSWORD_LENGTH];

                do {
                    printf("Nhập email: ");
                    fgets(email, sizeof(email), stdin);
                    email[strcspn(email, "\n")] = 0;
                    if (!validate_email(email)) {
                        printf("Email không đúng định dạng. Vui lòng nhập lại email:\n");
                    }
                } while (!validate_email(email));

                printf("Nhập tên: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                do {
                    printf("Nhập số điện thoại: ");
                    fgets(phone, sizeof(phone), stdin);
                    phone[strcspn(phone, "\n")] = 0;
                    if (!validate_phone(phone)) {
                        printf("Số điện thoại không đúng định dạng. Vui lòng nhập lại số điện thoại:\n");
                    }
                } while (!validate_phone(phone));

                do {
                    printf("Nhập mật khẩu: ");
                    fgets(password, sizeof(password), stdin);
                    password[strcspn(password, "\n")] = 0;
                    if (!validate_password(password)) {
                        printf("Mật khẩu phải có ít nhất 8 ký tự, bao gồm chữ hoa, chữ thường và số. Vui lòng nhập lại:\n");
                    }
                } while (!validate_password(password));

                if (register_user(email, name, phone, password)) {
                    is_logged_in = true;
                }
            } else if (option == 2) {
                char email[MAX_EMAIL_LENGTH];
                char password[MAX_PASSWORD_LENGTH];

                printf("Nhập email: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;

                printf("Nhập mật khẩu: ");
                fgets(password, sizeof(password), stdin);
                password[strcspn(password, "\n")] = 0;

                if (login_user(email, password)) {
                    printf("Đăng nhập thành công!\n");
                    is_logged_in = true;
                }
            } else if (option == 3) {
                printf("Tạm biệt!\n");
                break;
            } else {
                printf("Lựa chọn không hợp lệ.\n");
            }
        } else {
            if (option == 1) {
                printf("Đã đăng xuất.\n");
                is_logged_in = false;
            } else if (option == 2) {
                printf("Tạm biệt!\n");
                break;
            } else {
                printf("Lựa chọn không hợp lệ.\n");
            }
        }
    }
    return 0;
}
