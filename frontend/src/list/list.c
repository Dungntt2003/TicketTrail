#include <gtk/gtk.h>
#include "list.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include  <string.h>
#include "../ticket_detail/ticket_detail.h"
#include "../component/component.h"
#include "../global/global.h"

// Biến toàn cục
GtkWidget *ticket_list; // Danh sách vé
GtkWidget *main_box;    // Main box
GtkWidget *list_window;

//Link 
void on_detail_link_click(GtkWidget *widget, gpointer data) {
     char *flight_id = (char *)data;
     g_print("Check id: %s\n", flight_id);
     for (int i = 0; i < tem_flight_count; i++){
        if (strcmp(tem_flights[i].flight_id, flight_id) == 0){
            detail_flight = tem_flights[i];
        }
     }

    GtkWidget *detail_window = create_ticket_detail_window();
    set_content(detail_window);
}


// Hàm tạo nội dung vé
GtkWidget* create_ticket_list() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    for (int i = 0; i < tem_flight_count; i++) {
        GtkWidget *ticket_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        
        GtkWidget *airline_label = gtk_label_new(tem_flights[i].airplane_name);
        GtkWidget *departure_time_label = gtk_label_new(tem_flights[i].departure_time);
        GtkWidget *arrival_time_label = gtk_label_new(extract_middle_string(tem_flights[i].departure_airport));
        GtkWidget *class_label = gtk_label_new(extract_middle_string(tem_flights[i].arrival_airport));
        GtkWidget *price_label = gtk_label_new(format_number_with_separator(tem_flights[i].price, ','));

        GtkWidget *check_button = gtk_button_new_with_label("Check");
         gtk_widget_set_name(check_button, "check_button");
        g_signal_connect(check_button, "clicked", G_CALLBACK(on_detail_link_click), tem_flights[i].flight_id);

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
    for (int i = 0; i < tem_flight_count; i++) {
        for (int j = i + 1; j < tem_flight_count; j++) {
            if ((ascending && tem_flights[i].price > tem_flights[j].price) ||
                (!ascending && tem_flights[i].price < tem_flights[j].price)) {
                Flight temp = tem_flights[i];
                tem_flights[i] = tem_flights[j];
                tem_flights[j] = temp;
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
    GtkWidget *main_box, *header, *filter_box;

    // Tạo hộp chính
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Tạo header
    GtkWidget *buttons[4];
    header = create_header(buttons);
    gtk_box_pack_start(GTK_BOX(main_box), header, FALSE, FALSE, 0);

    // Tạo hộp lọc
    filter_box = create_filter_box();
    gtk_box_pack_start(GTK_BOX(main_box), filter_box, FALSE, FALSE, 0);

    // Tạo danh sách vé
    ticket_list = create_ticket_list();
    gtk_box_pack_start(GTK_BOX(main_box), ticket_list, TRUE, TRUE, 0);

    return main_box;
}


