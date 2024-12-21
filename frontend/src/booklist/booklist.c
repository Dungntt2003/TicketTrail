#include "booklist.h"
#include <cairo.h>
#include <cairo-pdf.h>
#include <gtk/gtk.h>
#include <math.h>
#include "../global/global.h"


const char *departure_city = "HA NOI";
const char *departure_airport = "Noi Bai Airport";
const char *arrival_city = "HO CHI MINH";
const char *arrival_airport = "Tan Son Nhat Airport";
const char *selected_date = "08/12/2024";
const char *selected_time = "9:30 PM";
const char *flight_code = "IN 230";
const char *gate_code_test = "22";
const char *seat_code = "2B, 3B";
const char *class_number = "Economy";
const char *ticket_price = "240000 VND";


static void draw_ticket(cairo_t *cr, double ticket_x, double ticket_y, double ticket_width, double ticket_height) {
    cairo_set_source_rgb(cr, 0.92, 0.94, 0.94); 
    cairo_new_path(cr);
    cairo_arc(cr, ticket_x + 24, ticket_y + 24, 24, M_PI, 3 * M_PI / 2);
    cairo_arc(cr, ticket_x + ticket_width - 24, ticket_y + 24, 24, 3 * M_PI / 2, 2 * M_PI);
    cairo_arc(cr, ticket_x + ticket_width - 24, ticket_y + ticket_height - 24, 24, 0, M_PI / 2);
    cairo_arc(cr, ticket_x + 24, ticket_y + ticket_height - 24, 24, M_PI / 2, M_PI);
    cairo_close_path(cr);
    cairo_fill(cr);


cairo_text_extents_t departure_city_extents, arrival_city_extents, departure_airport_extents, arrival_airport_extents;
cairo_text_extents(cr, departure_city, &departure_city_extents);
cairo_text_extents(cr, arrival_city, &arrival_city_extents);
cairo_text_extents(cr, departure_airport, &departure_airport_extents);
cairo_text_extents(cr, arrival_airport, &arrival_airport_extents);


double ticket_left_margin = 50; 
double flight_icon_width = 156; 
double flight_icon_x = ticket_x + ticket_left_margin + departure_airport_extents.width + 70; 
double flight_icon_y = ticket_y + 50; 


double departure_city_x = ticket_x + ticket_left_margin;
double departure_city_y = flight_icon_y + 10; 


double arrival_city_x = flight_icon_x + flight_icon_width + 20; 
double arrival_city_y = departure_city_y;


cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); 
cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
cairo_set_font_size(cr, 20);
cairo_move_to(cr, departure_city_x, departure_city_y);
if (departure_city) cairo_show_text(cr, departure_city);


cairo_move_to(cr, arrival_city_x, arrival_city_y);
if (arrival_city) cairo_show_text(cr, arrival_city);


double departure_airport_y = departure_city_y + departure_city_extents.height + 20; 
double arrival_airport_y = arrival_city_y + arrival_city_extents.height + 20; 


cairo_set_source_rgb(cr, 0.4, 0.4, 0.4); 
cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
cairo_set_font_size(cr, 18);
cairo_move_to(cr, departure_city_x, departure_airport_y);
if (departure_airport) cairo_show_text(cr, departure_airport);


cairo_move_to(cr, arrival_city_x, arrival_airport_y);
if (arrival_airport) cairo_show_text(cr, arrival_airport);


GdkPixbuf *flight_icon = gdk_pixbuf_new_from_file("../assets/images/flight-icon.png", NULL);
if (flight_icon) {
    GdkPixbuf *scaled_flight_icon = gdk_pixbuf_scale_simple(flight_icon, flight_icon_width, 36, GDK_INTERP_BILINEAR);
    gdk_cairo_set_source_pixbuf(cr, scaled_flight_icon, flight_icon_x, flight_icon_y);
    cairo_paint(cr);
    g_object_unref(scaled_flight_icon);
    g_object_unref(flight_icon);
}


