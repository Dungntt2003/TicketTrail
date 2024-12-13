#include <gtk/gtk.h>
#include "book_seat.h"
#include <pango/pango.h>
#include <pango/pangocairo.h>
#include <math.h>
#include <string.h>
#include <arpa/inet.h>
#include "../global/global.h"

#define ROWS 10
#define SEATS_PER_ROW 10

typedef struct {
    int row;
    int seat;
    gboolean selected;
    char label[4];
} Seat;
Seat seats[ROWS][SEATS_PER_ROW];

// Các giá trị có thể thay đổi
const char *departure_city = "HA NOI";
const char *departure_airport = "Noi Bai Airport";
const char *arrival_city = "HO CHI MINH";
const char *arrival_airport = "Tan Son Nhat Airport";
const char *departure_date = "08/12/24"; 
const char *arrival_date = "08/12/24";  
const char *departure_time = "9:30 PM";   
const char *arrival_time = "12:30 PM"; 
const char *ticket_price = "$240"; 
char selected_seat_label[4] = ""; // Ví dụ: "1A" hoặc "2B"

void initialize_seats() {
    const char *columns = "ABCDEFGHIJ";
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < SEATS_PER_ROW; j++) {
            seats[i][j].row = i + 1;
            seats[i][j].seat = j + 1;
            seats[i][j].selected = FALSE;
            snprintf(seats[i][j].label, sizeof(seats[i][j].label), "%d%c", i + 1, columns[j]);
        }
    }
}


static gboolean on_book_seat_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    GdkPixbuf *bg_pixbuf;
    GdkPixbuf *scaled_pixbuf;

    // Lấy kích thước màn hình
    gint screen_width = gtk_widget_get_allocated_width(widget);
    gint screen_height = gtk_widget_get_allocated_height(widget);

    // Vẽ nền (background)
    bg_pixbuf = gdk_pixbuf_new_from_file("../../assets/images/bg_login.png", NULL);
    if (!bg_pixbuf) {
        g_print("Error loading image!\n");
        return FALSE;
    }

    scaled_pixbuf = gdk_pixbuf_scale_simple(bg_pixbuf, screen_width, screen_height, GDK_INTERP_BILINEAR);
    gdk_cairo_set_source_pixbuf(cr, scaled_pixbuf, 0, 0);
    cairo_paint(cr);
    g_object_unref(scaled_pixbuf);
    g_object_unref(bg_pixbuf);

    // Vẽ phần nội dung trắng
    // Vẽ nền trắng với góc bo tròn
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // Màu nền trắng
    // Kích thước hình chữ nhật
    // Kích thước và khoảng cách hình chữ nhật
gfloat rect_width_left = 600.0;  // Chiều rộng hình chữ nhật bên trái
gfloat rect_height_left = 588.0; // Chiều cao hình chữ nhật bên trái
gfloat padding_left = 50.0;      // Khoảng cách cách lề trái
// Tính toán vị trí cho hình chữ nhật bên trái
gfloat x_position_left = padding_left;                     // Cách lề trái
gfloat y_position_left = (screen_height - rect_height_left) / 2; // Căn giữa theo chiều dọc

    gfloat rect_width = 500.0;  // Chiều rộng nhỏ hơn
    gfloat rect_height = 588; // Chiều cao lớn hơn
    gfloat radius = 24.0;       // Bán kính bo góc
    gfloat padding_right = 50.0; // Khoảng cách cách lề phải
    gfloat x_position = screen_width - rect_width - padding_right; // Căn bên phải màn hình
    gfloat y_position = (screen_height - rect_height) / 2; // Căn giữa theo chiều dọc

    // Tạo các góc bo tròn (arc) cho hình chữ nhật
    cairo_move_to(cr, x_position + radius, y_position);
