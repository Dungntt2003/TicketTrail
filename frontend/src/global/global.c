#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
GtkWidget *entry_email, *entry_password, *label_status;
char buffer[MAX_LENGTH];
int sock;
GtkWidget *window;
GtkWidget *content_area;
const int airport_count;
Flight flights[MAX_LENGTH];
int flight_count;
Flight tem_flights[MAX_LENGTH];
int tem_flight_count;
Flight detail_flight;
int seat_count;
char **seats_array;
const char *airports[] = {
       "SGN - Tân Sơn Nhất - Hồ Chí Minh",
        "HAN - Nội Bài - Hà Nội",
        "CXR - Cam Ranh - Khánh Hòa",
        "DAD - Đà Nẵng - Đà Nẵng",
        "HUI - Phú Bài - Thừa Thiên Huế",
        "UIH - Phù Cát - Bình Định",
        "VCL - Chu Lai - Quảng Nam",
        "VCS - Côn Đảo - Bà Rịa-Vũng Tàu",
        "THD - Trường Sa - Khánh Hòa",
        "DLI - Liên Khương - Lâm Đồng",
        "VKG - Quy Nhơn - Bình Định",
        "PQC - Phú Quốc - Kiên Giang",
        "VCA - Cần Thơ - Cần Thơ",
        "CAH - Cát Bi - Hải Phòng",
        "CJJ - Chu Lai - Quảng Nam"
    };

const int airport_count = sizeof(airports) / sizeof(airports[0]);


GtkWidget* create_main_window() {
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Register");

    
    gtk_window_fullscreen(GTK_WINDOW(window));

    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    
    GtkCssProvider *provider = gtk_css_provider_new();
    if (!gtk_css_provider_load_from_data(provider,
        "* { background-image: url('../assets/images/bg_login.png'); background-size: cover; background-position: center; }"
        "#register-box { background-color: #FFFFFF; border-radius: 20px; padding: 48px 72px; }"
        "#register-title { font-family: Arial, sans-serif; font-size: 28px; font-weight: bold; color: #101828; }",
        -1, NULL)) {
        g_print("Failed to load CSS\n");
    }

    
    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    
    content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(main_box), content_area, TRUE, TRUE, 0);

    return window;
}


void set_content(GtkWidget *new_content) {
    
    gtk_container_forall(GTK_CONTAINER(content_area), (GtkCallback)gtk_widget_destroy, NULL);

    
    gtk_box_pack_start(GTK_BOX(content_area), new_content, TRUE, TRUE, 0);

    
    gtk_widget_show_all(window);
}

int is_number(const char *str) {
    char *endptr;
    strtol(str, &endptr, 10);  
    return *endptr == '\0';  
}


char* format_number_with_separator(int number, char separator) {
    
    char temp[50];
    snprintf(temp, sizeof(temp), "%d", number);

    
    int length = strlen(temp);
    int is_negative = (temp[0] == '-');
    
    int groups = (length - is_negative - 1) / 3;

    int new_length = length + groups;
    char* result = (char*)malloc(new_length + 1);
    if (!result) return NULL; 

    int src_index = length - 1;
    int dest_index = new_length;
    result[dest_index--] = '\0'; 
      int count = 0;
    while (src_index >= is_negative) {
        result[dest_index--] = temp[src_index--];
        count++;
        if (count == 3 && src_index >= is_negative) {
            result[dest_index--] = separator;
            count = 0;
        }
    }
    
    if (is_negative) {
        result[dest_index] = '-';
    }

    return result;
}


char* extract_middle_string(const char *input) {
    
    char temp[256];
    strcpy(temp, input);
    
    strtok(temp, " -");
    char *middle_part = strtok(NULL, "-");
    
    if (middle_part != NULL) {
        
        while (*middle_part == ' ') {
            middle_part++;
        }

        char *end = middle_part + strlen(middle_part) - 1;
        while (end > middle_part && *end == ' ') {
            end--;
        }
        *(end + 1) = '\0'; 
        
        char *result = (char*)malloc(strlen(middle_part) + 1);
        if (result != NULL) {
            strcpy(result, middle_part);
            return result;
        }
    }

    
    return NULL;
}

void split_date_time(const char *input, char *date, char *time) {
    
    const char *space_ptr = strchr(input, ' ');
    if (space_ptr == NULL || strlen(input) < 19) {
        strcpy(date, "Invalid format");
        strcpy(time, "Invalid format");
        return;
    }
    size_t date_length = space_ptr - input; 
    strncpy(date, input, date_length);
    date[date_length] = '\0'; 
    strcpy(time, space_ptr + 1);
}

bool checkHaveSeat(int available_economy, int available_business, int available_first,const char *type, int number_seat){
    if (strcmp(type, "Economy") == 0)
        return available_economy >= number_seat;
    else if (strcmp(type, "Business") == 0)
        return available_business >= number_seat;
    else return available_first  >= number_seat;
}

void filter_flights(const Flight *flights, int flight_count, Flight *temp_flight, int *temp_count_flight, const char *departure, const char *arrival, const char *date,const char *class,const int number_seat) {
    *temp_count_flight = 0; 
    char date_split[20];
    char time_split[20];
    for (int i = 0; i < flight_count; i++) {
        split_date_time(flights[i].departure_time, date_split, time_split);
        printf("Check date: %s %s %s\n", date, date_split, strcmp(date, date_split) == 0 ? "true": "false");
        if (strcmp(extract_middle_string(flights[i].departure_airport), extract_middle_string(departure)) == 0 &&
        strcmp(extract_middle_string(flights[i].arrival_airport), extract_middle_string(arrival)) == 0 &&
        strcmp(date_split, date) == 0 && checkHaveSeat(flights[i].available_economy, flights[i].available_business, flights[i].available_first_class, class, number_seat) == true)  {
            temp_flight[*temp_count_flight] = flights[i];
            (*temp_count_flight)++;
        }
    }
    printf("After filter\n");
}