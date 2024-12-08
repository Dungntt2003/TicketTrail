#include <gtk/gtk.h>
#include "list.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../ticket_detail/ticket_detail.h"
#define MAX_LENGTH 1024

// Dữ liệu chuyến bay mẫu
static Flight flights[MAX_FLIGHTS] = {
    {"Airline A", "10:00", "12:00", "Economy", 100.0},
    {"Airline B", "14:00", "16:00", "Business", 200.0},
    {"Airline C", "09:00", "11:00", "First Class", 300.0},
    {"Airline D", "15:00", "17:00", "Economy", 150.0},
    {"Airline E", "08:00", "10:00", "Business", 250.0},
    {"Airline F", "11:00", "13:00", "Economy", 120.0},
    {"Airline G", "12:00", "14:00", "Business", 220.0},
    {"Airline H", "13:00", "15:00", "First Class", 320.0},
    {"Airline I", "16:00", "18:00", "Economy", 130.0},
    {"Airline J", "17:00", "19:00", "Business", 230.0}
};

// Biến toàn cục
GtkWidget *ticket_list; // Danh sách vé
GtkWidget *main_box;    // Main box
char buffer[MAX_LENGTH];
int sock;

//Link 
void on_detail_link_click(GtkWidget *widget, gpointer data) {
        gtk_widget_destroy(main_box);
        create_ticket_detail_window(sock);
}

// Khai báo hàm on_button_toggled
static void on_button_toggled(GtkToggleButton *button, gpointer user_data);

// Hàm tạo header
GtkWidget* create_header(GtkWidget **buttons) {
    GtkWidget *header, *logo, *menu_box, *home_button, *ticket_button, *account_button;

    header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(header, 1080, 40);
    gtk_widget_set_name(header, "header");
    gtk_widget_set_margin_top(header, 10);
    gtk_widget_set_margin_start(header, 20); 
    gtk_widget_set_margin_end(header, 300); 
    gtk_widget_set_margin_bottom(header, 10);

    logo = gtk_image_new_from_file("../../assets/images/logo.png");
    gtk_box_pack_start(GTK_BOX(header), logo, FALSE, FALSE, 10);
    gtk_widget_set_margin_end(logo, 250); 

    menu_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_set_homogeneous(GTK_BOX(menu_box), TRUE);

    home_button = gtk_toggle_button_new_with_label("Home");
    ticket_button = gtk_toggle_button_new_with_label("Ticket");
    account_button = gtk_toggle_button_new_with_label("Account");

    // CSS cho các nút
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

    gtk_style_context_add_provider(gtk_widget_get_style_context(home_button), GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(gtk_widget_get_style_context(ticket_button), GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(gtk_widget_get_style_context(account_button), GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_widget_set_size_request(home_button, 100, 40);
    gtk_widget_set_size_request(ticket_button, 100, 40);
    gtk_widget_set_size_request(account_button, 100, 40);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(home_button), TRUE);

    gtk_box_pack_start(GTK_BOX(menu_box), home_button, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(menu_box), ticket_button, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(menu_box), account_button, TRUE, TRUE, 10);

    gtk_box_pack_start(GTK_BOX(header), menu_box, TRUE, TRUE, 0);

    buttons[0] = home_button;
    buttons[1] = ticket_button;
    buttons[2] = account_button;
    buttons[3] = NULL;  

    g_signal_connect(home_button, "toggled", G_CALLBACK(on_button_toggled), buttons);
    g_signal_connect(ticket_button, "toggled", G_CALLBACK(on_button_toggled), buttons);
    g_signal_connect(account_button, "toggled", G_CALLBACK(on_button_toggled), buttons);

    return header;
}

// Cài đặt hàm on_button_toggled
static void on_button_toggled(GtkToggleButton *button, gpointer user_data) {
    GtkWidget **buttons = (GtkWidget **)user_data;

    if (gtk_toggle_button_get_active(button)) {
        for (int i = 0; buttons[i] != NULL; i++) {
            if (buttons[i] != GTK_WIDGET(button)) {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttons[i]), FALSE);
            }
        }
    }
}

