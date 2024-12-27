#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
Ticket list_tickets[MAX_LENGTH];
int ticket_count;
int seat_count;
char **seats_array;
int user_id;
int price = 0;
int number_seat_order;
int final_price;
char class[30];
char **temp_seats = NULL;
int tem_seats_size = 0;
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


int get_seat_position(const char *seat_code, int *i, int *j) {
    const char *columns = "ABCDEFGHIJ";
    int row;
    char column;
    
    if (sscanf(seat_code, "%d%c", &row, &column) != 2) {
        return -1; 
    }    
    *i = row - 1; 

    const char *column_ptr = strchr(columns, column);
    if (column_ptr == NULL) {
        return -1; 
    }
    *j = column_ptr - columns; 

    return 0; 
}

char **add_string_to_array(char **array, int *size, const char *new_string) {
    int index = -1;
    
    for (int i = 0; i < *size; i++) {
        if (strcmp(array[i], new_string) == 0) { 
            index = i;
            break;
        }
    }
    if (index != -1) { 
        printf("String '%s' not found in the array\n", new_string);
        return array;
    }

    char **new_array = realloc(array, (*size + 1) * sizeof(char *));
    if (!new_array) {
        perror("Failed to reallocate memory");
        return array; 
    }
    new_array[*size] = strdup(new_string); 
    if (!new_array[*size]) {
        perror("Failed to duplicate string");
        return new_array;
    }
    (*size)++; 
    return new_array;
}

char **remove_string_from_array(char **array, int *size, const char *target) {
    int index = -1;
    
    for (int i = 0; i < *size; i++) {
        if (strcmp(array[i], target) == 0) { 
            index = i;
            break;
        }
    }
    if (index == -1) { 
        printf("String '%s' not found in the array\n", target);
        return array;
    }
    free(array[index]); 
    
    for (int i = index; i < *size - 1; i++) {
        array[i] = array[i + 1];
    }
    
    char **new_array = realloc(array, (*size - 1) * sizeof(char *));
    if (!new_array && *size > 1) { 
        perror("Failed to reallocate memory");
        return array; 
    }
    (*size)--; 
    return new_array;
}

void print_array(char **array, int size) {
    printf("Array:\n");
    for (int i = 0; i < size; i++) {
        printf("  [%d]: %s\n", i, array[i]);
    }
}

char *join_strings(char **array, int size, const char *delimiter) {
    if (size == 0) {
        return strdup(""); 
    }
    
    int total_length = 0;
    int delimiter_length = strlen(delimiter);
    for (int i = 0; i < size; i++) {
        total_length += strlen(array[i]);
        if (i < size - 1) {
            total_length += delimiter_length; 
        }
    }    
    char *result = malloc(total_length + 1); 
    if (!result) {
        perror("Memory allocation failed");
        return NULL;
    }
    result[0] = '\0'; 
    for (int i = 0; i < size; i++) {
        strcat(result, array[i]);
        if (i < size - 1) {
            strcat(result, delimiter);
        }
    }

    return result;
}

int time_to_seconds(const char *time) {
    int hh, mm, ss;
    sscanf(time, "%d:%d:%d", &hh, &mm, &ss);
    return hh * 3600 + mm * 60 + ss;
}


void seconds_to_time(int total_seconds, char *time_str) {
    int hh = (total_seconds / 3600) % 24; 
    int mm = (total_seconds % 3600) / 60; 
    int ss = total_seconds % 60;         
    sprintf(time_str, "%02d:%02d:%02d", hh, mm, ss);
}


char *calculate_end_time(const char *start_time, int duration) {
    
    int start_seconds = time_to_seconds(start_time);

    
    int end_seconds = start_seconds + duration;

    
    char *end_time = malloc(9); 
    if (!end_time) {
        perror("Memory allocation failed");
        return NULL;
    }
    seconds_to_time(end_seconds, end_time);

    return end_time;
}


int is_valid_date(const char *date_str) {
    struct tm tm;
    time_t now;
    time(&now);  
    struct tm *current_time = localtime(&now);

    
    if (strptime(date_str, "%Y-%m-%d", &tm) == NULL) {
        return 0;  
    }

    
    if (tm.tm_year > current_time->tm_year) {
        return 0;  
    } else if (tm.tm_year == current_time->tm_year) {
        if (tm.tm_mon > current_time->tm_mon) {
            return 0;  
        } else if (tm.tm_mon == current_time->tm_mon) {
            if (tm.tm_mday > current_time->tm_mday) {
                return 0;  
            }
        }
    }

    return 1;  
}