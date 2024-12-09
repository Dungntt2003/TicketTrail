#include <gtk/gtk.h>
#include "ticket_detail.h"
#include <pango/pango.h>
#include <pango/pangocairo.h>
#include <math.h>
#include <string.h>
#include <arpa/inet.h>
#include "../global/global.h"


// Các giá trị có thể thay đổi
const char *departure_city = "HA NOI";
const char *departure_airport = "Noi Bai Airport";
const char *arrival_city = "HO CHI MINH";
const char *arrival_airport = "Tan Son Nhat Airport";
const char *selected_date = "08/12/2024";  
const char *selected_time = "9:30 PM";   
const char *ticket_price = "$240"; 


static gboolean on_ticket_detail_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    GdkPixbuf *bg_pixbuf;
    GdkPixbuf *scaled_pixbuf;

    // Lấy kích thước màn hình
    gint screen_width = gtk_widget_get_allocated_width(widget);
    gint screen_height = gtk_widget_get_allocated_height(widget);

    // Vẽ nền (background)
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

    // Vẽ phần nội dung trắng
    // Vẽ nền trắng với góc bo tròn
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // Màu nền trắng
    gfloat radius = 24.0;

    // Tạo các góc bo tròn (arc) cho hình chữ nhật
    cairo_move_to(cr, (screen_width - 936) / 2 + radius, (screen_height - 588) / 2); // Di chuyển đến góc trên bên trái
    cairo_arc(cr, (screen_width - 936) / 2 + 936 - radius, (screen_height - 588) / 2 + radius, radius, 3 * M_PI / 2, 2 * M_PI); // Góc trên bên phải
    cairo_arc(cr, (screen_width - 936) / 2 + 936 - radius, (screen_height - 588) / 2 + 588 - radius, radius, 0, M_PI / 2); // Góc dưới bên phải
    cairo_arc(cr, (screen_width - 936) / 2 + radius, (screen_height - 588) / 2 + 588 - radius, radius, M_PI / 2, M_PI); // Góc dưới bên trái
    cairo_arc(cr, (screen_width - 936) / 2 + radius, (screen_height - 588) / 2 + radius, radius, M_PI, 3 * M_PI / 2); // Góc trên bên trái

    cairo_close_path(cr); // Kết thúc đường vẽ

    cairo_set_line_width(cr, 1.0);
    cairo_fill_preserve(cr); // Vẽ hình chữ nhật với các góc bo tròn
    cairo_set_source_rgb(cr, 0.9, 0.9, 0.9); // Màu bóng đổ nhẹ
    cairo_stroke(cr); // Vẽ đường viền bóng đổ



    // Vẽ các phần nội dung khác
    // Logo Airline
    GdkPixbuf *airline_logo = gdk_pixbuf_new_from_file("../assets/images/airline.png", NULL);
    if (airline_logo) {
        GdkPixbuf *scaled_logo = gdk_pixbuf_scale_simple(airline_logo, 328, 80, GDK_INTERP_BILINEAR);
        gdk_cairo_set_source_pixbuf(cr, scaled_logo, (screen_width - 936) / 2 + (936 - 328) / 2, (screen_height - 588) / 2 + 10);
        cairo_paint(cr);
        g_object_unref(scaled_logo);
        g_object_unref(airline_logo);
    }

    // Vẽ đường kẻ dưới logo
    cairo_set_source_rgb(cr, 0.91, 0.91, 0.91); // Màu #E6E8E7
    cairo_set_line_width(cr, 1.0);
    cairo_move_to(cr, (screen_width - 936) / 2 + (936 - 600) / 2, (screen_height - 588) / 2 + 90);
    cairo_line_to(cr, (screen_width - 936) / 2 + (936 - 600) / 2 + 600, (screen_height - 588) / 2 + 90);
    cairo_stroke(cr);

    // Thêm phần nội dung điểm đi và điểm đến
    // Vẽ điểm đi (HANOI)
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Màu đen
    cairo_select_font_face(cr, "Poppins", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 24);
    cairo_move_to(cr, (screen_width - 936) / 2 + 220, (screen_height - 588) / 2 + 160);
    cairo_show_text(cr, departure_city);

    // Noi Bai Airport
    cairo_set_source_rgb(cr, 0.4, 0.4, 0.4); // Màu xám
    cairo_select_font_face(cr, "Poppins", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 20);
    cairo_move_to(cr, (screen_width - 936) / 2 + 200, (screen_height - 588) / 2 + 190);
    cairo_show_text(cr, departure_airport);

    // Vẽ điểm đến (HOCHIMINH)
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Màu đen
    cairo_move_to(cr, (screen_width - 936) / 2 + 620, (screen_height - 588) / 2 + 160);
    cairo_set_font_size(cr, 24);
    cairo_show_text(cr, arrival_city);

    // Tan Son Nhat Airport
    cairo_set_source_rgb(cr, 0.4, 0.4, 0.4); // Màu xám
    cairo_move_to(cr, (screen_width - 936) / 2 + 600, (screen_height - 588) / 2 + 190);
    cairo_set_font_size(cr, 20);
    cairo_show_text(cr, "Tan Son Nhat Airport");

    // Vẽ biểu tượng chuyến bay
    GdkPixbuf *flight_icon = gdk_pixbuf_new_from_file("../assets/images/flight-icon.png", NULL);
    if (flight_icon) {
        GdkPixbuf *scaled_flight_icon = gdk_pixbuf_scale_simple(flight_icon, 156, 36, GDK_INTERP_BILINEAR);
        gdk_cairo_set_source_pixbuf(cr, scaled_flight_icon, (screen_width - 936) / 2 + (936 - 156) / 2, (screen_height - 588) / 2 + 160);
        cairo_paint(cr);
        g_object_unref(scaled_flight_icon);
        g_object_unref(flight_icon);
    }

    // Vẽ đường kẻ dưới điểm đến
    cairo_set_source_rgb(cr, 0.91, 0.91, 0.91); // Màu #E6E8E7
    cairo_move_to(cr, (screen_width - 936) / 2 + (936 - 600) / 2, (screen_height - 588) / 2 + 250);
    cairo_line_to(cr, (screen_width - 936) / 2 + (936 - 600) / 2 + 600, (screen_height - 588) / 2 + 250);
    cairo_stroke(cr);

    // Vẽ ô chọn Date và Time
    // Cập nhật lại các label "Date" và "Time"
        cairo_set_source_rgb(cr, 0.4, 0.4, 0.4); // Màu xám
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 20); // Chỉnh font size cho label là 20px

    // Tính toán vị trí căn giữa
    cairo_text_extents_t date_extents, time_extents;
    cairo_text_extents(cr, "Date", &date_extents);
    cairo_text_extents(cr, "Time", &time_extents);

    // Vẽ label "Date"
    cairo_move_to(cr, (screen_width - 936) / 2 + 270 + (140 + 16) / 2 - date_extents.width / 2, 
                (screen_height - 588) / 2 + 300 - 10); // Cách ô 10px
    cairo_show_text(cr, "Date");

    // Vẽ label "Time"
    cairo_move_to(cr, (screen_width - 936) / 2 + 16 + 140 + 270 + (140 + 16) / 2 - time_extents.width / 2,
                (screen_height - 588) / 2 + 300 - 10); // Cách ô 10px
    cairo_show_text(cr, "Time");

    // Vẽ ô "Date" (chỉnh lại căn giữa theo chiều ngang)
    double date_x = (screen_width - 936) / 2 + 320;
    double time_x = date_x + 140 + 16;

    // Vẽ ô Date
    // Vẽ ô Date với góc bo tròn
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // Màu nền trắng cho ô
    cairo_new_path(cr);

    // Vẽ hình chữ nhật bo góc với bán kính 8px
    cairo_move_to(cr, date_x + 8, (screen_height - 588) / 2 + 300); // Đi đến điểm bắt đầu

    // Vẽ các cạnh bo góc
    cairo_line_to(cr, date_x + 140 - 8, (screen_height - 588) / 2 + 300); // Cạnh trên
    cairo_arc(cr, date_x + 140 - 8, (screen_height - 588) / 2 + 300 + 8, 8, 3 * M_PI / 2, 2 * M_PI); // Góc trên phải

    cairo_line_to(cr, date_x + 140, (screen_height - 588) / 2 + 300 + 46 - 8); // Cạnh phải
    cairo_arc(cr, date_x + 140 - 8, (screen_height - 588) / 2 + 300 + 46 - 8, 8, 0, M_PI / 2); // Góc dưới phải

    cairo_line_to(cr, date_x + 8, (screen_height - 588) / 2 + 300 + 46); // Cạnh dưới
    cairo_arc(cr, date_x + 8, (screen_height - 588) / 2 + 300 + 46 - 8, 8, M_PI / 2, M_PI); // Góc dưới trái

    cairo_line_to(cr, date_x, (screen_height - 588) / 2 + 300 + 8); // Cạnh trái
    cairo_arc(cr, date_x + 8, (screen_height - 588) / 2 + 300 + 8, 8, M_PI, 3 * M_PI / 2); // Góc trên trái

    cairo_close_path(cr); // Đóng đường path

    cairo_fill_preserve(cr); // Vẽ nền ô
    cairo_set_source_rgb(cr, 0.91, 0.91, 0.91); // Màu xám nhạt cho viền
    cairo_stroke(cr); // Vẽ viền ô

    // Vẽ biểu tượng Calendar.png trong ô Date
    GdkPixbuf *calendar_icon = gdk_pixbuf_new_from_file("../assets/images/Calendar.png", NULL);
    if (calendar_icon) {
        GdkPixbuf *scaled_calendar_icon = gdk_pixbuf_scale_simple(calendar_icon, 20, 20, GDK_INTERP_BILINEAR);
        gdk_cairo_set_source_pixbuf(cr, scaled_calendar_icon, date_x + 8, (screen_height - 588) / 2 + 300 + (46 - 20) / 2);
        cairo_paint(cr);
        g_object_unref(scaled_calendar_icon);
        g_object_unref(calendar_icon);
    }

    // Vẽ text "15/07/2022" trong ô Date
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Màu đen cho text
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 18); // Kích thước chữ là 20px

    // Tính toán chiều rộng của văn bản "15/07/2022"
    cairo_text_extents_t date_text_extents;
    cairo_text_extents(cr, selected_date, &date_text_extents);

    // Tính toán vị trí dọc sao cho văn bản căn giữa với biểu tượng Calendar
    double date_text_y = (screen_height - 588) / 2 + 315 + (46 - date_text_extents.height) / 2;

    // Căn chỉnh vị trí của văn bản "15/07/2022" theo chiều ngang (căn giữa trong ô)
    cairo_move_to(cr, date_x + (160 - date_text_extents.width) / 2, date_text_y);
    cairo_show_text(cr, selected_date);

    // Vẽ ô "Time"
    // Vẽ ô "Time" với góc bo tròn
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // Màu nền trắng cho ô
    cairo_new_path(cr);

    // Vẽ hình chữ nhật bo góc với bán kính 8px
    cairo_move_to(cr, time_x + 8, (screen_height - 588) / 2 + 300); // Đi đến điểm bắt đầu

    // Vẽ các cạnh bo góc
    cairo_line_to(cr, time_x + 140 - 8, (screen_height - 588) / 2 + 300); // Cạnh trên
    cairo_arc(cr, time_x + 140 - 8, (screen_height - 588) / 2 + 300 + 8, 8, 3 * M_PI / 2, 2 * M_PI); // Góc trên phải

    cairo_line_to(cr, time_x + 140, (screen_height - 588) / 2 + 300 + 46 - 8); // Cạnh phải
    cairo_arc(cr, time_x + 140 - 8, (screen_height - 588) / 2 + 300 + 46 - 8, 8, 0, M_PI / 2); // Góc dưới phải

    cairo_line_to(cr, time_x + 8, (screen_height - 588) / 2 + 300 + 46); // Cạnh dưới
    cairo_arc(cr, time_x + 8, (screen_height - 588) / 2 + 300 + 46 - 8, 8, M_PI / 2, M_PI); // Góc dưới trái

    cairo_line_to(cr, time_x, (screen_height - 588) / 2 + 300 + 8); // Cạnh trái
    cairo_arc(cr, time_x + 8, (screen_height - 588) / 2 + 300 + 8, 8, M_PI, 3 * M_PI / 2); // Góc trên trái

    cairo_close_path(cr); // Đóng đường path

    cairo_fill_preserve(cr); // Vẽ nền ô
    cairo_set_source_rgb(cr, 0.91, 0.91, 0.91); // Màu xám nhạt cho viền
    cairo_stroke(cr); // Vẽ viền ô


    // Vẽ biểu tượng Clock.png trong ô Time
    GdkPixbuf *clock_icon = gdk_pixbuf_new_from_file("../assets/images/Clock.png", NULL);
    if (clock_icon) {
        GdkPixbuf *scaled_clock_icon = gdk_pixbuf_scale_simple(clock_icon, 20, 20, GDK_INTERP_BILINEAR);
        gdk_cairo_set_source_pixbuf(cr, scaled_clock_icon, time_x + 8, (screen_height - 588) / 2 + 300 + (46 - 20) / 2);
        cairo_paint(cr);
        g_object_unref(scaled_clock_icon);
        g_object_unref(clock_icon);
    }

    // Vẽ text "Time"
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Màu đen cho text
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 18); // Font size cho text "12:30 PM"
    cairo_text_extents_t time_text_extents;
    cairo_text_extents(cr, selected_time, &time_text_extents);  // Tính toán kích thước văn bản

    // Tính toán vị trí theo chiều dọc cho "12:30 PM"
    double time_text_y = (screen_height - 588) / 2 + 315 + (46 - time_text_extents.height) / 2;  // Tính toán căn giữa trong ô Time

    // Căn giữa text "12:30 PM" theo chiều ngang trong ô Time
    cairo_move_to(cr, time_x + (150 - time_text_extents.width) / 2, time_text_y);

    // Vẽ text "12:30 PM"
    cairo_show_text(cr, selected_time);



    // Vẽ đường kẻ dưới Date Time
    cairo_set_source_rgb(cr, 0.91, 0.91, 0.91); // Màu #E6E8E7
    cairo_move_to(cr, (screen_width - 936) / 2 + (936 - 600) / 2, (screen_height - 588) / 2 + 380);
    cairo_line_to(cr, (screen_width - 936) / 2 + (936 - 600) / 2 + 600, (screen_height - 588) / 2 + 380);
    cairo_stroke(cr);

    // Vẽ phần Price
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Màu đen cho chữ "Price"
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD); // Chọn font "Inter" với trọng lượng 600 (bold)
    cairo_set_font_size(cr, 22); // Font size 22px
    cairo_text_extents_t price_text_extents;
    cairo_text_extents(cr, "Price", &price_text_extents); // Lấy kích thước của chữ "Price"

    // Tính toán vị trí căn giữa của "Price"
    double price_x = (screen_width - 936) / 2 + (160 - price_text_extents.width) / 2 + 350;
    double price_y = (screen_height - 588) / 2 + 430;

    // Vẽ chữ "Price"
    cairo_move_to(cr, price_x, price_y);
    cairo_show_text(cr, "Price");

    // Vẽ phần "$230"
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Màu đen cho chữ "$230"
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD); // Chọn font "Inter" với trọng lượng 600 (bold)
    cairo_set_font_size(cr, 32); // Font size 32px
    cairo_text_extents_t price_value_extents;
    cairo_text_extents(cr, ticket_price, &price_value_extents); // Lấy kích thước của "$230"

    // Tính toán vị trí căn giữa của "$230" bên cạnh "Price"
    double price_value_x = price_x + price_text_extents.width + 10; // Cộng thêm khoảng cách giữa "Price" và "$230"
    double price_value_y = price_y; // Cùng chiều dọc với "Price"

    // Vẽ chữ "$230"
    cairo_move_to(cr, price_value_x, price_value_y);
    cairo_show_text(cr, ticket_price);


    // Vẽ các nút Cancel và Confirm
    // Vẽ nút Cancel
    cairo_set_source_rgb(cr, 0.92, 0.94, 0.94); // Màu nền xám nhạt (#EAF0F0)
    cairo_new_path(cr);
    cairo_arc(cr, (screen_width - 936) / 2 + (936 - 344) / 2 + 8, (screen_height - 588) / 2 + 500 + 8, 8, M_PI, 3 * M_PI / 2); // Góc trên bên trái
    cairo_arc(cr, (screen_width - 936) / 2 + (936 - 344) / 2 + 156 - 8, (screen_height - 588) / 2 + 500 + 8, 8, 3 * M_PI / 2, 2 * M_PI); // Góc trên bên phải
    cairo_arc(cr, (screen_width - 936) / 2 + (936 - 344) / 2 + 156 - 8, (screen_height - 588) / 2 + 500 + 56 - 8, 8, 0, M_PI / 2); // Góc dưới bên phải
    cairo_arc(cr, (screen_width - 936) / 2 + (936 - 344) / 2 + 8, (screen_height - 588) / 2 + 500 + 56 - 8, 8, M_PI / 2, M_PI); // Góc dưới bên trái
    cairo_close_path(cr);
    cairo_fill_preserve(cr); // Vẽ nền nút
    cairo_set_source_rgb(cr, 0.13, 0.23, 0.37); // Màu viền (#223A60)
    cairo_set_line_width(cr, 1.0);
    cairo_stroke(cr); // Vẽ viền nút

    // Vẽ text "Cancel" trên nút Cancel
    cairo_set_source_rgb(cr, 0.13, 0.23, 0.37); // Màu chữ (#223A60)
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL); // Font chữ
    cairo_set_font_size(cr, 18); // Kích thước chữ

    // Căn giữa text "Cancel"
    cairo_text_extents_t cancel_extents;
    cairo_text_extents(cr, "Cancel", &cancel_extents);
    cairo_move_to(
        cr,
        (screen_width - 936) / 2 + (936 - 344) / 2 + 156 / 2 - cancel_extents.width / 2,
        (screen_height - 588) / 2 + 500 + 56 / 2 + cancel_extents.height / 2
    );
    cairo_show_text(cr, "Cancel");

    // Vẽ nút Confirm
    cairo_set_source_rgb(cr, 0.13, 0.23, 0.37); // Màu nền xanh đậm (#223A60)
    cairo_new_path(cr);
    cairo_arc(cr, (screen_width - 936) / 2 + (936 - 344) / 2 + 180 + 8, (screen_height - 588) / 2 + 500 + 8, 8, M_PI, 3 * M_PI / 2); // Góc trên bên trái
    cairo_arc(cr, (screen_width - 936) / 2 + (936 - 344) / 2 + 180 + 156 - 8, (screen_height - 588) / 2 + 500 + 8, 8, 3 * M_PI / 2, 2 * M_PI); // Góc trên bên phải
    cairo_arc(cr, (screen_width - 936) / 2 + (936 - 344) / 2 + 180 + 156 - 8, (screen_height - 588) / 2 + 500 + 56 - 8, 8, 0, M_PI / 2); // Góc dưới bên phải
    cairo_arc(cr, (screen_width - 936) / 2 + (936 - 344) / 2 + 180 + 8, (screen_height - 588) / 2 + 500 + 56 - 8, 8, M_PI / 2, M_PI); // Góc dưới bên trái
    cairo_close_path(cr);
    cairo_fill_preserve(cr); // Vẽ nền nút
    cairo_stroke(cr); // Vẽ viền nút

    // Vẽ text "Confirm" trên nút Confirm
    cairo_set_source_rgb(cr, 0.92, 0.94, 0.94); // Màu chữ (#EAF0F0)
    cairo_select_font_face(cr, "Inter", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL); // Font chữ
    cairo_set_font_size(cr, 18); // Kích thước chữ

    // Căn giữa text "Confirm"
    cairo_text_extents_t confirm_extents;
    cairo_text_extents(cr, "Confirm", &confirm_extents);
    cairo_move_to(
        cr,
        (screen_width - 936) / 2 + (936 - 344) / 2 + 180 + 156 / 2 - confirm_extents.width / 2,
        (screen_height - 588) / 2 + 500 + 56 / 2 + confirm_extents.height / 2
    );
    cairo_show_text(cr, "Confirm");

        return FALSE;
    }
GtkWidget* create_ticket_detail_window() {
    GtkWidget *drawing_area, *main_box;

    // Tạo một box chính để chứa nội dung
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Tạo khu vực vẽ
    drawing_area = gtk_drawing_area_new();
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_ticket_detail_draw), NULL);

    // Thêm drawing_area vào main_box
    gtk_box_pack_start(GTK_BOX(main_box), drawing_area, TRUE, TRUE, 0);

    return main_box;
}

