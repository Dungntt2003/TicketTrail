#ifndef LIST_H
#define LIST_H

#include <gtk/gtk.h>

#define MAX_FLIGHTS 10
#define FLIGHTS_PER_PAGE 5

typedef struct {
    gchar airline[50];
    gchar departure_time[20];
    gchar arrival_time[20];
    gchar class[20];
    gdouble price;
} Flight;

// Khai báo các hàm
void create_flight_list_widget();
GtkWidget* create_ticket_list();
GtkWidget* create_filter_box();
void on_detail_link_click(GtkWidget *widget, gpointer data);
void sort_flights(gboolean ascending);  
void on_sort_changed(GtkComboBox *combo, gpointer user_data); 
void refresh_ticket_list(GtkWidget *container);
void create_flight_list_widget();

#endif