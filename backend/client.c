#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "auth.h"
#include "filter.h"

#define PORT 8080
#define BUFFER_SIZE 1024

bool is_logged_in = false;
int option;
Flight flights[MAX_FLIGHTS];
int count;
const char *nullValue = "NULL";

void display_main_menu() {
    printf("\n1. Đăng ký\n");
    printf("2. Đăng nhập\n");
    printf("3. Thoát\n");
    printf("Chọn một tùy chọn: ");
}

void display_logged_in_menu() {
    printf("\n1. Đăng xuất\n");
    printf("2. Xem danh sách chuyến bay\n");
    printf("3. Lọc chuyến bay\n");
    printf("4. Quản lý vé đã đặt\n");
    printf("5. Thông báo\n");
    printf("6. Thoát\n");
    printf("Chọn một tùy chọn: ");
}


int main() {
    
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char message[100];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }
    while (1) {
        if (!is_logged_in) {
            display_main_menu();
        } else {
            display_logged_in_menu();
        }

        scanf("%d", &option);
        getchar();
        send(sock, &option, sizeof(option), 0);

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
                send(sock, email, strlen(email), 0);

                printf("Nhập tên: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                send(sock, name, strlen(name), 0);

                do {
                    printf("Nhập số điện thoại: ");
                    fgets(phone, sizeof(phone), stdin);
                    phone[strcspn(phone, "\n")] = 0;
                    if (!validate_phone(phone)) {
                        printf("Số điện thoại không đúng định dạng. Vui lòng nhập lại số điện thoại:\n");
                    }
                } while (!validate_phone(phone));
                send(sock, phone, strlen(phone), 0);

                do {
                    printf("Nhập mật khẩu: ");
                    fgets(password, sizeof(password), stdin);
                    password[strcspn(password, "\n")] = 0;
                    if (!validate_password(password)) {
                        printf("Mật khẩu phải có ít nhất 8 ký tự, bao gồm chữ hoa, chữ thường và số. Vui lòng nhập lại:\n");
                    }
                } while (!validate_password(password));
                send(sock, password, strlen(password), 0); 
                recv(sock, buffer, sizeof(buffer), 0);
                if (strcmp(buffer, "Success") == 0){
                    is_logged_in = true;
                    printf("Đăng ký thành công\n");
                }
                else printf("Thất bại, vui lòng đăng ký lại\n");
            } 
            else if (option == 2) {
                char email[MAX_EMAIL_LENGTH];
                char password[MAX_PASSWORD_LENGTH];

                printf("Nhập email: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;
                send(sock, email, sizeof(email), 0);

                printf("Nhập mật khẩu: ");
                fgets(password, sizeof(password), stdin);
                password[strcspn(password, "\n")] = 0;
                send(sock, password, sizeof(password), 0);
                recv(sock, buffer, sizeof(buffer), 0);
                printf("Receive: %s\n", buffer);
                if (strcmp(buffer, "Success") == 0) {
                    printf("Đăng nhập thành công\n");
                    is_logged_in = true;
                }
                else printf("Vui lòng thử đăng nhập lại\n");
            }
             else if (option == 3) {
                printf("Tạm biệt!\n");
                break;
            } else {
                printf("Lựa chọn không hợp lệ.\n");
            }
        } else {
            recv(sock, &count, sizeof(count), 0);
            recv(sock, flights, sizeof(Flight) * count, 0);
            if (option == 1) {
                printf("Đã đăng xuất.\n");
                is_logged_in = false;
            } else if (option == 2){
                printFlights(flights, count);
            } else if (option == 3){
                Flight filterFlights[MAX_FLIGHTS];
                char departure[BUFFER_SIZE];
                char destination[BUFFER_SIZE];
                char departureDate[BUFFER_SIZE];
                char seatClass[BUFFER_SIZE];
                int filteCount;
                printf("Nhập tên sân bay đi: ");
                fgets(departure, sizeof(departure), stdin);
                departure[strcspn(departure, "\n")] = 0;
                if (strlen(departure) == 0) {
                send(sock, nullValue, strlen(nullValue), 0);
                } else {
                    send(sock, departure, strlen(departure), 0);
                }


                printf("Nhập tên sân bay đến: ");
                fgets(destination, sizeof(destination), stdin);
                destination[strcspn(destination, "\n")] = 0;
                if (strlen(destination) == 0) {
                send(sock, nullValue, strlen(nullValue), 0);
                } else {
                    send(sock, destination, strlen(destination), 0);
                }
                
                printf("Nhập ngày bay: ");
                fgets(departureDate, sizeof(departureDate), stdin);
                departureDate[strcspn(departureDate, "\n")] = 0;
                if (strlen(departureDate) == 0) {
                send(sock, nullValue, strlen(nullValue), 0);
                } else {
                    send(sock, departureDate, strlen(departureDate), 0);
                }
                
                printf("Nhập hạng vé bạn muốn: ");
                fgets(seatClass, sizeof(seatClass), stdin);
                seatClass[strcspn(seatClass, "\n")] = 0;
                if (strlen(seatClass) == 0) {
                send(sock, nullValue, strlen(nullValue), 0);
                } else {
                    send(sock, seatClass, strlen(seatClass), 0);
                }
                recv(sock, &filteCount, sizeof(filteCount), 0);
                recv(sock, filterFlights, sizeof(Flight)*filteCount, 0);
                printf("Số chuyến bay khớp: %d\n", filteCount);
                printFlights(filterFlights, filteCount);
                printCheapestFlight(filterFlights, filteCount);
                // printFlights(filterFlights, filteCount);
                printf("Danh sách các chuyến bay khớp với giá tăng dần: \n");
                sortFlightsByPriceAsc(filterFlights, filteCount);
                printFlights(filterFlights, filteCount);
                printf("Danh sách các chuyến bay khớp với giá giảm dần: \n");
                sortFlightsByPriceDesc(filterFlights, filteCount);
                printFlights(filterFlights, filteCount);
            }
            else if (option == 6) {
                printf("Tạm biệt!\n");
                break;
            } else {
                printf("Lựa chọn không hợp lệ.\n");
            }
        }
    }


    close(sock);
    return 0;
}
