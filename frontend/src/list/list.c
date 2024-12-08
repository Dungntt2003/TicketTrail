#include "list.h"

static int current_page = 0;
static GtkWidget *list_box;
static GtkWidget *label_page_info;

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

static void update_flight_list() {
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(list_box));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    int start_index = current_page * FLIGHTS_PER_PAGE;
    int end_index = start_index + FLIGHTS_PER_PAGE;

    for (int i = start_index; i < MAX_FLIGHTS && i < end_index; i++) {
        GtkWidget *flight_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        gtk_widget_set_name(flight_box, "ticket_box");

        gchar info[200];
        g_snprintf(info, sizeof(info), "Airline: %s\nDeparture: %s\nArrival: %s\nClass: %s\nPrice: %.2f",
                   flights[i].airline, flights[i].departure_time, flights[i].arrival_time, flights[i].class, flights[i].price);
        GtkWidget *label = gtk_label_new(info);

        GtkWidget *check_button = gtk_button_new_with_label("Check");
        gtk_widget_set_name(check_button, "check_button");

        gtk_box_pack_start(GTK_BOX(flight_box), label, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(flight_box), check_button, FALSE, FALSE, 0);

        gtk_box_pack_start(GTK_BOX(list_box), flight_box, FALSE, FALSE, 10);
    }

    gchar page_info[50];
    g_snprintf(page_info, sizeof(page_info), "Page %d of %d", current_page + 1, (MAX_FLIGHTS + FLIGHTS_PER_PAGE - 1) / FLIGHTS_PER_PAGE);
    gtk_label_set_text(GTK_LABEL(label_page_info), page_info);

    gtk_widget_show_all(list_box);
}

static void change_page(int offset) {
    int new_page = current_page + offset;
    int total_pages = (MAX_FLIGHTS + FLIGHTS_PER_PAGE - 1) / FLIGHTS_PER_PAGE;

    if (new_page >= 0 && new_page < total_pages) {
        current_page = new_page;
        update_flight_list();
    }
}

static void on_filter_changed(GtkComboBox *combo_box, gpointer user_data) {
    gint active_index = gtk_combo_box_get_active(combo_box);
    if (active_index == 0) {
        sort_flights(TRUE);
    } else if (active_index == 1) {
        sort_flights(FALSE);
    }
    update_flight_list();
}

static GtkWidget* create_pagination_box() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *prev_button = gtk_button_new_with_label("Previous");
    GtkWidget *next_button = gtk_button_new_with_label("Next");
    label_page_info = gtk_label_new("");

    g_signal_connect(prev_button, "clicked", G_CALLBACK(change_page), GINT_TO_POINTER(-1));
    g_signal_connect(next_button, "clicked", G_CALLBACK(change_page), GINT_TO_POINTER(1));

    gtk_box_pack_start(GTK_BOX(box), prev_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), label_page_info, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), next_button, TRUE, TRUE, 0);

    return box;
}

static GtkWidget* create_filter_box() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *filter_label = gtk_label_new("Filter by price:");
    GtkWidget *combo_box = gtk_combo_box_text_new();

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "Low to High");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "High to Low");

    g_signal_connect(combo_box, "changed", G_CALLBACK(on_filter_changed), NULL);

    gtk_box_pack_start(GTK_BOX(box), filter_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), combo_box, FALSE, FALSE, 0);

    return box;
}

void create_flight_list_widget() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Flight List");
    // gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
     gtk_window_fullscreen(GTK_WINDOW(window));

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "../../assets/css/list.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    GtkWidget *filter_box = create_filter_box();
    gtk_box_pack_start(GTK_BOX(main_box), filter_box, FALSE, FALSE, 0);

    list_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(main_box), list_box, TRUE, TRUE, 0);

    GtkWidget *pagination_box = create_pagination_box();
    gtk_box_pack_start(GTK_BOX(main_box), pagination_box, FALSE, FALSE, 0);

    update_flight_list();

    gtk_widget_show_all(window);
     gtk_main();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    create_flight_list_widget();
    return 0;
}

