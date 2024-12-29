#include "booklist.h"
#include <cairo.h>
#include <cairo-pdf.h>
#include <gtk/gtk.h>
#include <math.h>

// Define ticket data structure
typedef struct {
    const char *departure_city;
    const char *departure_airport;
    const char *arrival_city;
    const char *arrival_airport;
    const char *date;
    const char *time;
    const char *flight_code;
    const char *gate_code;
    const char *seat_code;
    const char *class_type;
    const char *price;
} Ticket;
typedef struct {
    Ticket *tickets;
    int *ticket_count;
    int index;
} ConfirmParams;

// Fake ticket data array
#define NUM_TICKETS 3
Ticket tickets[NUM_TICKETS] = {
    {"HA NOI", "Noi Bai Airport", "HO CHI MINH", "Tan Son Nhat Airport", "08/12/2024", "9:30 PM", "IN 230", "22", "2B, 3B", "Economy", "240000 VND"},
    {"DA NANG", "Da Nang Airport", "HA NOI", "Noi Bai Airport", "09/12/2024", "10:00 AM", "VN 456", "18", "4C, 5C", "Business", "320000 VND"},
    {"HO CHI MINH", "Tan Son Nhat Airport", "PHU QUOC", "Phu Quoc Airport", "10/12/2024", "2:30 PM", "PQ 789", "15", "6A, 7A", "Economy", "180000 VND"}
};
static void show_confirmation_dialog(GtkWidget *parent, const char *message, void (*confirm_action)(void *), void *data) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(parent)),
                                               GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_QUESTION,
                                               GTK_BUTTONS_NONE,
                                               "%s", message);

    GtkWidget *yes_button = gtk_dialog_add_button(GTK_DIALOG(dialog), "Yes", GTK_RESPONSE_YES);
    GtkWidget *no_button = gtk_dialog_add_button(GTK_DIALOG(dialog), "No", GTK_RESPONSE_NO);

    g_signal_connect_swapped(yes_button, "clicked", G_CALLBACK(confirm_action), data);
    g_signal_connect_swapped(no_button, "clicked", G_CALLBACK(gtk_widget_destroy), dialog);

    gtk_widget_show_all(dialog);
}

// Cancel ticket callback
static void on_cancel_confirm(GtkWidget *widget, gpointer user_data) {
    ConfirmParams *params = (ConfirmParams *)user_data;
    Ticket *tickets = params->tickets;
    int *ticket_count = params->ticket_count;
    int index = params->index;

    // Remove the ticket
    for (int i = index; i < (*ticket_count) - 1; i++) {
        tickets[i] = tickets[i + 1];
    }
    (*ticket_count)--;

    // Redraw the ticket list
    GtkWidget *parent_window = gtk_widget_get_toplevel(widget);
    gtk_widget_queue_draw(parent_window);

    g_free(params);
}

// Change ticket callback
static void on_change_confirm(GtkWidget *widget, gpointer user_data) {
    ConfirmParams *params = (ConfirmParams *)user_data;
    g_print("Redirecting to homepage for ticket %d...\n", params->index);

    g_free(params);
}

