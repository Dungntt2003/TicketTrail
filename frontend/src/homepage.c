#include <gtk/gtk.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>
static void on_window_realize(GtkWidget *widget, gpointer user_data) {
    GtkWidget *calendar = GTK_WIDGET(user_data);
    gtk_widget_hide(calendar);  // Ẩn calendar ngay khi cửa sổ được khởi tạo
}
// Hàm vẽ hình nền cho cửa sổ
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    GdkPixbuf *pixbuf;
    GdkPixbuf *scaled_pixbuf;

    // Load hình ảnh từ file
    pixbuf = gdk_pixbuf_new_from_file("assets/images/homepage.png", NULL);
    if (!pixbuf) {
        g_print("Error loading image!\n");
        return FALSE;
    }

    // Thay đổi kích thước ảnh cho phù hợp với kích thước cửa sổ
    gint width = gtk_widget_get_allocated_width(widget);
    gint height = gtk_widget_get_allocated_height(widget);
    scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, width, height, GDK_INTERP_BILINEAR);

    // Vẽ hình ảnh lên cửa sổ
    gdk_cairo_set_source_pixbuf(cr, scaled_pixbuf, 0, 0);
    cairo_paint(cr);

    // Giải phóng bộ nhớ của pixbuf
    g_object_unref(pixbuf);
    g_object_unref(scaled_pixbuf);

    return FALSE;
}

// Hàm xử lý khi cửa sổ bị đóng
static void on_window_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

// Hàm xử lý khi nhấn nút
static void on_button_toggled(GtkToggleButton *button, gpointer user_data) {
    // Lấy tất cả các nút trong group
    GtkWidget **buttons = (GtkWidget **)user_data;

    // Nếu nút này được chọn, bỏ chọn tất cả các nút còn lại
    if (gtk_toggle_button_get_active(button)) {
        for (int i = 0; buttons[i] != NULL; i++) {
            // Bỏ chọn tất cả các nút không phải nút hiện tại
            if (buttons[i] != GTK_WIDGET(button)) {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttons[i]), FALSE);
            }
        }
    }
}
// Hàm xử lý khi người dùng chọn ngày trong calendar
static void on_calendar_day_selected(GtkCalendar *calendar, gpointer user_data) {
    gchar date_str[11];  // Mảng chứa ngày theo định dạng mm/dd/yyyy
    guint year, month, day;
    GtkWidget *input_box = GTK_WIDGET(user_data);  // Ô nhập liệu (entry)

    // Lấy ngày đã chọn từ calendar
    gtk_calendar_get_date(calendar, &year, &month, &day);
    
    // Định dạng ngày theo kiểu mm/dd/yyyy
    g_snprintf(date_str, sizeof(date_str), "%02d/%02d/%04d", month + 1, day, year);
    
    // Cập nhật giá trị ngày vào ô nhập liệu
    gtk_entry_set_text(GTK_ENTRY(input_box), date_str);
    
    // Ẩn calendar sau khi chọn xong ngày
    gtk_widget_hide(GTK_WIDGET(calendar));
}
// Hàm xử lý khi người dùng nhấn vào ô nhập liệu
static gboolean on_input_box_focus_in(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    GtkWidget *calendar = GTK_WIDGET(user_data);

    // Hiển thị calendar khi người dùng nhấn vào ô nhập liệu
    gtk_widget_show(calendar);
    return FALSE;
}
static gboolean on_input_class_focus_in(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    GtkWidget *combo_box = GTK_WIDGET(user_data);

    // Hiển thị combo box khi người dùng nhấn vào ô nhập liệu
    gtk_widget_show(combo_box);
    return FALSE;
}

