// homepage.c
#include <gtk/gtk.h>
#include "homepage.h"  // Bao gồm header của homepage

void create_homepage_window() {
    // Tạo cửa sổ trang chủ
    GtkWidget *window_homepage = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window_homepage), "Homepage");
    gtk_window_set_default_size(GTK_WINDOW(window_homepage), 800, 600);
    gtk_window_set_position(GTK_WINDOW(window_homepage), GTK_WIN_POS_CENTER);
    
    // Thêm widget vào cửa sổ trang chủ
    GtkWidget *label_homepage = gtk_label_new("Welcome to the Homepage!");
    gtk_container_add(GTK_CONTAINER(window_homepage), label_homepage);
    
    // Hiển thị tất cả các widget trong cửa sổ trang chủ
    gtk_widget_show_all(window_homepage);
}
