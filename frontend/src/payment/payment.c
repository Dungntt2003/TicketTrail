#include "payment.h"
#include <cairo.h>
#include <math.h>
#include "../global/global.h"
const char *departure_city = "HA NOI";
const char *departure_airport = "Noi Bai Airport";
const char *arrival_city = "HO CHI MINH";
const char *arrival_airport = "Tan Son Nhat Airport";
const char *selected_date = "08/12/2024";  
const char *selected_time = "9:30 PM";
const char *ticket_price = "$240";
const char *flight_code = "IN 230";
const char *gate_code= "22";
const char *seat_code= "2B, 3B";
const char *class_number= "Economy";


static gboolean on_payment_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    GdkPixbuf *bg_pixbuf;
    GdkPixbuf *scaled_pixbuf;

    gint screen_width = gtk_widget_get_allocated_width(widget);
    gint screen_height = gtk_widget_get_allocated_height(widget);

    
    bg_pixbuf = gdk_pixbuf_new_from_file("../assets/images/bg_login.png", NULL);
    if (!bg_pixbuf) {
        g_print("Error loading image!\n");
        return FALSE;
    }

    scaled_pixbuf = gdk_pixbuf_scale_simple(bg_pixbuf, screen_width, screen_height, GDK_INTERP_BILINEAR);
    gdk_cairo_set_source_pixbuf(cr, scaled_pixbuf, 0, 0);
    cairo_paint(cr);
    g_object_unref(scaled_pixbuf);
    g_object_unref(bg_pixbuf);

    
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_new_path(cr);
    cairo_arc(cr, screen_width / 2 - 500 + 24, screen_height / 2 - 300 + 24, 24, M_PI, 3 * M_PI / 2);
    cairo_arc(cr, screen_width / 2 + 500 - 24, screen_height / 2 - 300 + 24, 24, 3 * M_PI / 2, 2 * M_PI);
    cairo_arc(cr, screen_width / 2 + 500 - 24, screen_height / 2 + 300 - 24, 24, 0, M_PI / 2);
    cairo_arc(cr, screen_width / 2 - 500 + 24, screen_height / 2 + 300 - 24, 24, M_PI / 2, M_PI);
    cairo_close_path(cr);
    cairo_fill_preserve(cr);

    
    cairo_set_source_rgb(cr, 34.0 / 255.0, 58.0 / 255.0, 96.0 / 255.0); 
    cairo_new_path(cr);

    
    cairo_arc(cr, screen_width / 2 - 500 + 24, screen_height / 2 - 300 + 24, 24, M_PI, 3 * M_PI / 2);

    
    cairo_line_to(cr, screen_width / 2, screen_height / 2 - 300);

    
    cairo_line_to(cr, screen_width / 2, screen_height / 2 + 300);

    
    cairo_arc(cr, screen_width / 2 - 500 + 24, screen_height / 2 + 300 - 24, 24, M_PI / 2, M_PI);

    
    cairo_line_to(cr, screen_width / 2 - 500, screen_height / 2 - 300 + 24);

    cairo_close_path(cr); 
    cairo_fill(cr); 


    
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); 
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 30);
    cairo_text_extents_t vouchers_label_extents;
    cairo_text_extents(cr, "MY VOUCHERS", &vouchers_label_extents);
    cairo_move_to(cr, screen_width / 2 - 500 + (500 - vouchers_label_extents.width) / 2, screen_height / 2 - 300 + 61);
    cairo_show_text(cr, "MY VOUCHERS");

    
    double voucher_x = (screen_width - 1000) / 2 + (500 - 400) / 2;
    double voucher_y = screen_height / 2 - 300 + 100; 
    for (int i = 0; i < 4; i++) {
        
        cairo_set_source_rgb(cr, 0.98, 0.98, 0.98); 
        cairo_new_path(cr);

        
        cairo_arc(cr, voucher_x + 8, voucher_y + 8, 8, M_PI, 3 * M_PI / 2);

        
        cairo_line_to(cr, voucher_x + 400 - 8, voucher_y);

        
        cairo_arc(cr, voucher_x + 400 - 8, voucher_y + 8, 8, 3 * M_PI / 2, 2 * M_PI);

        
        cairo_line_to(cr, voucher_x + 400, voucher_y + 104 - 8);

        
        cairo_arc(cr, voucher_x + 400 - 8, voucher_y + 104 - 8, 8, 0, M_PI / 2);

        
        cairo_line_to(cr, voucher_x + 8, voucher_y + 104);

        
        cairo_arc(cr, voucher_x + 8, voucher_y + 104 - 8, 8, M_PI / 2, M_PI);

        cairo_close_path(cr); 

        
        cairo_fill_preserve(cr);

        
        cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.25); 
        cairo_stroke(cr);


        
        cairo_set_dash(cr, (double[]){6.0, 4.0}, 2, 0.0);
        cairo_move_to(cr, voucher_x + 124, voucher_y + 8.55);
        cairo_line_to(cr, voucher_x + 124, voucher_y + 95.45);
        cairo_set_source_rgba(cr, 0.4, 0.4, 0.4, 0.2); 
        cairo_stroke(cr);
        cairo_set_dash(cr, NULL, 0, 0);

        
        char voucher_image[256];
        snprintf(voucher_image, sizeof(voucher_image), "../assets/images/voucher%d.png", i + 1);
        GdkPixbuf *voucher_icon = gdk_pixbuf_new_from_file(voucher_image, NULL);
        if (voucher_icon) {
            GdkPixbuf *scaled_voucher_icon = gdk_pixbuf_scale_simple(voucher_icon, 55, 55, GDK_INTERP_BILINEAR);
            gdk_cairo_set_source_pixbuf(cr, scaled_voucher_icon, voucher_x + 34.5, voucher_y + 24.5);
            cairo_paint(cr);
            g_object_unref(scaled_voucher_icon);
            g_object_unref(voucher_icon);
        }

        
        const char *voucher_texts[4][3] = {
            {"200.000 VND", "Welcome Voucher", "Valid until 01 February 2025"},
            {"25% OFF", "VNPAY Payment Voucher", "Valid until 03 March 2025"},
            {"20% OFF", "VNPAY Payment Voucher", "Valid until 05 March 2025"},
            {"10% OFF", "Over 1 Million Discount", "Valid until 09 March 2025"}};

        
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); 
        cairo_select_font_face(cr, "Montserrat", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 24);
        cairo_move_to(cr, voucher_x + 140, voucher_y + 35);
        cairo_show_text(cr, voucher_texts[i][0]);

        
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); 
        cairo_select_font_face(cr, "Montserrat", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 16);
        cairo_move_to(cr, voucher_x + 140, voucher_y + 65);
        cairo_show_text(cr, voucher_texts[i][1]);

        
        cairo_set_source_rgb(cr, 0.33, 0.33, 0.33); 
        cairo_select_font_face(cr, "Montserrat", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 10);
        cairo_move_to(cr, voucher_x + 140, voucher_y + 90);
        cairo_show_text(cr, voucher_texts[i][2]);

        voucher_y += 120; 
    }

    
    GdkPixbuf *logo_pixbuf = gdk_pixbuf_new_from_file("../assets/images/logovn.png", NULL);
    if (logo_pixbuf) {
        GdkPixbuf *scaled_logo_pixbuf = gdk_pixbuf_scale_simple(logo_pixbuf, 120, 120, GDK_INTERP_BILINEAR);
        gdk_cairo_set_source_pixbuf(cr, scaled_logo_pixbuf, screen_width / 2 + 250 - 60, screen_height / 2 - 300 + 20);
        cairo_paint(cr);
        g_object_unref(scaled_logo_pixbuf);
        g_object_unref(logo_pixbuf);
    }

    
