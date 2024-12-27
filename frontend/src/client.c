#include "./global/global.h"
#include "./homepage/homepage.h"
#include "./server_com/server_com.h"
#include "./register/register.h"
// #include "./cardinfo/cardinfo.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

int main(int argc, char *argv[]) {
  sock = connect_to_server(SERVER_IP, PORT);
  gtk_init(&argc, &argv);
  GtkWidget *main_window = create_main_window();
  gtk_widget_show_all(main_window);

  GtkWidget *register_window = create_register_window();
  set_content(register_window);
  gtk_main();
  close(sock);
  return 0;
}
