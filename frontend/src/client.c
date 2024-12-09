#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "./server_com/server_com.h"
#include "./register/register.h"
#include "./global/global.h"


#define PORT 8080
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"


int main(int argc, char *argv[]) {
    sock = connect_to_server(SERVER_IP, PORT); 
    
    gtk_init(&argc, &argv);
    create_register_widget();
    gtk_main();
    close(sock);
    return 0;
}