const char *flight_labels[4] = {"Flight", "Gate", "Seat", "Class"};
const char *flight_info[4] = {flight_code, gate_code, seat_code, class_number};


int label_font_size = 18;
int info_font_size = 20;


double column_spacing = 100;


double right_half_start_x = screen_width / 2; 
double right_half_width = 500; 
double content_width = 3 * column_spacing; 
double base_x = right_half_start_x + (right_half_width - content_width) / 2;


double base_y = screen_height / 2 - 300 + 350; 
double row_spacing = 30; 


for (int i = 0; i < 4; i++) {
    
    double column_x = base_x + i * column_spacing;

    
    cairo_text_extents_t label_extents, info_extents;

    
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, label_font_size);
    cairo_text_extents(cr, flight_labels[i], &label_extents);

    
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, info_font_size);
    cairo_text_extents(cr, flight_info[i], &info_extents);

    
    double label_y = base_y;
    double info_y = base_y + row_spacing + (label_extents.height - info_extents.height) / 2;

    
    cairo_set_source_rgb(cr, 0.33, 0.33, 0.33); 
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, label_font_size);
    cairo_move_to(cr, column_x - label_extents.width / 2, label_y);
    cairo_show_text(cr, flight_labels[i]);

    
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); 
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, info_font_size);
    cairo_move_to(cr, column_x - info_extents.width / 2, info_y);
    cairo_show_text(cr, flight_info[i]);
}