cairo_arc(cr, x_position + rect_width - radius, y_position + radius, radius, 3 * M_PI / 2, 2 * M_PI); // Góc trên bên phải
cairo_arc(cr, x_position + rect_width - radius, y_position + rect_height - radius, radius, 0, M_PI / 2); // Góc dưới bên phải
cairo_arc(cr, x_position + radius, y_position + rect_height - radius, radius, M_PI / 2, M_PI); // Góc dưới bên trái
cairo_arc(cr, x_position + radius, y_position + radius, radius, M_PI, 3 * M_PI / 2); // Góc trên bên trái

    cairo_close_path(cr); // Kết thúc đường vẽ

    cairo_set_line_width(cr, 1.0);
    cairo_fill_preserve(cr); // Vẽ hình chữ nhật với các góc bo tròn
    cairo_set_source_rgb(cr, 0.9, 0.9, 0.9); // Màu bóng đổ nhẹ
    cairo_stroke(cr); // Vẽ đường viền bóng đổ

    // Bắt đầu vẽ hình chữ nhật bên trái
cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // Màu nền xanh nhạt
cairo_move_to(cr, x_position_left + radius, y_position_left);
cairo_arc(cr, x_position_left + rect_width_left - radius, y_position_left + radius, radius, 3 * M_PI / 2, 2 * M_PI); // Góc trên bên phải
cairo_arc(cr, x_position_left + rect_width_left - radius, y_position_left + rect_height_left - radius, radius, 0, M_PI / 2); // Góc dưới bên phải
cairo_arc(cr, x_position_left + radius, y_position_left + rect_height_left - radius, radius, M_PI / 2, M_PI); // Góc dưới bên trái
cairo_arc(cr, x_position_left + radius, y_position_left + radius, radius, M_PI, 3 * M_PI / 2); // Góc trên bên trái
cairo_close_path(cr);
cairo_fill_preserve(cr); // Tô màu nền
cairo_set_source_rgb(cr, 0.9, 0.9, 0.9); // Màu viền xám
cairo_stroke(cr); // Vẽ viền

