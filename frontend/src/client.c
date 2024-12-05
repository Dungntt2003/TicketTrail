#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "./server_com/server_com.h"
#include "./register/register.h"


#define PORT 8080
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define MAX_LENGTH 1024

bool is_logged_in = false;
int option;
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

int main(int argc, char *argv[]) {
    int sock = connect_to_server(SERVER_IP, PORT); 
    char buffer[BUFFER_SIZE];
    
    gtk_init(&argc, &argv);
    create_register_widget(sock);
    gtk_main();
    close(sock);
    return 0;
}