double right_half_center_x = screen_width / 2 + 500 / 2; 
double flight_icon_y = screen_height / 2 - 300 + 180;    



cairo_text_extents_t departure_city_extents, arrival_city_extents;
cairo_text_extents(cr, departure_city, &departure_city_extents);
cairo_text_extents(cr, arrival_city, &arrival_city_extents);


double flight_icon_x = right_half_center_x - 156 / 2; 
double city_spacing = 20; 


double departure_city_x = flight_icon_x - city_spacing - departure_city_extents.width;
double departure_city_y = flight_icon_y;


double arrival_city_x = flight_icon_x + 156 + city_spacing;
double arrival_city_y = flight_icon_y;


cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); 
cairo_select_font_face(cr, "Poppins", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
cairo_set_font_size(cr, 20);
cairo_move_to(cr, departure_city_x, departure_city_y);
if (departure_city) cairo_show_text(cr, departure_city);


cairo_move_to(cr, arrival_city_x, arrival_city_y);
if (arrival_city) cairo_show_text(cr, arrival_city);


cairo_text_extents_t departure_airport_extents, arrival_airport_extents;
cairo_text_extents(cr, departure_airport, &departure_airport_extents);
cairo_text_extents(cr, arrival_airport, &arrival_airport_extents);

double departure_airport_y = departure_city_y + (departure_city_extents.height + 20); 
double arrival_airport_y = arrival_city_y + (arrival_city_extents.height + 20); 


cairo_set_source_rgb(cr, 0.4, 0.4, 0.4); 
cairo_select_font_face(cr, "Poppins", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
cairo_set_font_size(cr, 18);
cairo_move_to(cr, departure_city_x + departure_city_extents.width / 2 - departure_airport_extents.width / 2, departure_airport_y);
if (departure_airport) cairo_show_text(cr, departure_airport);


cairo_move_to(cr, arrival_city_x + arrival_city_extents.width / 2 - arrival_airport_extents.width / 2, arrival_airport_y);
if (arrival_airport) cairo_show_text(cr, arrival_airport);


GdkPixbuf *flight_icon = gdk_pixbuf_new_from_file("../assets/images/flight-icon.png", NULL);
if (flight_icon) {
    GdkPixbuf *scaled_flight_icon = gdk_pixbuf_scale_simple(flight_icon, 156, 36, GDK_INTERP_BILINEAR);
    gdk_cairo_set_source_pixbuf(cr, scaled_flight_icon, flight_icon_x, flight_icon_y - 18);
    cairo_paint(cr);
    g_object_unref(scaled_flight_icon);
    g_object_unref(flight_icon);
}



double date_x = screen_width / 2 + (500 - 400) / 2 + 50;


GdkPixbuf *calendar_icon = gdk_pixbuf_new_from_file("../assets/images/Calendar.png", NULL);
if (calendar_icon) {
    GdkPixbuf *scaled_calendar_icon = gdk_pixbuf_scale_simple(calendar_icon, 20, 20, GDK_INTERP_BILINEAR);
    gdk_cairo_set_source_pixbuf(cr, scaled_calendar_icon, date_x + 8, screen_height / 2 - 300 + 257 + (46 - 20) / 2);
    cairo_paint(cr);
    g_object_unref(scaled_calendar_icon);
    g_object_unref(calendar_icon);
}


cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
cairo_set_font_size(cr, 18);
cairo_text_extents_t date_text_extents;
cairo_text_extents(cr, selected_date, &date_text_extents);
double date_text_y = screen_height / 2 - 300 + 270 + (46 - date_text_extents.height) / 2;
cairo_move_to(cr, date_x + 20 + 8 + 10, date_text_y);
cairo_show_text(cr, selected_date);


double time_x = screen_width / 2 + (500 - 400) / 2 + 230;


GdkPixbuf *clock_icon = gdk_pixbuf_new_from_file("../assets/images/Clock.png", NULL);
if (clock_icon) {
    GdkPixbuf *scaled_clock_icon = gdk_pixbuf_scale_simple(clock_icon, 20, 20, GDK_INTERP_BILINEAR);
    gdk_cairo_set_source_pixbuf(cr, scaled_clock_icon, time_x + 8, screen_height / 2 - 300 + 257 + (46 - 20) / 2);
    cairo_paint(cr);
    g_object_unref(scaled_clock_icon);
    g_object_unref(clock_icon);
}


cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
cairo_set_font_size(cr, 18);
cairo_text_extents_t time_text_extents;
cairo_text_extents(cr, selected_time, &time_text_extents);
double time_text_y = screen_height / 2 - 300 + 270 + (46 - time_text_extents.height) / 2;
cairo_move_to(cr, time_x + 20 + 8 + 10, time_text_y);
cairo_show_text(cr, selected_time);


    
    
const char *price_label = "Price";
const char *price_value = ticket_price;


cairo_text_extents_t price_label_extents, price_value_extents;
cairo_set_font_size(cr, 18);
cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
cairo_text_extents(cr, price_label, &price_label_extents);

cairo_set_font_size(cr, 32);
cairo_text_extents(cr, price_value, &price_value_extents);

double total_width = price_label_extents.width + 10 + price_value_extents.width; 


double base1_x = screen_width / 2 + 500 - (500 / 2) - (total_width / 2);


double base1_y = base_y + 100; 


cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); 
cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
cairo_set_font_size(cr, 22);
cairo_move_to(cr, base1_x, base1_y);
cairo_show_text(cr, price_label);


cairo_set_font_size(cr, 32); 
cairo_move_to(cr, base1_x + price_label_extents.width + 20, base1_y); 
cairo_show_text(cr, price_value);


    
    double button_y = screen_height / 2 + 300 - 80;
    double cancel_button_x = screen_width / 2 + 250 - 180 - 10;
    double confirm_button_x = screen_width / 2 + 250 + 10;

    
    cairo_set_source_rgb(cr, 0.92, 0.94, 0.94); 
    cairo_new_path(cr);
    cairo_arc(cr, cancel_button_x + 8, button_y + 8, 8, M_PI, 3 * M_PI / 2); 
    cairo_arc(cr, cancel_button_x + 156 - 8, button_y + 8, 8, 3 * M_PI / 2, 2 * M_PI); 
    cairo_arc(cr, cancel_button_x + 156 - 8, button_y + 56 - 8, 8, 0, M_PI / 2); 
    cairo_arc(cr, cancel_button_x + 8, button_y + 56 - 8, 8, M_PI / 2, M_PI); 
    cairo_close_path(cr);
    cairo_fill_preserve(cr);
    cairo_set_source_rgb(cr, 0.13, 0.23, 0.37); 
    cairo_stroke(cr);

    
    cairo_set_source_rgb(cr, 0.13, 0.23, 0.37); 
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 18);
    cairo_text_extents_t cancel_extents;
    cairo_text_extents(cr, "Cancel", &cancel_extents);
    cairo_move_to(
        cr,
        cancel_button_x + 156 / 2 - cancel_extents.width / 2,
        button_y + 56 / 2 + cancel_extents.height / 2
    );
    cairo_show_text(cr, "Cancel");

    
    cairo_set_source_rgb(cr, 0.13, 0.23, 0.37); 
    cairo_new_path(cr);
    cairo_arc(cr, confirm_button_x + 8, button_y + 8, 8, M_PI, 3 * M_PI / 2); 
    cairo_arc(cr, confirm_button_x + 156 - 8, button_y + 8, 8, 3 * M_PI / 2, 2 * M_PI); 
    cairo_arc(cr, confirm_button_x + 156 - 8, button_y + 56 - 8, 8, 0, M_PI / 2); 
    cairo_arc(cr, confirm_button_x + 8, button_y + 56 - 8, 8, M_PI / 2, M_PI); 
    cairo_close_path(cr);
    cairo_fill_preserve(cr);
    cairo_stroke(cr);

    
    cairo_set_source_rgb(cr, 0.92, 0.94, 0.94); 
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 18);
    cairo_text_extents_t confirm_extents;
    cairo_text_extents(cr, "Confirm", &confirm_extents);
    cairo_move_to(
        cr,
        confirm_button_x + 156 / 2 - confirm_extents.width / 2,
        button_y + 56 / 2 + confirm_extents.height / 2
    );
    cairo_show_text(cr, "Confirm");

    return FALSE;
}


GtkWidget* create_payment_window() {
    GtkWidget *window, *drawing_area;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Payment");
    gtk_window_fullscreen(GTK_WINDOW(window));
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    drawing_area = gtk_drawing_area_new();
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_payment_draw), NULL);
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    return window;
}