double seat_width = 30, seat_height = 30;
    double margin = 10;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < SEATS_PER_ROW; j++) {
            double x = 150 + margin + j * (seat_width + margin);
            double y = 150 + margin + i * (seat_height + margin);

            // Set seat color based on class
            if (i == 0) {
                cairo_set_source_rgb(cr, 1.0, 0.75, 0.8); // Pink for First class
            } else if (i == 1 || i == 2) {
                cairo_set_source_rgb(cr, 0.7, 0.9, 1.0); // Light blue for Business class
            } else {
                cairo_set_source_rgb(cr, 0.7, 0.5, 1.0); // Blue for Economy class
            }

            // Change color if selected
            if (seats[i][j].selected) {
                cairo_set_source_rgb(cr, 0.5, 0.5, 0.5); // Gray for selected
            }

            cairo_rectangle(cr, x, y, seat_width, seat_height);
            cairo_fill(cr);

            // Draw seat label
            cairo_set_source_rgb(cr, 0, 0, 0); // Black for text
            cairo_move_to(cr, x + seat_width / 4, y + seat_height / 1.5);
            cairo_show_text(cr, seats[i][j].label);
        }
    }



    // Vẽ các phần nội dung khác
    // Logo Airline
    GdkPixbuf *airline_logo = gdk_pixbuf_new_from_file("../../assets/images/airline.png", NULL);
    if (airline_logo) {
        GdkPixbuf *scaled_logo = gdk_pixbuf_scale_simple(airline_logo, 219, 53, GDK_INTERP_BILINEAR);
        gdk_cairo_set_source_pixbuf(cr, scaled_logo,  x_position + radius + 120, y_position + 10);
        cairo_paint(cr);
        g_object_unref(scaled_logo);
        g_object_unref(airline_logo);
    }

    //Vẽ đường kẻ dưới logo
    cairo_set_source_rgb(cr, 0.91, 0.91, 0.91); // Màu #E6E8E7
    cairo_set_line_width(cr, 1.0);
    cairo_move_to(cr, x_position + radius, y_position + 61);
    cairo_line_to(cr, x_position + radius + 450, y_position + 61);
    cairo_stroke(cr);

    // Thêm phần nội dungđiểm đi và điểm đến
    // Vẽ điểm đi (HANOI)
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Màu đen
    cairo_select_font_face(cr, "Poppins", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);
    cairo_move_to(cr, (screen_width - 936) / 2 + 600, (screen_height - 588) / 2 + 100);
    cairo_show_text(cr, departure_city);

    // Noi Bai Airport
    cairo_set_source_rgb(cr, 0.4, 0.4, 0.4); // Màu xám
    cairo_select_font_face(cr, "Poppins", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12);
    cairo_move_to(cr, (screen_width - 936) / 2 + 580, (screen_height - 588) / 2 + 115);
    cairo_show_text(cr, departure_airport);

    // Vẽ điểm đến (HOCHIMINH)
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Màu đen
    cairo_move_to(cr, (screen_width - 936) / 2 + 730, (screen_height - 588) / 2 + 100);
    cairo_set_font_size(cr, 14);
    cairo_show_text(cr, arrival_city);

    // Tan Son Nhat Airport
    cairo_set_source_rgb(cr, 0.4, 0.4, 0.4); // Màu xám
    cairo_move_to(cr, (screen_width - 936) / 2 + 720, (screen_height - 588) / 2 + 115);
    cairo_set_font_size(cr, 12);
    cairo_show_text(cr, "Tan Son Nhat Airport");

    // Vẽ biểu tượng chuyến bay
    GdkPixbuf *flight_icon = gdk_pixbuf_new_from_file("../../assets/images/flight-icon.png", NULL);
    if (flight_icon) {
        GdkPixbuf *scaled_flight_icon = gdk_pixbuf_scale_simple(flight_icon, 59, 14, GDK_INTERP_BILINEAR);
        gdk_cairo_set_source_pixbuf(cr, scaled_flight_icon, (screen_width - 936) / 2 + 660, (screen_height - 588) / 2 + 88);
        cairo_paint(cr);
        g_object_unref(scaled_flight_icon);
        g_object_unref(flight_icon);
    }

    // Đặt màu ve ke doc
cairo_set_source_rgb(cr, 0.91, 0.91, 0.91); 
cairo_set_line_width(cr, 1.0);

// Tọa độ bắt đầu và kết thúc đường kẻ dọc
double x_pos = (screen_width - 936) / 2 + 850; // Hoành độ giữ nguyên
double y_start = (screen_height - 588) / 2 + 65; // Tung độ bắt đầu
double y_end = y_start + 75; // Tung độ kết thúc (chiều cao hình chữ nhật)

