#include <gtk/gtk.h>
#include "component.h"
#include <pango/pango.h>
#include <pango/pangocairo.h>
#include "../global/global.h"
#include "../homepage/homepage.h"
void on_home_button_clicked(GtkWidget *widget, gpointer data) {
    if (widget == NULL) {
        g_print("Error: widget is NULL\n");
        return;
    }
    g_print("Home button clicked!\n");
    GtkWidget *homepage_widget = create_homepage_window();
    set_content(homepage_widget);
}

static void on_button_toggled(GtkToggleButton *button, gpointer user_data) {
    GtkWidget **buttons = (GtkWidget **)user_data;

    if (buttons == NULL) {
        g_print("Error: buttons array is NULL\n");
        return;
    }

    if (gtk_toggle_button_get_active(button)) {
        for (int i = 0; buttons[i] != NULL; i++) {
            if (buttons[i] != GTK_WIDGET(button)) {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttons[i]), FALSE);
            }
        }
    }
}
void get_notifications_data(const char ***messages, const char ***dates, int *count) {
    static const char *msgs[] = {
        "Vietjet Air xin thông báo delay chuyến bay từ HN đến HCM vào 17:00 xuống 21:00",
        "Vietnam Airlines xin thông báo delay chuyến bay từ HCM đến Đà Nẵng vào 14:30 xuống 15:30"
    };
    static const char *dates_array[] = {
        "17/12/2024",
        "17/12/2024"
    };

    *messages = msgs;
    *dates = dates_array;
    *count = 2;
}

void show_notification(GtkWidget *widget, gpointer data) {
    // Lấy dữ liệu các thông báo và ngày báo
    const char **messages;
    const char **dates;
    int count;
    get_notifications_data(&messages, &dates, &count);

    // Tạo popover chứa thông báo
    GtkWidget *popover = gtk_popover_new(GTK_WIDGET(data));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
    // Duyệt qua các thông báo và hiển thị
    for (int i = 0; i < count; i++) {
        // Tạo Label cho thông báo
        GtkWidget *message_label = gtk_label_new(messages[i]);
        gtk_box_pack_start(GTK_BOX(box), message_label, FALSE, FALSE, 0);
          gtk_widget_set_halign(message_label, GTK_ALIGN_START); // Căn trái
        
        // Tạo Label cho ngày báo và gán nội dung
        GtkWidget *date_label = gtk_label_new(dates[i]);
        gtk_box_pack_start(GTK_BOX(box), date_label, FALSE, FALSE, 0);
         gtk_widget_set_halign(date_label, GTK_ALIGN_START); // Căn trái

         // Thay đổi màu sắc cho ngày (màu nhạt)
        GdkRGBA color;
        gdk_rgba_parse(&color, "#A9A9A9"); // Màu xám nhạt
        gtk_widget_override_color(date_label, GTK_STATE_FLAG_NORMAL, &color);
    }

    // Thêm box vào popover
    gtk_container_add(GTK_CONTAINER(popover), box);

    // Hiển thị popover
    gtk_widget_show_all(popover);

    // Hiển thị popover ở vị trí gần nút
    gtk_popover_set_relative_to(GTK_POPOVER(popover), widget);
    gtk_popover_popup(GTK_POPOVER(popover));
}
GtkWidget* create_header(GtkWidget **buttons, GtkWidget *parent_container) {
    GtkWidget *header, *logo, *menu_box, *home_button, *ticket_button, *account_button, *notification_button;

    // Tạo box chứa header
    header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(header, 1060, 40);

    gtk_widget_set_name(header, "header");
    gtk_widget_set_margin_top(header, 10);
    gtk_widget_set_margin_start(header, 20);
    gtk_widget_set_margin_end(header, 280);
    gtk_widget_set_margin_bottom(header, 10);

    // Logo
    logo = gtk_image_new_from_file("../assets/images/logo.png");
    gtk_box_pack_start(GTK_BOX(header), logo, FALSE, FALSE, 10);
    gtk_widget_set_margin_end(logo, 250);

    // Menu box
    menu_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_set_homogeneous(GTK_BOX(menu_box), TRUE);

    // Tạo các nút
    home_button = gtk_toggle_button_new_with_label("Home");
    ticket_button = gtk_toggle_button_new_with_label("Ticket");
    account_button = gtk_toggle_button_new_with_label("Account");
    notification_button = gtk_toggle_button_new_with_label("Notification");

    // Áp dụng CSS cho các nút
    GtkCssProvider *button_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(button_provider,
        "button {"
        "  font-family: 'Poppins', sans-serif;"
        "  font-size: 16px;"
        "  font-weight: 600;"
        "  letter-spacing: 0.06em;"
        "  background: transparent;"
        "  color: #283841;"
        "  padding: 10px 10px;"
        "  border-radius: 10px;"
        "  text-align: center;"
        "  border: none;"
        "  margin-right: 20px;"
        "}"
        "button:hover {"
        "  background: #223A60;"
        "  font-weight: 800;"
        "  cursor: pointer;"
        "  color: white;"
        "}"
        "button:active {"
        "  background: #223A60;"
        "  color: white;"
        "}"
        "button:checked {"
        "  background: #223A60;"
        "  color: white;"
        "}",
        -1, NULL);

    // Thêm CSS cho từng nút
    gtk_style_context_add_provider(gtk_widget_get_style_context(home_button), GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(gtk_widget_get_style_context(ticket_button), GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(gtk_widget_get_style_context(account_button), GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(gtk_widget_get_style_context(notification_button), GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Cài đặt kích thước nút
    gtk_widget_set_size_request(home_button, 100, 40);
    gtk_widget_set_size_request(ticket_button, 100, 40);
    gtk_widget_set_size_request(account_button, 100, 40);
    gtk_widget_set_size_request(notification_button, 130, 40);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(home_button), TRUE);

    // Thêm các nút vào menu box
    gtk_box_pack_start(GTK_BOX(menu_box), home_button, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(menu_box), ticket_button, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(menu_box), account_button, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(menu_box), notification_button, TRUE, TRUE, 10);

    // Thêm menu box vào header
    gtk_box_pack_start(GTK_BOX(header), menu_box, TRUE, TRUE, 0);

    // Lưu các nút vào mảng
    buttons[0] = home_button;
    buttons[1] = ticket_button;
    buttons[2] = account_button;
    buttons[3] = notification_button;
    buttons[4] = NULL;

    // Kết nối tín hiệu
    g_signal_connect(home_button, "toggled", G_CALLBACK(on_button_toggled), buttons);
    g_signal_connect(ticket_button, "toggled", G_CALLBACK(on_button_toggled), buttons);
    g_signal_connect(account_button, "toggled", G_CALLBACK(on_button_toggled), buttons);
    g_signal_connect(notification_button, "toggled", G_CALLBACK(on_button_toggled), buttons);
    g_signal_connect(notification_button, "clicked",G_CALLBACK(show_notification), window);
    g_signal_connect(home_button, "clicked", G_CALLBACK(on_home_button_clicked), NULL);

    return header;
}
