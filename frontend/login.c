#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "homepage.h"
#include "../backend/auth/auth.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

GtkWidget *window, *overlay, *login_box, *label_login_title, *button_login, *hbox_footer;
GtkWidget *entry_email, *entry_password, *label_status;
char buffer[BUFFER_SIZE];

void on_login_clicked(GtkWidget *widget, gpointer data) {
    const char *email = gtk_entry_get_text(GTK_ENTRY(entry_email));
    const char *password = gtk_entry_get_text(GTK_ENTRY(entry_password));

    if (strlen(email) == 0 || strlen(password) == 0) {
        gtk_label_set_text(GTK_LABEL(label_status), "Please fill in all fields!");
        return;
    }

    if (!validate_email(email)){
         gtk_label_set_text(GTK_LABEL(label_status), "Email is not valid");
        return;
    }

    if (!validate_password(password)){
         gtk_label_set_text(GTK_LABEL(label_status), "Password has to be at least 1 number, 1 uppercase, 6 characters");
        return;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        gtk_label_set_text(GTK_LABEL(label_status), "Cannot create socket!");
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        gtk_label_set_text(GTK_LABEL(label_status), "Cannot connect to server!");
        close(sock);
        return;
    }
    int option = 2;
    send(sock, &option, sizeof(option), 0);
    snprintf(buffer, sizeof(buffer), "%s;%s", email, password);

    send(sock, buffer, strlen(buffer) + 1, 0);
    recv(sock, buffer, sizeof(buffer), 0);
    g_print("Receive: %s\n", buffer);
    if (strcmp(buffer, "Success") == 0) {
        gtk_label_set_text(GTK_LABEL(label_status), "Login successfully");
         gtk_widget_hide(window);
        create_homepage_window();
    }
    else  {
        gtk_label_set_text(GTK_LABEL(label_status), "Please try again, email or password is not match");
        // gtk_widget_hide(window);
        // create_homepage_window();
    }

    

    
    // send(sock, buffer, strlen(buffer), 0);

    // char response[256];
    // int bytes_received = recv(sock, response, sizeof(response) - 1, 0);
    // if (bytes_received > 0) {
    //     response[bytes_received] = '\0';
    //     gtk_label_set_text(GTK_LABEL(label_status), response);
    // } else {
    //     gtk_label_set_text(GTK_LABEL(label_status), "No response from server!");
    // }

    close(sock);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);


    // Tạo cửa sổ chính
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Login");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600); // Kích thước ban đầu
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "* { background-color: #050A24; }"
        "#login-box { background-color: #FFFFFF; border-radius: 20px; padding: 48px 72px; }"
        "#login-title { font-family: Poppins; font-size: 28px; font-weight: 600; color: #101828; background-color: transparent; }"
        "#email-label, #password-label { font-family: Poppins; font-size: 18px; font-weight: 500; color: #344054; background-color: transparent; }"
        "#email-entry, #password-entry { border: 3px solid #D0D5DD; border-radius: 8px; padding: 12px 16px; color: #344054; background-color: #FFFFFF; }"
        "#login-button { color: #FCFCFD; border-radius: 8px; font-family: Poppins; font-weight: 600; font-size: 16px; background-color: #1570EF;}"
        "#login-button:hover { background-color: #125ECB; }"
        "#footer-text, #sign-up { font-family: Poppins; font-size: 16px; font-weight: 400; color: #98A2B3; background-color: transparent; }"
        "#sign-up { color: #1570EF; text-decoration: underline; }",
        -1, NULL);

    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);


    
    // Sử dụng Overlay để căn giữa login_box
    overlay = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(window), overlay);

    // Login box
    login_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 24); // Sử dụng gap giữa các thành phần
    gtk_widget_set_name(login_box, "login-box");
    GtkStyleContext *login_box_context = gtk_widget_get_style_context(login_box);
    gtk_style_context_add_provider(login_box_context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), login_box);
    gtk_widget_set_halign(login_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(login_box, GTK_ALIGN_CENTER);

    // Tiêu đề
    label_login_title = gtk_label_new("Login to your account");
    gtk_widget_set_name(label_login_title, "login-title");
    GtkStyleContext *title_context = gtk_widget_get_style_context(label_login_title);
    gtk_style_context_add_provider(title_context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_box_pack_start(GTK_BOX(login_box), label_login_title, FALSE, FALSE, 0);

    // Email label và input
    GtkWidget *label_email = gtk_label_new("Email");
    gtk_widget_set_name(label_email, "email-label");
    gtk_box_pack_start(GTK_BOX(login_box), label_email, FALSE, FALSE, 0);

    entry_email = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_email), "Enter your email");
    gtk_widget_set_name(entry_email, "email-entry");
    GtkStyleContext *email_entry_context = gtk_widget_get_style_context(entry_email);
    gtk_style_context_add_provider(email_entry_context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_box_pack_start(GTK_BOX(login_box), entry_email, FALSE, FALSE, 0);

    // Password label và input
    GtkWidget *label_password = gtk_label_new("Password");
    gtk_widget_set_name(label_password, "password-label");
    gtk_box_pack_start(GTK_BOX(login_box), label_password, FALSE, FALSE, 0);

    entry_password = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_password), "Enter your password");
    gtk_entry_set_visibility(GTK_ENTRY(entry_password), FALSE);
    gtk_widget_set_name(entry_password, "password-entry");
    GtkStyleContext *password_entry_context = gtk_widget_get_style_context(entry_password);
    gtk_style_context_add_provider(password_entry_context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_box_pack_start(GTK_BOX(login_box), entry_password, FALSE, FALSE, 0);

    // Nút đăng nhập
    button_login = gtk_button_new_with_label("Login now");
    gtk_widget_set_name(button_login, "login-button");
    g_signal_connect(button_login, "clicked", G_CALLBACK(on_login_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(login_box), button_login, FALSE, FALSE, 0);

     label_status = gtk_label_new("");
    gtk_widget_set_name(label_status, "status-label");
    gtk_box_pack_start(GTK_BOX(login_box), label_status, FALSE, FALSE, 0);

    // Footer text
    hbox_footer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    GtkWidget *footer_text = gtk_label_new("Don't have an account?");
    gtk_widget_set_name(footer_text, "footer-text");
    gtk_box_pack_start(GTK_BOX(hbox_footer), footer_text, FALSE, FALSE, 0);

    GtkWidget *sign_up = gtk_label_new("Sign up?");
    gtk_widget_set_name(sign_up, "sign-up");
    gtk_box_pack_start(GTK_BOX(hbox_footer), sign_up, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(login_box), hbox_footer, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
