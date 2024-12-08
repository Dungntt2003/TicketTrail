#include <gtk/gtk.h>
#include "list.h"

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

// Hàm để sắp xếp vé theo giá
static void sort_flights(gboolean ascending) {
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

// Hàm xử lý sự kiện cho nút sắp xếp
static void on_sort_button_clicked(GtkButton *button, gpointer user_data) {
    gboolean ascending = (gpointer)user_data == GINT_TO_POINTER(1);
    sort_flights(ascending);
    
    // Làm mới danh sách vé
    GtkWidget *parent_box = gtk_widget_get_parent(gtk_widget_get_parent(button));
    gtk_widget_destroy(parent_box);
    
    GtkWidget *ticket_list = create_ticket_list();
    gtk_box_pack_start(GTK_BOX(parent_box), ticket_list, TRUE, TRUE, 0);
    gtk_widget_show_all(parent_box);
}

// Tạo các nút sắp xếp
GtkWidget* create_sort_buttons() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    
    GtkWidget *asc_button = gtk_button_new_with_label("Sắp xếp Tăng dần");
    GtkWidget *desc_button = gtk_button_new_with_label("Sắp xếp Giảm dần");

    g_signal_connect(asc_button, "clicked", G_CALLBACK(on_sort_button_clicked), GINT_TO_POINTER(1));
    g_signal_connect(desc_button, "clicked", G_CALLBACK(on_sort_button_clicked), GINT_TO_POINTER(0));

    gtk_box_pack_start(GTK_BOX(box), asc_button, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box), desc_button, TRUE, TRUE, 10);

    return box;
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
    // Ví dụ: tạo các ô vé
    for (int i = 0; i < MAX_FLIGHTS; i++) {
        GtkWidget *ticket_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        
        // Tạo các label cho thông tin vé
        GtkWidget *airline_label = gtk_label_new(flights[i].airline);
        GtkWidget *departure_time_label = gtk_label_new(flights[i].departure_time);
        GtkWidget *arrival_time_label = gtk_label_new(flights[i].arrival_time);
        GtkWidget *class_label = gtk_label_new(flights[i].class);
        GtkWidget *price_label = gtk_label_new(g_strdup_printf("$%.2f", flights[i].price));
        GtkWidget *check_button = gtk_button_new_with_label("Check");

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

// Hàm tạo list window
GtkWidget* create_list_window() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "List of Tickets");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    
    // Tạo header
    GtkWidget *buttons[4];
    GtkWidget *header = create_header(buttons);

    // Tạo main box
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(main_box), header, FALSE, FALSE, 0);

    // Tạo nút sắp xếp và thêm vào main box
    GtkWidget *sort_buttons = create_sort_buttons();
    gtk_box_pack_start(GTK_BOX(main_box), sort_buttons, FALSE, FALSE, 0);
    
    // Tạo nội dung vé
    GtkWidget *ticket_list = create_ticket_list();
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
    gtk_init(&argc, &argv);  // Khởi tạo GTK
    create_flight_list_widget(); // Gọi hàm tạo widget danh sách
    return 0;                 // Trở về 0 khi kết thúc
}