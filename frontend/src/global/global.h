#ifndef GLOBAL_H
#define GLOBAL_H
#include <gtk/gtk.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>
#include <stdbool.h>
#define MAX_LENGTH 4096
#define M_PI 3.14159265358979323846

typedef struct {
    char flight_id[MAX_LENGTH];
    char departure_airport[MAX_LENGTH];
    char arrival_airport[MAX_LENGTH];
    char departure_time[MAX_LENGTH];
    int duration_minutes;
    char airplane_name[MAX_LENGTH];
    int price;
    int capacity;
    int available_economy;
    int available_business;
    int available_first_class;
} Flight;

extern GtkWidget *entry_email, *entry_password, *label_status;
extern char buffer[MAX_LENGTH];
extern int sock;
extern GtkWidget *window;
extern GtkWidget *content_area;
extern const char *airports[];
extern const int airport_count;
extern  Flight flights[MAX_LENGTH];
extern  int flight_count;
extern Flight tem_flights[MAX_LENGTH];
extern int tem_flight_count;
extern Flight detail_flight;
GtkWidget* create_main_window();
void set_content(GtkWidget *new_content);
int is_number(const char *str);
char* format_number_with_separator(int number, char separator);
char* extract_middle_string(const char *input);
void split_date_time(const char *input, char *date, char *time);
bool checkHaveSeat(int available_economy, int available_business, int available_first,const char *type, int number_seat);
void filter_flights(const Flight *flights, int flight_count, Flight *temp_flight, int *temp_count_flight, const char *departure, const char *arrival, const char *date,const char *class,const int number_seat);

#endif 