// Vẽ đường kẻ doc
cairo_move_to(cr, x_pos, y_start); // Điểm bắt đầu
cairo_line_to(cr, x_pos, y_end);  // Điểm kết thúc
cairo_stroke(cr);


    //Vẽ đường kẻ dưới 
    cairo_set_source_rgb(cr, 0.91, 0.91, 0.91); // Màu #E6E8E7
    cairo_set_line_width(cr, 1.0);
    cairo_move_to(cr, x_position + radius, y_position + 150);
    cairo_line_to(cr, x_position + radius + 450, y_position + 150);
    cairo_stroke(cr);

     // Vẽ date depature
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Màu đen
    cairo_select_font_face(cr, "Poppins", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);
    cairo_move_to(cr, (screen_width - 936) / 2 + 870, (screen_height - 588) / 2 + 100);
    cairo_show_text(cr, departure_date);

    // Time depature
    cairo_set_source_rgb(cr, 0.4, 0.4, 0.4); // Màu xám
    cairo_select_font_face(cr, "Poppins", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12);
    cairo_move_to(cr, (screen_width - 936) / 2 + 870, (screen_height - 588) / 2 + 115);
    cairo_show_text(cr, departure_time);

    // Vẽ date arrived
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Màu đen
    cairo_move_to(cr, (screen_width - 936) / 2 + 970, (screen_height - 588) / 2 + 100);
    cairo_set_font_size(cr, 14);
    cairo_show_text(cr, arrival_date);

      // Vẽ -
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Màu đen
    cairo_move_to(cr, (screen_width - 936) / 2 + 940, (screen_height - 588) / 2 + 100);
    cairo_set_font_size(cr, 14);
    cairo_show_text(cr, "--");

    // time arrived
    cairo_set_source_rgb(cr, 0.4, 0.4, 0.4); // Màu xám
    cairo_move_to(cr, (screen_width - 936) / 2 + 970, (screen_height - 588) / 2 + 115);
    cairo_set_font_size(cr, 12);
    cairo_show_text(cr, arrival_time);  


    //Logo cac hang ghe
      GdkPixbuf *economy_logo = gdk_pixbuf_new_from_file("../../assets/images/Economy.png", NULL);
    if (economy_logo) {
        GdkPixbuf *scaled_economy = gdk_pixbuf_scale_simple(economy_logo, 213, 120, GDK_INTERP_BILINEAR);
        gdk_cairo_set_source_pixbuf(cr, scaled_economy,  x_position + radius - 30, y_position + 200);
        cairo_paint(cr);
        g_object_unref(scaled_economy);
        g_object_unref(economy_logo);
    }

     // Vẽ ten hang
    cairo_set_source_rgb(cr, 0.7, 0.5, 1.0); 
    cairo_select_font_face(cr, "Poppins", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);
    cairo_move_to(cr, (screen_width - 936) / 2 + 630, (screen_height - 588) / 2 + 330);
    cairo_show_text(cr, "Economy Class");

      GdkPixbuf *business_logo = gdk_pixbuf_new_from_file("../../assets/images/Business.png", NULL);
    if (business_logo) {
        GdkPixbuf *scaled_business = gdk_pixbuf_scale_simple(business_logo, 213, 120, GDK_INTERP_BILINEAR);
        gdk_cairo_set_source_pixbuf(cr, scaled_business,  x_position + radius + 230, y_position + 200);
        cairo_paint(cr);
        g_object_unref(scaled_business);
        g_object_unref(business_logo);
    }

    //
    cairo_set_source_rgb(cr, 0.7, 0.9, 1.0); 
    cairo_select_font_face(cr, "Poppins", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);
    cairo_move_to(cr, (screen_width - 936) / 2 + 870, (screen_height - 588) / 2 + 330);
    cairo_show_text(cr, "Business Class");

     GdkPixbuf *first_logo = gdk_pixbuf_new_from_file("../../assets/images/First.png", NULL);
    if (first_logo) {
        GdkPixbuf *scaled_first = gdk_pixbuf_scale_simple(first_logo, 213, 120, GDK_INTERP_BILINEAR);
        gdk_cairo_set_source_pixbuf(cr, scaled_first,  x_position + radius + 100, y_position + 350);
        cairo_paint(cr);
        g_object_unref(scaled_first);
        g_object_unref(first_logo);
    }

    //
    cairo_set_source_rgb(cr, 1.0, 0.75, 0.8); 
    cairo_select_font_face(cr, "Poppins", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);
    cairo_move_to(cr, (screen_width - 936) / 2 + 750, (screen_height - 588) / 2 + 480);
    cairo_show_text(cr, "First Class");


      //Vẽ đường kẻ dưới 
    cairo_set_source_rgb(cr, 0.91, 0.91, 0.91); // Màu #E6E8E7
    cairo_set_line_width(cr, 1.0);
    cairo_move_to(cr, x_position + radius, y_position + 500);
    cairo_line_to(cr, x_position + radius + 450, y_position + 500);
    cairo_stroke(cr);  

    //seat number
   cairo_set_source_rgb(cr, 0.4, 0.4, 0.4); // Màu đen
    cairo_select_font_face(cr, "Poppins", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);
    cairo_move_to(cr, (screen_width - 936) / 2 + 600, (screen_height - 588) / 2 + 550);
    cairo_show_text(cr, "Seat number: ");
    cairo_show_text(cr, selected_seat_label); 

   // Vẽ nút Confirm