double flight_icon_height = 36; 
double icon_size = 20; 
double column_spacing = 120; 
double row_spacing = 30; 


double center_y = flight_icon_y + flight_icon_height / 2; 


double date_time_start_x = ticket_x + ticket_left_margin + 70; 
double current_y = center_y - (icon_size / 2) + 70; 


GdkPixbuf *calendar_icon = gdk_pixbuf_new_from_file("../assets/images/Calendar.png", NULL);
if (calendar_icon) {
    GdkPixbuf *scaled_calendar_icon = gdk_pixbuf_scale_simple(calendar_icon, icon_size, icon_size, GDK_INTERP_BILINEAR);
    gdk_cairo_set_source_pixbuf(cr, scaled_calendar_icon, date_time_start_x, current_y);
    cairo_paint(cr);
    g_object_unref(scaled_calendar_icon);
    g_object_unref(calendar_icon);
}


cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); 
cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
cairo_set_font_size(cr, 18);
cairo_text_extents_t date_text_extents;
cairo_text_extents(cr, selected_date, &date_text_extents);
double date_text_x = date_time_start_x + icon_size + 10; 
cairo_move_to(cr, date_text_x, current_y + icon_size / 2 + 6);
cairo_show_text(cr, selected_date);


double time_start_x = date_text_x + date_text_extents.width + 50; 
GdkPixbuf *clock_icon = gdk_pixbuf_new_from_file("../assets/images/Clock.png", NULL);
if (clock_icon) {
    GdkPixbuf *scaled_clock_icon = gdk_pixbuf_scale_simple(clock_icon, icon_size, icon_size, GDK_INTERP_BILINEAR);
    gdk_cairo_set_source_pixbuf(cr, scaled_clock_icon, time_start_x, current_y);
    cairo_paint(cr);
    g_object_unref(scaled_clock_icon);
    g_object_unref(clock_icon);
}


cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); 
cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
cairo_set_font_size(cr, 18);
cairo_text_extents_t time_text_extents;
cairo_text_extents(cr, selected_time, &time_text_extents);
double time_text_x = time_start_x + icon_size + 10; 
cairo_move_to(cr, time_text_x, current_y + icon_size / 2 + 6);
cairo_show_text(cr, selected_time);


current_y += 80;


const char *flight_labels[4] = {"Flight", "Gate", "Seat", "Class"};
const char *flight_info[4] = {flight_code, gate_code_test, seat_code, class_number};


double content_width = 3 * column_spacing; 
double base_x = flight_icon_x - content_width / 2 + flight_icon_width / 2; 


for (int i = 0; i < 4; i++) {
    double column_x = base_x + i * column_spacing;

    
    cairo_text_extents_t label_extents, info_extents;
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 18);
    cairo_text_extents(cr, flight_labels[i], &label_extents);

    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 18);
    cairo_text_extents(cr, flight_info[i], &info_extents);

    
    cairo_set_source_rgb(cr, 0.33, 0.33, 0.33); 
    cairo_move_to(cr, column_x - label_extents.width / 2, current_y);
    cairo_show_text(cr, flight_labels[i]);

    
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); 
    cairo_move_to(cr, column_x - info_extents.width / 2, current_y + row_spacing);
    cairo_show_text(cr, flight_info[i]);
}

const char *price_label = "Price";
const char *price_value = ticket_price;


cairo_text_extents_t price_label_extents, price_value_extents;
cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
cairo_set_font_size(cr, 22);
cairo_text_extents(cr, price_label, &price_label_extents);

cairo_set_font_size(cr, 32);
cairo_text_extents(cr, price_value, &price_value_extents);


double price_y = arrival_city_y; 


double price_x = ticket_x + ticket_width - 400 + 50; 


cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); 
cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
cairo_set_font_size(cr, 22);
cairo_move_to(cr, price_x, price_y);
cairo_show_text(cr, price_label);