// Hàm xử lý khi người dùng chọn một lớp từ combo box
static void on_class_selected(GtkComboBox *combo_box, gpointer user_data) {
    const gchar *selected_class = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_box));
    GtkWidget *input_class = GTK_WIDGET(user_data);

    // Cập nhật ô nhập liệu với lớp đã chọn
    gtk_entry_set_text(GTK_ENTRY(input_class), selected_class);

    // Ẩn combo box sau khi chọn
    gtk_widget_hide(GTK_WIDGET(combo_box));
}
static void on_window_realize1(GtkWidget *widget, gpointer user_data) {
    GtkWidget *combo_box = GTK_WIDGET(user_data);
    gtk_widget_hide(combo_box);  // Ẩn combo box ngay khi cửa sổ được khởi tạo
}
// Hàm tạo header với logo và menu
GtkWidget* create_header(GtkWidget **buttons) {
    GtkWidget *header, *logo, *menu_box, *home_button, *ticket_button, *account_button;

    // Tạo một box chứa header
    header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(header, 1090, 50);

    // Đặt các thuộc tính của header
    gtk_widget_set_name(header, "header");
    gtk_widget_set_margin_top(header, 10);
    gtk_widget_set_margin_start(header, 20); // Thay gtk_widget_set_margin_left
    gtk_widget_set_margin_end(header, 330); // Thay gtk_widget_set_margin_right
    gtk_widget_set_margin_bottom(header, 10);

    // Tạo logo (bên trái của header)
    logo = gtk_image_new_from_file("assets/images/logo.png");
    gtk_box_pack_start(GTK_BOX(header), logo, FALSE, FALSE, 10);
    gtk_widget_set_margin_end(logo, 250); // Đặt margin-right cho logo là 300px

    // Tạo một box chứa menu
    menu_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_set_homogeneous(GTK_BOX(menu_box), TRUE);

    // Tạo các nút menu (đổi thành GtkToggleButton)
    home_button = gtk_toggle_button_new_with_label("Home");
    ticket_button = gtk_toggle_button_new_with_label("Ticket");
    account_button = gtk_toggle_button_new_with_label("Account");

    // Áp dụng CSS cho các nút
    GtkCssProvider *button_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(button_provider,
        "button {"
        "  font-family: 'Poppins', sans-serif;"
        "  font-size: 16px;"
        "  font-weight: 600;"
        "  letter-spacing: 0.06em;"
        "  background: transparent;"
        "  color: #283841;"
        "  padding: 10px 10px;"
        "  border-radius: 10px;"
        "  text-align: center;"
        "  border: none;"
        "  margin-right: 20px;"
        "}"
        "button:hover {"
        "  background: #223A60;"
        "  font-weight: 800;"
        "  cursor: pointer;"
        "  color: white;"
        "}"
        "button:active {"
        "  background: #223A60;"
        "  color: white;"
        "}"
        "button:checked {"
        "  background: #223A60;" 
        "  color: white;"
        // Giữ màu khi nút đã được nhấn
        "}",
        -1, NULL);

    // Áp dụng CSS cho các button
    gtk_style_context_add_provider(gtk_widget_get_style_context(home_button), GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(gtk_widget_get_style_context(ticket_button), GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(gtk_widget_get_style_context(account_button), GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Đặt kích thước cho các button
    gtk_widget_set_size_request(home_button, 100, 40);
    gtk_widget_set_size_request(ticket_button, 100, 40);
    gtk_widget_set_size_request(account_button, 100, 40);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(home_button), TRUE);

    // Thêm các nút vào menu_box
    gtk_box_pack_start(GTK_BOX(menu_box), home_button, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(menu_box), ticket_button, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(menu_box), account_button, TRUE, TRUE, 10);

    // Thêm menu_box vào header
    gtk_box_pack_start(GTK_BOX(header), menu_box, TRUE, TRUE, 0);

    // Gán các nút vào mảng buttons để có thể dùng cho nhóm toggle
    buttons[0] = home_button;
    buttons[1] = ticket_button;
    buttons[2] = account_button;
    buttons[3] = NULL;  // Kết thúc danh sách

    // Đăng ký sự kiện "toggled" cho các nút
    g_signal_connect(home_button, "toggled", G_CALLBACK(on_button_toggled), buttons);
    g_signal_connect(ticket_button, "toggled", G_CALLBACK(on_button_toggled), buttons);
    g_signal_connect(account_button, "toggled", G_CALLBACK(on_button_toggled), buttons);

    return header;
}

// Hàm tạo text nằm trên nền ảnh
GtkWidget* create_input_box(const gchar *placeholder) {
    GtkWidget *input_box = gtk_entry_new();  // Ô nhập văn bản
    gtk_widget_set_size_request(input_box, 240, 45);  // Kích thước ô
    gtk_entry_set_placeholder_text(GTK_ENTRY(input_box), placeholder);

    // Áp dụng CSS cho ô
    GtkCssProvider *input_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(input_provider,
        "entry {"
        "  font-family: 'DM Sans', sans-serif;"
        "  font-size: 16px;"
        "  font-weight: 500;"
        "  letter-spacing: 0.06em;"
        "  text-align: center;"
        "  border-radius: 6px;"
        "  padding: 12px 20px;"
        "  background-color: white;"
        "  border: 1px solid #ccc;"
        "  color: #283841;"
        "}",
        -1, NULL);

    gtk_style_context_add_provider(gtk_widget_get_style_context(input_box), GTK_STYLE_PROVIDER(input_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    return input_box;
}

// Hàm tạo ô nhập liệu với date picker (chọn ngày tháng)
GtkWidget* create_input_box_with_date_picker(const gchar *placeholder) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);  // Box chứa Entry và Calendar
    GtkWidget *input_box = create_input_box(placeholder);  // Sử dụng hàm tạo ô nhập liệu thông thường
    GtkWidget *calendar = gtk_calendar_new();  // Tạo calendar để chọn ngày

    // Thiết lập kích thước cho calendar
    gtk_widget_set_size_request(calendar, 240, 100);  // Điều chỉnh kích thước calendar

    // Thêm input_box và calendar vào box
    gtk_box_pack_start(GTK_BOX(box), input_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), calendar, FALSE, FALSE, 0);

    // Khi người dùng nhấn vào ô nhập liệu, calendar sẽ hiện lên
    g_signal_connect(input_box, "focus-in-event", G_CALLBACK(on_input_box_focus_in), calendar);

    // Khi người dùng chọn ngày, cập nhật vào ô nhập liệu
    g_signal_connect(calendar, "day-selected", G_CALLBACK(on_calendar_day_selected), input_box);
    // Ẩn calendar ngay sau khi cửa sổ được tạo xong
    g_signal_connect(gtk_widget_get_toplevel(calendar), "realize", G_CALLBACK(on_window_realize), calendar);
    return box;
}

// Hàm tạo các phần label text
GtkWidget* create_label(const gchar *text) {
    GtkWidget *label = gtk_label_new(text);
    // Áp dụng CSS cho label
    GtkCssProvider *label_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(label_provider,
        "label {"
        "  font-family: 'DM Sans', sans-serif;"
        "  font-size: 16px;"
        "  font-weight: 700;"
        "  letter-spacing: 0.06em;"
        "  text-align: center;"
        "  color: #283841;"
        "  height: 21px;"
        "  line-height: 20.83px;"
        "}",
        -1, NULL);

    gtk_style_context_add_provider(gtk_widget_get_style_context(label), GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    return label;
}
// Hàm tạo Selection Box theo yêu cầu (chia 5 phần)
GtkWidget* create_selection_box() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_widget_set_margin_top(box, 400);   // Thêm margin-top
    gtk_widget_set_margin_start(box, 30);  // Thêm margin-left
    gtk_widget_set_margin_end(box, 30);    // Thêm margin-right
    GtkCssProvider *box_provider = gtk_css_provider_new();
    // Chia thành 5 phần
    for (int i = 0; i < 5; i++) {
        GtkWidget *section_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
        gtk_widget_set_size_request(section_box, 240, -1); // Điều chỉnh chiều rộng cho mỗi ô

        if (i == 0) {
            GtkWidget *label_from = create_label("From");
            GtkWidget *input_from = create_input_box("   Where are you from?");

            GtkWidget *label_to = create_label("To");
            GtkWidget *input_to = create_input_box("   Where are you going?");

            gtk_box_pack_start(GTK_BOX(section_box), label_from, FALSE, FALSE, 0);
            gtk_box_pack_start(GTK_BOX(section_box), input_from, FALSE, FALSE, 0);
            gtk_box_pack_start(GTK_BOX(section_box), label_to, FALSE, FALSE, 0);
            gtk_box_pack_start(GTK_BOX(section_box), input_to, FALSE, FALSE, 0);
        }
        else if (i == 1) {
            GtkWidget *label_departure = create_label("Departure");
            GtkWidget *input_departure = create_input_box_with_date_picker("Choose Departure Date");

            GtkWidget *label_return = create_label("Return");
            GtkWidget *input_return = create_input_box_with_date_picker("+ Add Return Date (Optional)");

            // Thêm label và ô nhập vào box
            gtk_box_pack_start(GTK_BOX(section_box), label_departure, FALSE, FALSE, 0);
            gtk_box_pack_start(GTK_BOX(section_box), input_departure, FALSE, FALSE, 0);
            gtk_box_pack_start(GTK_BOX(section_box), label_return, FALSE, FALSE, 0);
            gtk_box_pack_start(GTK_BOX(section_box), input_return, FALSE, FALSE, 0);
        }
        else if (i == 2) {
            GtkWidget *label_traveller = create_label("Traveller");
            GtkWidget *input_traveller = create_input_box("Choose Traveller Number");

            gtk_box_pack_start(GTK_BOX(section_box), label_traveller, FALSE, FALSE, 0);
            gtk_box_pack_start(GTK_BOX(section_box), input_traveller, FALSE, FALSE, 0);
        }
        else if (i == 3) {
            GtkWidget *label_class = create_label("Class");

            // Tạo GtkComboBox cho "Class"
            GtkWidget *combo_box = gtk_combo_box_text_new();
            
            // Thêm mục "Choose Class" làm giá trị mặc định
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "Choose Class");
            
            // Thêm các lựa chọn khác
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "Economy");
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "Business");
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "First Class");

            // Set mục mặc định là "Choose Class"
            gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box), 0);  // Mục "Choose Class" ở chỉ số 0

            // Áp dụng CSS cho ComboBox giống như các ô nhập liệu
            GtkCssProvider *combo_provider = gtk_css_provider_new();
            gtk_css_provider_load_from_data(combo_provider,
                "combobox {"
                "  font-family: 'DM Sans', sans-serif;"
                "  font-size: 16px;"
                "  font-weight: 500;"
                "  letter-spacing: 0.06em;"
                "  text-align: center;"
                "  border-radius: 6px;"
                "  padding: 12px 20px;"
                "  background-color: white;"
                "  border: 1px solid #ccc;"
                "  color: #283841;"
                "}",          
                -1, NULL);

            gtk_style_context_add_provider(gtk_widget_get_style_context(combo_box), GTK_STYLE_PROVIDER(combo_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

            // Thêm label và combo box vào section_box
            gtk_box_pack_start(GTK_BOX(section_box), label_class, FALSE, FALSE, 0);
            gtk_box_pack_start(GTK_BOX(section_box), combo_box, FALSE, FALSE, 0);
        }
        else if (i == 4) {
            GtkWidget *search_button = gtk_button_new_with_label("Search Flight");
            gtk_widget_set_size_request(search_button, 192, 48);

            // Áp dụng CSS cho nút
            GtkCssProvider *search_provider = gtk_css_provider_new();
            gtk_css_provider_load_from_data(search_provider,
                "button {"
                "  font-family: 'Poppins', sans-serif;"
                "  font-size: 16px;"
                "  font-weight: 600;"
                "  letter-spacing: 0.06em;"
                "  color: white;"
                "  background: #223A60;"
                "  border-radius: 8px;"
                "  text-align: center;"
                "  padding: 16px;"
                "}",
                -1, NULL);

            gtk_style_context_add_provider(gtk_widget_get_style_context(search_button), GTK_STYLE_PROVIDER(search_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_widget_set_margin_top(search_button, 35);   // Thêm margin-top
            gtk_box_pack_start(GTK_BOX(section_box), search_button, FALSE, FALSE, 0);
        }

        // Thêm phần vào selection box
        gtk_box_pack_start(GTK_BOX(box), section_box, TRUE, TRUE, 0);
    }

    return box;
}

// Hàm chính tạo cửa sổ và thêm header
int main(int argc, char *argv[]) {
    GtkWidget *window, *header, *main_box, *overlay, *darea, *selection_box;

    // Khởi tạo GTK
    gtk_init(&argc, &argv);

    // Tạo cửa sổ mới
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Homepage");

    // Lấy kích thước màn hình và tránh vượt quá màn hình
    GdkDisplay *display = gdk_display_get_default();
    GdkMonitor *monitor = gdk_display_get_primary_monitor(display);
    GdkRectangle geometry;
    gdk_monitor_get_geometry(monitor, &geometry);
    gint screen_width = geometry.width;
    gint screen_height = geometry.height;

    // Đặt cửa sổ chiếm toàn bộ màn hình
    gtk_window_set_default_size(GTK_WINDOW(window), screen_width, screen_height);

    // Cấm thay đổi kích thước cửa sổ
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // Đăng ký sự kiện khi đóng cửa sổ
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

    // Tạo main container (dọc)
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Đảm bảo main_box có thể co giãn hoàn toàn theo chiều dọc
    gtk_widget_set_halign(main_box, GTK_ALIGN_FILL);
    gtk_widget_set_valign(main_box, GTK_ALIGN_FILL);

    // Tạo mảng lưu các nút trong header
    GtkWidget *buttons[4];

    // Tạo header
    header = create_header(buttons);
    gtk_box_pack_start(GTK_BOX(main_box), header, FALSE, FALSE, 0);

    // Tạo overlay
    overlay = gtk_overlay_new();
    gtk_box_pack_start(GTK_BOX(main_box), overlay, TRUE, TRUE, 0);

    // Đảm bảo overlay có thể co giãn
    gtk_widget_set_halign(overlay, GTK_ALIGN_FILL);
    gtk_widget_set_valign(overlay, GTK_ALIGN_FILL);

    // Tạo khu vực vẽ nền (background)
    darea = gtk_drawing_area_new();
    g_signal_connect(darea, "draw", G_CALLBACK(on_draw_event), NULL);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), darea);

    // Đảm bảo darea co giãn để phủ toàn bộ không gian cửa sổ
    gtk_widget_set_halign(darea, GTK_ALIGN_FILL);
    gtk_widget_set_valign(darea, GTK_ALIGN_FILL);

    // Tạo selection box và thêm vào overlay
    selection_box = create_selection_box();
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), selection_box);

    // Đảm bảo selection_box co giãn để phủ toàn bộ không gian cửa sổ
    gtk_widget_set_halign(selection_box, GTK_ALIGN_FILL);
    gtk_widget_set_valign(selection_box, GTK_ALIGN_FILL);

    // Hiển thị cửa sổ
    gtk_widget_show_all(window);

    // Bắt đầu vòng lặp sự kiện của GTK
    gtk_main();

    return 0;
}