cairo_set_source_rgb(cr, 0.13, 0.23, 0.37); // Màu nền xanh đậm (#223A60)
cairo_new_path(cr);
cairo_arc(cr, screen_width - 70 - 120 + 8, (screen_height - 588) / 2 + 530 + 8, 8, M_PI, 3 * M_PI / 2); // Góc trên bên trái
cairo_arc(cr, screen_width - 70 - 8, (screen_height - 588) / 2 + 530 + 8, 8, 3 * M_PI / 2, 2 * M_PI); // Góc trên bên phải
cairo_arc(cr, screen_width - 70 - 8, (screen_height - 588) / 2 + 530 + 40 - 8, 8, 0, M_PI / 2); // Góc dưới bên phải
cairo_arc(cr, screen_width - 70 - 120 + 8, (screen_height - 588) / 2 + 530 + 40 - 8, 8, M_PI / 2, M_PI); // Góc dưới bên trái
cairo_close_path(cr);
cairo_fill_preserve(cr); // Vẽ nền nút
cairo_stroke(cr); // Vẽ viền nút

// Vẽ text "Confirm" trên nút Confirm
cairo_set_source_rgb(cr, 0.92, 0.94, 0.94); // Màu chữ (#EAF0F0)
cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL); // Font chữ
cairo_set_font_size(cr, 16); // Kích thước chữ

// Căn giữa text "Confirm"
cairo_text_extents_t confirm_extents;
cairo_text_extents(cr, "Confirm", &confirm_extents);
cairo_move_to(
    cr,
    screen_width - 70 - 120 / 2 - confirm_extents.width / 2, // Căn giữa theo chiều ngang
    (screen_height - 588) / 2 + 530 + 40 / 2 + confirm_extents.height / 2 // Căn giữa theo chiều dọc
);
    cairo_show_text(cr, "Confirm");

        return FALSE;
    }

  gboolean on_mouse_click(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    double seat_width = 30, seat_height = 30;
    double margin = 10;
    double offset_x = 150; // Cập nhật với vị trí mới

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < SEATS_PER_ROW; j++) {
            // Cập nhật tọa độ ghế theo mới
            double x = offset_x + margin + j * (seat_width + margin);
            double y = 150 + margin + i * (seat_height + margin);  // Vẫn giữ margin cho Y

            // Kiểm tra nếu chuột click vào ghế
            if (event->x >= x && event->x <= x + seat_width && event->y >= y && event->y <= y + seat_height) {
                // Toggle trạng thái chọn của ghế
                seats[i][j].selected = !seats[i][j].selected;
                gtk_widget_queue_draw(widget);  // Yêu cầu vẽ lại widget

                // In ra thông báo ghế được chọn hay bỏ chọn
                if (seats[i][j].selected) {
                    strncpy(selected_seat_label, seats[i][j].label, sizeof(selected_seat_label));
                    g_print("Seat selected: %s\n", selected_seat_label);
                } else {
                    g_print("Seat deselected: %s\n", seats[i][j].label);
                }
                return TRUE;
            }
        }
    }

    return FALSE;
}

  GtkWidget* create_book_seat_window() {
    GtkWidget *drawing_area, *main_box;

    // Tạo một box chính để chứa nội dung
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

     initialize_seats(); 

    // Tạo khu vực vẽ
    drawing_area = gtk_drawing_area_new();
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_book_seat_draw), NULL);
     g_signal_connect(G_OBJECT(drawing_area), "button-press-event", G_CALLBACK(on_mouse_click), NULL);

 gtk_widget_add_events(drawing_area, GDK_BUTTON_PRESS_MASK);

    // Thêm drawing_area vào main_box
    gtk_box_pack_start(GTK_BOX(main_box), drawing_area, TRUE, TRUE, 0);

    return main_box;
}