double ticket_price_x = price_x + price_label_extents.width + 10; 
cairo_set_font_size(cr, 32);
cairo_move_to(cr, ticket_price_x, price_y);
cairo_show_text(cr, price_value);
                
                double barcode_x = ticket_x + ticket_width - 400; 
                double barcode_y = 190; 
                double barcode_width = 320; 
                double barcode_height = 50; 
                int num_bars = 60; 

                

                srand(time(NULL)); 
                for (int i = 0; i < num_bars; i++) {
                    double bar_x = barcode_x + i * (barcode_width / num_bars);
                    double bar_width = (rand() % 2 == 0) ? (barcode_width / num_bars) : (barcode_width / (num_bars * 1.5));
                    cairo_set_source_rgb(cr, 0, 0, 0); 
                    cairo_rectangle(cr, bar_x, barcode_y, bar_width, barcode_height);
                    cairo_fill(cr);
                }
}


static gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
    if (event->type == GDK_BUTTON_PRESS) {
        
        double button_gap = 20; 
        double button_width = 150; 
        double print_button_width = 2 * button_width + button_gap; 
        double print_button_height = 50; 
        gint screen_width = gtk_widget_get_allocated_width(widget);
        double ticket_width = 1000; 
        double ticket_height = 280; 
        double ticket_x = (screen_width - ticket_width) / 2; 
        double print_button_x = ticket_x + ticket_width - 400; 
        double print_button_y = 330; 

        
        if (event->x >= print_button_x && event->x <= print_button_x + print_button_width &&
            event->y >= print_button_y && event->y <= print_button_y + print_button_height) {

GtkWidget *dialog = gtk_file_chooser_dialog_new(
    "Save Ticket",
    NULL, 
    GTK_FILE_CHOOSER_ACTION_SAVE,
    "_Cancel", GTK_RESPONSE_CANCEL,
    "_Save", GTK_RESPONSE_ACCEPT,
    NULL
);


gtk_window_set_default_size(GTK_WINDOW(dialog), 1000, 800);
gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);


            
            gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "Your_Flight_Ticket.pdf");

            
            if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
                char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

                
                cairo_surface_t *pdf_surface = cairo_pdf_surface_create(filename, ticket_width, ticket_height);
                cairo_t *pdf_cr = cairo_create(pdf_surface);

                
                draw_ticket(pdf_cr, 0, 0, ticket_width, ticket_height);

                
                cairo_destroy(pdf_cr);
                cairo_surface_destroy(pdf_surface);

                
                g_print("Ticket saved as '%s'.\n", filename);

                
                g_free(filename);
            }

            gtk_widget_destroy(dialog);

            return TRUE; 
        }
    }
    return FALSE; 
}