static void draw_ticket(cairo_t *cr, double ticket_x, double ticket_y, double ticket_width, double ticket_height, const Ticket *ticket) {
    // Draw ticket background with rounded corners
    cairo_set_source_rgb(cr, 0.92, 0.94, 0.94); // Light gray background
    cairo_new_path(cr);
    cairo_arc(cr, ticket_x + 24, ticket_y + 24, 24, M_PI, 3 * M_PI / 2);
    cairo_arc(cr, ticket_x + ticket_width - 24, ticket_y + 24, 24, 3 * M_PI / 2, 2 * M_PI);
    cairo_arc(cr, ticket_x + ticket_width - 24, ticket_y + ticket_height - 24, 24, 0, M_PI / 2);
    cairo_arc(cr, ticket_x + 24, ticket_y + ticket_height - 24, 24, M_PI / 2, M_PI);
    cairo_close_path(cr);
    cairo_fill(cr);

    // Draw departure and arrival cities
    double ticket_left_margin = 50;
    double flight_icon_width = 156;
    double flight_icon_x = ticket_x + ticket_left_margin + 120; // Adjust for city text alignment
    double flight_icon_y = ticket_y + 50;

    double departure_city_x = ticket_x + ticket_left_margin;
    double departure_city_y = flight_icon_y + 10;
    double arrival_city_x = flight_icon_x + flight_icon_width + 50; // Offset after flight icon
    double arrival_city_y = departure_city_y;

    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Black color
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 20);

    // Draw departure city
    cairo_move_to(cr, departure_city_x, departure_city_y);
    cairo_show_text(cr, ticket->departure_city);

    // Draw arrival city
    cairo_move_to(cr, arrival_city_x, arrival_city_y);
    cairo_show_text(cr, ticket->arrival_city);

    // Draw departure and arrival airports
    double departure_airport_y = departure_city_y + 30;
    double arrival_airport_y = arrival_city_y + 30;

    cairo_set_source_rgb(cr, 0.4, 0.4, 0.4); // Gray color
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 18);

    cairo_move_to(cr, departure_city_x, departure_airport_y);
    cairo_show_text(cr, ticket->departure_airport);

    cairo_move_to(cr, arrival_city_x, arrival_airport_y);
    cairo_show_text(cr, ticket->arrival_airport);

    // Draw flight icon
    GdkPixbuf *flight_icon = gdk_pixbuf_new_from_file("../assets/images/flight-icon.png", NULL);
    if (flight_icon) {
        GdkPixbuf *scaled_flight_icon = gdk_pixbuf_scale_simple(flight_icon, flight_icon_width, 36, GDK_INTERP_BILINEAR);
        gdk_cairo_set_source_pixbuf(cr, scaled_flight_icon, flight_icon_x, flight_icon_y);
        cairo_paint(cr);
        g_object_unref(scaled_flight_icon);
        g_object_unref(flight_icon);
    }

    // Draw date and time
    double icon_size = 20;
    double date_time_start_x = ticket_x + ticket_left_margin + 70;
    double current_y = ticket_y + 130;

    GdkPixbuf *calendar_icon = gdk_pixbuf_new_from_file("../assets/images/Calendar.png", NULL);
    if (calendar_icon) {
        GdkPixbuf *scaled_calendar_icon = gdk_pixbuf_scale_simple(calendar_icon, icon_size, icon_size, GDK_INTERP_BILINEAR);
        gdk_cairo_set_source_pixbuf(cr, scaled_calendar_icon, date_time_start_x, current_y);
        cairo_paint(cr);
        g_object_unref(scaled_calendar_icon);
        g_object_unref(calendar_icon);
    }

    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Black text
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 18);
    cairo_move_to(cr, date_time_start_x + 30, current_y + 15);
    cairo_show_text(cr, ticket->date);

    GdkPixbuf *clock_icon = gdk_pixbuf_new_from_file("../assets/images/Clock.png", NULL);
    if (clock_icon) {
        GdkPixbuf *scaled_clock_icon = gdk_pixbuf_scale_simple(clock_icon, icon_size, icon_size, GDK_INTERP_BILINEAR);
        gdk_cairo_set_source_pixbuf(cr, scaled_clock_icon, date_time_start_x + 200, current_y);
        cairo_paint(cr);
        g_object_unref(scaled_clock_icon);
        g_object_unref(clock_icon);
    }

    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Black text
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 18);
    cairo_move_to(cr, date_time_start_x + 230, current_y + 15);
    cairo_show_text(cr, ticket->time);

    // Draw flight details
    const char *flight_labels[4] = {"Flight", "Gate", "Seat", "Class"};
    const char *flight_info[4] = {ticket->flight_code, ticket->gate_code, ticket->seat_code, ticket->class_type};

    double base_x = ticket_x + 70;
    current_y += 70;
    for (int i = 0; i < 4; i++) {
        cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
        cairo_set_font_size(cr, 16);
        cairo_move_to(cr, base_x + i * 150, current_y);
        cairo_show_text(cr, flight_labels[i]);

        cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
        cairo_set_font_size(cr, 18);
        cairo_move_to(cr, base_x + i * 150, current_y + 20);
        cairo_show_text(cr, flight_info[i]);
    }

    // Draw price
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Black
    cairo_set_font_size(cr, 22);
    cairo_move_to(cr, ticket_x + ticket_width - 350, ticket_y + 60);
    cairo_show_text(cr, "Price:");

    cairo_set_font_size(cr, 32);
    cairo_move_to(cr, ticket_x + ticket_width - 280, ticket_y + 60);
    cairo_show_text(cr, ticket->price);

    // Draw barcode
    double barcode_x = ticket_x + ticket_width - 350;
    double barcode_y = ticket_y + 80;
    double barcode_width = 320;
    double barcode_height = 50;
    int num_bars = 80;

    srand(12345); // Fixed seed for consistent barcode
    for (int i = 0; i < num_bars; i++) {
        double bar_x = barcode_x + i * (barcode_width / num_bars);
        double bar_width = (rand() % 2 == 0) ? (barcode_width / num_bars) : (barcode_width / (num_bars * 1.2));
        cairo_set_source_rgb(cr, 0, 0, 0); // Black
        cairo_rectangle(cr, bar_x, barcode_y, bar_width, barcode_height);
        cairo_fill(cr);
    }
}

