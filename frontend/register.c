#include <gtk/gtk.h>
#include <regex.h>
#include <string.h>

GtkWidget *entry_email, *entry_password, *entry_confirm_password, *entry_username, *entry_phone, *label_status;

void on_register_clicked(GtkWidget *widget, gpointer data) {
    const char *email = gtk_entry_get_text(GTK_ENTRY(entry_email));
    const char *password = gtk_entry_get_text(GTK_ENTRY(entry_password));
    const char *confirm_password = gtk_entry_get_text(GTK_ENTRY(entry_confirm_password));
    const char *username = gtk_entry_get_text(GTK_ENTRY(entry_username));
    const char *phone = gtk_entry_get_text(GTK_ENTRY(entry_phone));

    // Kiểm tra định dạng email
    regex_t regex;
    int reti = regcomp(&regex, "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$", REG_EXTENDED);
    if (reti) {
        gtk_label_set_text(GTK_LABEL(label_status), "Error compiling regex!");
        return;
    }
    reti = regexec(&regex, email, 0, NULL, 0);
    regfree(&regex);

    // Kiểm tra trường không rỗng
    if (strlen(email) == 0 || strlen(password) == 0 || strlen(confirm_password) == 0 || 
        strlen(username) == 0 || strlen(phone) == 0) {
        gtk_label_set_text(GTK_LABEL(label_status), "All fields are required!");
        return;
    }

    // Kiểm tra định dạng email
    if (reti) {
        gtk_label_set_text(GTK_LABEL(label_status), "Invalid email format!");
        return;
    }

    // Kiểm tra mật khẩu khớp
    if (strcmp(password, confirm_password) != 0) {
        gtk_label_set_text(GTK_LABEL(label_status), "Passwords do not match!");
        return;
    }

    // Kiểm tra mật khẩu đủ độ dài
    if (strlen(password) < 6) {
        gtk_label_set_text(GTK_LABEL(label_status), "Password must be at least 6 characters long!");
        return;
    }

    gtk_label_set_text(GTK_LABEL(label_status), "Registration successful!");
}

void on_login_clicked(GtkWidget *widget, gpointer data) {
    gtk_label_set_text(GTK_LABEL(label_status), "Log in button clicked!");
    // Logic để mở trang login nếu cần
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window, *overlay, *register_box, *label_register_title, *button_register, *hbox_footer;

    // Tạo cửa sổ chính
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Create an account");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "* { background-color: #050A24; }"
        "#register-box { background-color: #FFFFFF; border-radius: 20px; padding: 48px 72px; }"
        "#register-title { font-family: Poppins; font-size: 28px; font-weight: 600; color: #101828; background-color: transparent; }"
        "#email-label, #password-label, #confirm-password-label, #username-label, #phone-label { font-family: Poppins; font-size: 20px; font-weight: bold; color: #344054; background-color: transparent; }"
        "#email-entry, #password-entry, #confirm-password-entry, #username-entry, #phone-entry { border: 3px solid #D0D5DD; border-radius: 8px; padding: 12px 16px; color: #344054; background-color: #FFFFFF; }"
        "#register-button { color: #FCFCFD; border-radius: 8px; font-family: Poppins; font-weight: 600; font-size: 16px; background-color: #1570EF; }"
        "#register-button:hover { background-color: #125ECB; }"
        "#footer-text, #login-link { font-family: Poppins; font-size: 16px; font-weight: 400; color: #98A2B3; background-color: transparent; }"
        "#login-link { color: #1570EF; text-decoration: underline; }",
        -1, NULL);

    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Sử dụng Overlay để căn giữa register_box
    overlay = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(window), overlay);

    // Register box
    register_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 24);
    gtk_widget_set_name(register_box, "register-box");
    GtkStyleContext *register_box_context = gtk_widget_get_style_context(register_box);
    gtk_style_context_add_provider(register_box_context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), register_box);
    gtk_widget_set_halign(register_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(register_box, GTK_ALIGN_CENTER);

    // Tiêu đề
    label_register_title = gtk_label_new("Create an account");
    gtk_widget_set_name(label_register_title, "register-title");
    gtk_box_pack_start(GTK_BOX(register_box), label_register_title, FALSE, FALSE, 0);

    // Username label và input
    GtkWidget *label_username = gtk_label_new("Username");
    gtk_widget_set_name(label_username, "username-label");
    gtk_box_pack_start(GTK_BOX(register_box), label_username, FALSE, FALSE, 0);

    entry_username = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_username), "Enter your username");
    gtk_widget_set_name(entry_username, "username-entry");
    gtk_box_pack_start(GTK_BOX(register_box), entry_username, FALSE, FALSE, 0);

    // Phone label và input
    GtkWidget *label_phone = gtk_label_new("Phone Number");
    gtk_widget_set_name(label_phone, "phone-label");
    gtk_box_pack_start(GTK_BOX(register_box), label_phone, FALSE, FALSE, 0);

    entry_phone = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_phone), "Enter your phone number");
    gtk_widget_set_name(entry_phone, "phone-entry");
    gtk_box_pack_start(GTK_BOX(register_box), entry_phone, FALSE, FALSE, 0);

    // Email label và input
    GtkWidget *label_email = gtk_label_new("Email");
    gtk_widget_set_name(label_email, "email-label");
    gtk_box_pack_start(GTK_BOX(register_box), label_email, FALSE, FALSE, 0);

    entry_email = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_email), "Enter your email");
    gtk_widget_set_name(entry_email, "email-entry");
    gtk_box_pack_start(GTK_BOX(register_box), entry_email, FALSE, FALSE, 0);

    // Password label và input
    GtkWidget *label_password = gtk_label_new("Password");
    gtk_widget_set_name(label_password, "password-label");
    gtk_box_pack_start(GTK_BOX(register_box), label_password, FALSE, FALSE, 0);

    entry_password = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_password), "Enter your password");
    gtk_entry_set_visibility(GTK_ENTRY(entry_password), FALSE);
    gtk_widget_set_name(entry_password, "password-entry");
    gtk_box_pack_start(GTK_BOX(register_box), entry_password, FALSE, FALSE, 0);

    // Confirm Password label và input
    GtkWidget *label_confirm_password = gtk_label_new("Confirm Password");
    gtk_widget_set_name(label_confirm_password, "confirm-password-label");
    gtk_box_pack_start(GTK_BOX(register_box), label_confirm_password, FALSE, FALSE, 0);

    entry_confirm_password = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_confirm_password), "Re-enter your password");
    gtk_entry_set_visibility(GTK_ENTRY(entry_confirm_password), FALSE);
    gtk_widget_set_name(entry_confirm_password, "confirm-password-entry");
    gtk_box_pack_start(GTK_BOX(register_box), entry_confirm_password, FALSE, FALSE, 0);

    // Nút đăng ký
    button_register = gtk_button_new_with_label("Create account");
    gtk_widget_set_name(button_register, "register-button");
    g_signal_connect(button_register, "clicked", G_CALLBACK(on_register_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(register_box), button_register, FALSE, FALSE, 0);

    // Footer
    hbox_footer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    GtkWidget *footer_text = gtk_label_new("Already have an account?");
    gtk_widget_set_name(footer_text, "footer-text");
    gtk_box_pack_start(GTK_BOX(hbox_footer), footer_text, FALSE, FALSE, 0);

    GtkWidget *login_button = gtk_button_new_with_label("Log in");
    gtk_widget_set_name(login_button, "login-link");
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox_footer), login_button, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(register_box), hbox_footer, FALSE, FALSE, 0);

    // Label trạng thái
    label_status = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(register_box), label_status, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}