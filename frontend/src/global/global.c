#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
GtkWidget *entry_email, *entry_password, *label_status;
char buffer[MAX_LENGTH];
int sock;
GtkWidget *window;
GtkWidget *content_area;
const int airport_count;
Flight flights[MAX_LENGTH];
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

// Hàm tạo cửa sổ chính
GtkWidget* create_main_window() {
    // Tạo cửa sổ chính
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Register");

    // Đặt cửa sổ toàn màn hình
    gtk_window_fullscreen(GTK_WINDOW(window));

    // Kết nối tín hiệu đóng cửa sổ
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Tạo một CSS Provider để thiết lập nền
    GtkCssProvider *provider = gtk_css_provider_new();
    if (!gtk_css_provider_load_from_data(provider,
        "* { background-image: url('../assets/images/bg_login.png'); background-size: cover; background-position: center; }"
        "#register-box { background-color: #FFFFFF; border-radius: 20px; padding: 48px 72px; }"
        "#register-title { font-family: Arial, sans-serif; font-size: 28px; font-weight: bold; color: #101828; }",
        -1, NULL)) {
        g_print("Failed to load CSS\n");
    }

    // Áp dụng CSS cho cửa sổ chính
    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Tạo một container cho toàn bộ cửa sổ (sử dụng vbox để dễ dàng quản lý)
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Tạo content area để thay đổi khi cần
    content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(main_box), content_area, TRUE, TRUE, 0);

    return window;
}

// Hàm thay đổi phần nội dung của cửa sổ (giống như nội dung bên trong phần giữa)
void set_content(GtkWidget *new_content) {
    // Xóa nội dung cũ (nếu có)
    gtk_container_forall(GTK_CONTAINER(content_area), (GtkCallback)gtk_widget_destroy, NULL);

    // Thêm nội dung mới vào content_area
    gtk_box_pack_start(GTK_BOX(content_area), new_content, TRUE, TRUE, 0);

    // Hiển thị lại cửa sổ
    gtk_widget_show_all(window);
}

int is_number(const char *str) {
    char *endptr;
    strtol(str, &endptr, 10);  // Chuyển chuỗi thành số (hệ thập phân)

    // Kiểm tra xem chuỗi có phải là một số hợp lệ
    return *endptr == '\0';  // Nếu endptr trỏ đến ký tự kết thúc chuỗi, tức là chuỗi hợp lệ
}