// Draw ticket PDF
static void save_ticket_as_pdf(const char *filename, const Ticket *ticket) {
    double ticket_width = 1000, ticket_height = 280;

    cairo_surface_t *pdf_surface = cairo_pdf_surface_create(filename, ticket_width, ticket_height);
    cairo_t *pdf_cr = cairo_create(pdf_surface);

    draw_ticket(pdf_cr, 0, 0, ticket_width, ticket_height, ticket);

    cairo_destroy(pdf_cr);
    cairo_surface_destroy(pdf_surface);
}
static gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
    if (event->type == GDK_BUTTON_PRESS) {
        Ticket *tickets = (Ticket *)user_data;
        int ticket_count = NUM_TICKETS;

        gint screen_width = gtk_widget_get_allocated_width(widget);
        double ticket_width = 1000, ticket_height = 280;

        for (int i = 0; i < ticket_count; ++i) {
            double ticket_x = (screen_width - ticket_width) / 2;

            // Print Button
            double print_button_x = ticket_x + ticket_width - 350;
            double print_button_y = 200 + i * (ticket_height + 50);

            if (event->x >= print_button_x && event->x <= print_button_x + 320 &&
                event->y >= print_button_y && event->y <= print_button_y + 50) {

                // Open file chooser to save PDF
                GtkWidget *dialog = gtk_file_chooser_dialog_new(
                    "Save Ticket",
                    GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                    GTK_FILE_CHOOSER_ACTION_SAVE,
                    "_Cancel", GTK_RESPONSE_CANCEL,
                    "_Save", GTK_RESPONSE_ACCEPT,
                    NULL);

                gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "Your_Flight_Ticket.pdf");

                if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
                    char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
                    save_ticket_as_pdf(filename, &tickets[i]);
                    g_print("Ticket saved as: %s\n", filename);
                    g_free(filename);
                }

                gtk_widget_destroy(dialog);
                return TRUE;
            }

            // Cancel Button
            double cancel_button_x = ticket_x + ticket_width - 350;
            double cancel_button_y = print_button_y + 70;

            if (event->x >= cancel_button_x && event->x <= cancel_button_x + 150 &&
                event->y >= cancel_button_y && event->y <= cancel_button_y + 50) {
                ConfirmParams *params = g_new(ConfirmParams, 1); // Dùng g_new để đảm bảo cấp phát hợp lệ
                params->tickets = tickets;
                params->ticket_count = &ticket_count;
                params->index = i;

                show_confirmation_dialog(widget, "Are you sure you want to cancel?", (void (*)(void *))on_cancel_confirm, params);
                return TRUE;
            }

            // Change Button
            double change_button_x = ticket_x + ticket_width - 200;
            double change_button_y = cancel_button_y;

            if (event->x >= change_button_x && event->x <= change_button_x + 150 &&
                event->y >= change_button_y && event->y <= change_button_y + 50) {
                ConfirmParams *params = g_malloc(sizeof(ConfirmParams));
                params->tickets = tickets;
                params->ticket_count = &ticket_count;
                params->index = i;

                show_confirmation_dialog(widget, "Are you sure you want to change?", (void (*)(void *))on_change_confirm, params);
                return TRUE;
            }
        }
    }
    return FALSE;
}
static gboolean on_booklist_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    gint screen_width = gtk_widget_get_allocated_width(widget);
    gint screen_height = gtk_widget_get_allocated_height(widget);

    // Background
    GdkPixbuf *bg_pixbuf = gdk_pixbuf_new_from_file("../assets/images/bg_login.png", NULL);
    if (bg_pixbuf) {
        GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(bg_pixbuf, screen_width, screen_height, GDK_INTERP_BILINEAR);
        gdk_cairo_set_source_pixbuf(cr, scaled_pixbuf, 0, 0);
        cairo_paint(cr);
        g_object_unref(scaled_pixbuf);
        g_object_unref(bg_pixbuf);
    }

    Ticket *tickets = (Ticket *)user_data; // Correctly cast user_data to a Ticket array
    size_t ticket_count = NUM_TICKETS;

    double ticket_width = 1000;
    double ticket_height = 280;
    double ticket_x = (screen_width - ticket_width) / 2;
    double vertical_spacing = 50; // Space between tickets

    // Render tickets and buttons
    for (size_t i = 0; i < ticket_count; ++i) {
        double ticket_y = 50 + i * (ticket_height + vertical_spacing); // Adjust Y for each ticket

        // Draw the ticket
        draw_ticket(cr, ticket_x, ticket_y, ticket_width, ticket_height, &tickets[i]);

        // Barcode is already drawn in draw_ticket, adjust placement of buttons below barcode
        double button_width = 320;
        double button_height = 50;
        double button_gap = 20; // Gap between Cancel and Change buttons
        double cancel_change_width = (button_width - button_gap) / 2;

        double button_x = ticket_x + ticket_width - 350;

        // Print Button
        double print_button_y = ticket_y + 150; // Below the ticket
        cairo_set_source_rgb(cr, 34.0 / 255.0, 58.0 / 255.0, 96.0 / 255.0); // Button color
        cairo_new_path(cr);
        cairo_arc(cr, button_x + 8, print_button_y + 8, 8, M_PI, 3 * M_PI / 2);
        cairo_arc(cr, button_x + button_width - 8, print_button_y + 8, 8, 3 * M_PI / 2, 2 * M_PI);
        cairo_arc(cr, button_x + button_width - 8, print_button_y + button_height - 8, 8, 0, M_PI / 2);
        cairo_arc(cr, button_x + 8, print_button_y + button_height - 8, 8, M_PI / 2, M_PI);
        cairo_close_path(cr);
        cairo_fill_preserve(cr);
        cairo_set_source_rgb(cr, 0.13, 0.23, 0.37); // Border color
        cairo_stroke(cr);

        // Print Button Text
        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // White text
        cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 18);
        cairo_text_extents_t print_text_extents;
        cairo_text_extents(cr, "Print ticket out", &print_text_extents);
        cairo_move_to(cr, button_x + (button_width - print_text_extents.width) / 2,
                      print_button_y + (button_height + print_text_extents.height) / 2);
        cairo_show_text(cr, "Print ticket out");

        // Cancel Button
        double cancel_button_y = print_button_y + button_height + 20;
        double cancel_button_x = button_x;

        cairo_set_source_rgb(cr, 0.92, 0.94, 0.94); // Cancel button background
        cairo_new_path(cr);
        cairo_arc(cr, cancel_button_x + 8, cancel_button_y + 8, 8, M_PI, 3 * M_PI / 2);
        cairo_arc(cr, cancel_button_x + cancel_change_width - 8, cancel_button_y + 8, 8, 3 * M_PI / 2, 2 * M_PI);
        cairo_arc(cr, cancel_button_x + cancel_change_width - 8, cancel_button_y + button_height - 8, 8, 0, M_PI / 2);
        cairo_arc(cr, cancel_button_x + 8, cancel_button_y + button_height - 8, 8, M_PI / 2, M_PI);
        cairo_close_path(cr);
        cairo_fill_preserve(cr);
        cairo_set_source_rgb(cr, 0.13, 0.23, 0.37); // Border color
        cairo_stroke(cr);

        // Cancel Button Text
        cairo_set_source_rgb(cr, 0.13, 0.23, 0.37);
        cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 18);
        cairo_text_extents_t cancel_text_extents;
        cairo_text_extents(cr, "Cancel", &cancel_text_extents);
        cairo_move_to(cr, cancel_button_x + (cancel_change_width - cancel_text_extents.width) / 2,
                      cancel_button_y + (button_height + cancel_text_extents.height) / 2);
        cairo_show_text(cr, "Cancel");

        // Change Button
        double change_button_x = cancel_button_x + cancel_change_width + button_gap;

        cairo_set_source_rgb(cr, 0.92, 0.94, 0.94); // Change button background
        cairo_new_path(cr);
        cairo_arc(cr, change_button_x + 8, cancel_button_y + 8, 8, M_PI, 3 * M_PI / 2);
        cairo_arc(cr, change_button_x + cancel_change_width - 8, cancel_button_y + 8, 8, 3 * M_PI / 2, 2 * M_PI);
        cairo_arc(cr, change_button_x + cancel_change_width - 8, cancel_button_y + button_height - 8, 8, 0, M_PI / 2);
        cairo_arc(cr, change_button_x + 8, cancel_button_y + button_height - 8, 8, M_PI / 2, M_PI);
        cairo_close_path(cr);
        cairo_fill_preserve(cr);
        cairo_set_source_rgb(cr, 0.13, 0.23, 0.37); // Border color
        cairo_stroke(cr);

        // Change Button Text
        cairo_set_source_rgb(cr, 0.13, 0.23, 0.37);
        cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 18);
        cairo_text_extents_t change_text_extents;
        cairo_text_extents(cr, "Change", &change_text_extents);
        cairo_move_to(cr, change_button_x + (cancel_change_width - change_text_extents.width) / 2,
                      cancel_button_y + (button_height + change_text_extents.height) / 2);
        cairo_show_text(cr, "Change");
    }

    return FALSE;
}

GtkWidget *create_booklist_window() {
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_add_events(drawing_area, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_booklist_draw), tickets);
    g_signal_connect(drawing_area, "button-press-event", G_CALLBACK(on_button_press), tickets);

    gtk_widget_set_size_request(drawing_area, -1, 3 * (280 + 50 + 50)); // Adjust height for tickets and spacing
    gtk_container_add(GTK_CONTAINER(scrolled_window), drawing_area);

    return scrolled_window;
}