// Hàm tạo nội dung vé
GtkWidget* create_ticket_list() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    for (int i = 0; i < MAX_FLIGHTS; i++) {
        GtkWidget *ticket_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        
        // Tạo các label cho thông tin vé
        GtkWidget *airline_label = gtk_label_new(flights[i].airline);
        GtkWidget *departure_time_label = gtk_label_new(flights[i].departure_time);
        GtkWidget *arrival_time_label = gtk_label_new(flights[i].arrival_time);
        GtkWidget *class_label = gtk_label_new(flights[i].class);
        GtkWidget *price_label = gtk_label_new(g_strdup_printf("$%.2f", flights[i].price));

        GtkWidget *check_button = gtk_button_new_with_label("Check");
         gtk_widget_set_name(check_button, "check_button");
        g_signal_connect(check_button, "clicked", G_CALLBACK(on_detail_link_click),main_box);

        // Thêm các label vào ticket_box
        gtk_box_pack_start(GTK_BOX(ticket_box), airline_label, TRUE, TRUE, 5);
        gtk_box_pack_start(GTK_BOX(ticket_box), departure_time_label, TRUE, TRUE, 5);
        gtk_box_pack_start(GTK_BOX(ticket_box), arrival_time_label, TRUE, TRUE, 5);
        gtk_box_pack_start(GTK_BOX(ticket_box), class_label, TRUE, TRUE, 5);
        gtk_box_pack_start(GTK_BOX(ticket_box), price_label, TRUE, TRUE, 5);
        gtk_box_pack_start(GTK_BOX(ticket_box), check_button, FALSE, FALSE, 5);

        // Thêm ticket_box vào main box
        gtk_box_pack_start(GTK_BOX(box), ticket_box, FALSE, FALSE, 0);

        // CSS cho ticket_box
        gtk_widget_set_size_request(ticket_box, 800, 40);
        gtk_widget_set_name(ticket_box, "ticket_box");
    }
    
    return box;
}

// Hàm làm mới danh sách vé
void refresh_ticket_list(GtkWidget *container) {
   GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(ticket_list));
    
    // Xóa các widget hiện có trong list
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    // Tạo lại danh sách vé mới và thêm vào container
    GtkWidget *new_ticket_list = create_ticket_list();
    gtk_box_pack_start(GTK_BOX(container), new_ticket_list, TRUE, TRUE, 0);
    gtk_widget_show_all(container); // Hiển thị tất cả các widget trong container
}
// Hàm sắp xếp vé
void sort_flights(gboolean ascending) {
    for (int i = 0; i < MAX_FLIGHTS; i++) {
        for (int j = i + 1; j < MAX_FLIGHTS; j++) {
            if ((ascending && flights[i].price > flights[j].price) ||
                (!ascending && flights[i].price < flights[j].price)) {
                Flight temp = flights[i];
                flights[i] = flights[j];
                flights[j] = temp;
            }
        }
    }
}

// Hàm xử lý khi người dùng chọn sắp xếp
void on_sort_changed(GtkComboBox *combo, gpointer user_data) {
    gint active_index = gtk_combo_box_get_active(combo);
    if (active_index == 0) {
        sort_flights(TRUE);
    } else if (active_index == 1) {
        sort_flights(FALSE);
    }

    // Gọi hàm làm mới danh sách vé
    refresh_ticket_list(ticket_list); // Làm mới ticket_list
}
GtkWidget* create_filter_box() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *filter_label = gtk_label_new("Filter by price:");
    GtkWidget *combo_box = gtk_combo_box_text_new();

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "Low to High");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "High to Low");

    g_signal_connect(combo_box, "changed", G_CALLBACK(on_sort_changed), NULL);

    gtk_box_pack_start(GTK_BOX(box), filter_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), combo_box, FALSE, FALSE, 0);

    return box;
}
// Hàm tạo list window
GtkWidget* create_list_window() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Danh sách vé");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    
    // Tạo header
    GtkWidget *buttons[4];
    GtkWidget *header = create_header(buttons);

    // Tạo main box
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // Đặt main_box là biến toàn cục
    gtk_box_pack_start(GTK_BOX(main_box), header, FALSE, FALSE, 0);

        GtkWidget *filter_box = create_filter_box();
    gtk_box_pack_start(GTK_BOX(main_box), filter_box, FALSE, FALSE, 0);

    // Tạo nội dung vé
    ticket_list = create_ticket_list(); // Đặt ticket_list là biến toàn cục
    gtk_box_pack_start(GTK_BOX(main_box), ticket_list, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(window), main_box);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    return window;
}

// Hàm chính
void create_flight_list_widget() {
    GtkWidget *list_window = create_list_window();
    gtk_widget_show_all(list_window);
    gtk_main();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);  
    create_flight_list_widget(); 
    return 0;                
}