#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"  // Địa chỉ IP của server
#define SERVER_PORT 8080       // Cổng của server

GtkWidget *entry_username, *entry_password, *label_status;

void on_register_clicked(GtkWidget *widget, gpointer data) {
    const char *username = gtk_entry_get_text(GTK_ENTRY(entry_username));
    const char *password = gtk_entry_get_text(GTK_ENTRY(entry_password));
    
    if (strlen(username) == 0 || strlen(password) == 0) {
        gtk_label_set_text(GTK_LABEL(label_status), "Vui lòng nhập đầy đủ thông tin!");
        return;
    }

    // Tạo socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        gtk_label_set_text(GTK_LABEL(label_status), "Không thể tạo socket!");
        return;
    }

    // Cấu hình địa chỉ server
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Kết nối tới server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        gtk_label_set_text(GTK_LABEL(label_status), "Không thể kết nối đến server!");
        close(sock);
        return;
    }

    // Chuẩn bị và gửi dữ liệu
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "username=%s&password=%s", username, password);
    send(sock, buffer, strlen(buffer), 0);

    // Nhận phản hồi từ server
    char response[256];
    int bytes_received = recv(sock, response, sizeof(response) - 1, 0);
    if (bytes_received > 0) {
        response[bytes_received] = '\0';
        gtk_label_set_text(GTK_LABEL(label_status), response);  // Hiển thị phản hồi
    } else {
        gtk_label_set_text(GTK_LABEL(label_status), "Không nhận được phản hồi từ server!");
    }

    close(sock);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Tạo cửa sổ chính
    GtkWidget *window, *grid, *button_register;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Đăng ký");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Tạo lưới bố cục
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Label và entry cho tên đăng nhập
    GtkWidget *label_username = gtk_label_new("Tên đăng nhập:");
    gtk_grid_attach(GTK_GRID(grid), label_username, 0, 0, 1, 1);

    entry_username = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_username, 1, 0, 1, 1);

    // Label và entry cho mật khẩu
    GtkWidget *label_password = gtk_label_new("Mật khẩu:");
    gtk_grid_attach(GTK_GRID(grid), label_password, 0, 1, 1, 1);

    entry_password = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry_password), FALSE);  // Ẩn mật khẩu
    gtk_grid_attach(GTK_GRID(grid), entry_password, 1, 1, 1, 1);

    // Nút đăng ký
    button_register = gtk_button_new_with_label("Đăng ký");
    gtk_grid_attach(GTK_GRID(grid), button_register, 0, 2, 2, 1);
    g_signal_connect(button_register, "clicked", G_CALLBACK(on_register_clicked), NULL);

    // Label hiển thị trạng thái
    label_status = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid), label_status, 0, 3, 2, 1);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