static gboolean on_booklist_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    gint screen_width = gtk_widget_get_allocated_width(widget);
    gint screen_height = gtk_widget_get_allocated_height(widget);

    
    GdkPixbuf *bg_pixbuf = gdk_pixbuf_new_from_file("../assets/images/bg_login.png", NULL);
    if (bg_pixbuf) {
        GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(bg_pixbuf, screen_width, screen_height, GDK_INTERP_BILINEAR);
        gdk_cairo_set_source_pixbuf(cr, scaled_pixbuf, 0, 0);
        cairo_paint(cr);
        g_object_unref(scaled_pixbuf);
        g_object_unref(bg_pixbuf);
    }

    
    double ticket_x = (screen_width - 1000) / 2;
    double ticket_y = 111;
    double ticket_width = 1000;
    double ticket_height = 280;

    draw_ticket(cr, ticket_x, ticket_y, ticket_width, ticket_height);

    
    double button_width = 320;
    double button_height = 50;
    double button_x = ticket_x + ticket_width - 400;
    double print_button_y = 330;

    
    cairo_set_source_rgb(cr, 34.0 / 255.0, 58.0 / 255.0, 96.0 / 255.0); 
    cairo_new_path(cr);
    cairo_arc(cr, button_x + 8, print_button_y + 8, 8, M_PI, 3 * M_PI / 2);
    cairo_arc(cr, button_x + button_width - 8, print_button_y + 8, 8, 3 * M_PI / 2, 2 * M_PI);
    cairo_arc(cr, button_x + button_width - 8, print_button_y+ button_height - 8, 8, 0, M_PI / 2);
    cairo_arc(cr, button_x + 8, print_button_y + button_height - 8, 8, M_PI / 2, M_PI);
    cairo_close_path(cr);
    cairo_fill_preserve(cr);
    cairo_set_source_rgb(cr, 0.13, 0.23, 0.37); 
    cairo_stroke(cr);

    
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); 
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 18);
    cairo_text_extents_t text_extents;
    cairo_text_extents(cr, "Print ticket out", &text_extents);
    cairo_move_to(cr, button_x + (button_width - text_extents.width) / 2, print_button_y + (button_height + text_extents.height) / 2);
    cairo_show_text(cr, "Print ticket out");
            
    double cancel_x = ticket_x + ticket_width - 400;
    double button_y = 260;
    cairo_set_source_rgb(cr, 0.92, 0.94, 0.94); 
    cairo_new_path(cr);
    cairo_arc(cr, cancel_x + 8, button_y + 8, 8, M_PI, 3 * M_PI / 2);
    cairo_arc(cr, cancel_x + 150 - 8, button_y + 8, 8, 3 * M_PI / 2, 2 * M_PI);
    cairo_arc(cr, cancel_x + 150 - 8, button_y + button_height - 8, 8, 0, M_PI / 2);
    cairo_arc(cr, cancel_x + 8, button_y + button_height - 8, 8, M_PI / 2, M_PI);
    cairo_close_path(cr);
    cairo_fill_preserve(cr);
    cairo_set_source_rgb(cr, 0.13, 0.23, 0.37); 
    cairo_stroke(cr);
    
    double change_x = cancel_x + 150 + 20;
    cairo_set_source_rgb(cr, 0.92, 0.94, 0.94); 
    cairo_new_path(cr);
    cairo_arc(cr, change_x + 8, button_y + 8, 8, M_PI, 3 * M_PI / 2);
    cairo_arc(cr, change_x + 150 - 8, button_y + 8, 8, 3 * M_PI / 2, 2 * M_PI);
    cairo_arc(cr, change_x + 150 - 8, button_y + button_height - 8, 8, 0, M_PI / 2);
    cairo_arc(cr, change_x + 8, button_y + button_height - 8, 8, M_PI / 2, M_PI);
    cairo_close_path(cr);
    cairo_fill_preserve(cr);
    cairo_set_source_rgb(cr, 0.13, 0.23, 0.37); 
    cairo_stroke(cr);

    
    cairo_set_source_rgb(cr, 0.13, 0.23, 0.37);
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 18);
    cairo_text_extents_t cancel_extents;
    cairo_text_extents(cr, "Cancel", &cancel_extents);
    cairo_move_to(cr, cancel_x + 150 / 2 - cancel_extents.width / 2, button_y + button_height / 2 + cancel_extents.height / 2);
    cairo_show_text(cr, "Cancel");


    
    cairo_set_source_rgb(cr, 0.13, 0.23, 0.37);
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 18);
    cairo_text_extents_t change_extents;
    cairo_text_extents(cr, "Change", &change_extents);
    cairo_move_to(cr, change_x + 150 / 2 - change_extents.width / 2, button_y + button_height / 2 + change_extents.height / 2);
    cairo_show_text(cr, "Change");
    return FALSE;
}


GtkWidget *create_booklist_window() {
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_add_events(drawing_area, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_booklist_draw), NULL);
    g_signal_connect(drawing_area, "button-press-event", G_CALLBACK(on_button_press), NULL);
    return drawing_area;
}
