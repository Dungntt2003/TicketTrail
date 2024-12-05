#include <gtk/gtk.h>
#include "homepage.h"
#include <pango/pango.h>
#include <pango/pangocairo.h>
GtkWidget *homepage_window ;
static void on_window_realize(GtkWidget *widget, gpointer user_data) {
    GtkWidget *calendar = GTK_WIDGET(user_data);
    gtk_widget_hide(calendar);  
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    GdkPixbuf *pixbuf;
    GdkPixbuf *scaled_pixbuf;

    pixbuf = gdk_pixbuf_new_from_file("../assets/images/homepage.png", NULL);
    if (!pixbuf) {
        g_print("Error loading image!\n");
        return FALSE;
    }

    gint width = gtk_widget_get_allocated_width(widget);
    gint height = gtk_widget_get_allocated_height(widget);
    scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, width, height, GDK_INTERP_BILINEAR);

    gdk_cairo_set_source_pixbuf(cr, scaled_pixbuf, 0, 0);
    cairo_paint(cr);

    g_object_unref(pixbuf);
    g_object_unref(scaled_pixbuf);

    return FALSE;
}

static void on_window_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

static void on_button_toggled(GtkToggleButton *button, gpointer user_data) {
    GtkWidget **buttons = (GtkWidget **)user_data;

    
    if (gtk_toggle_button_get_active(button)) {
        for (int i = 0; buttons[i] != NULL; i++) {
            
            if (buttons[i] != GTK_WIDGET(button)) {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttons[i]), FALSE);
            }
        }
    }
}

static void on_calendar_day_selected(GtkCalendar *calendar, gpointer user_data) {
    gchar date_str[11];  
    guint year, month, day;
    GtkWidget *input_box = GTK_WIDGET(user_data);  

    
    gtk_calendar_get_date(calendar, &year, &month, &day);
    
    
    g_snprintf(date_str, sizeof(date_str), "%02d/%02d/%04d", month + 1, day, year);
    
    
    gtk_entry_set_text(GTK_ENTRY(input_box), date_str);
    
    
    gtk_widget_hide(GTK_WIDGET(calendar));
}

static gboolean on_input_box_focus_in(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    GtkWidget *calendar = GTK_WIDGET(user_data);

    
    gtk_widget_show(calendar);
    return FALSE;
}
static gboolean on_input_class_focus_in(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    GtkWidget *combo_box = GTK_WIDGET(user_data);

    
    gtk_widget_show(combo_box);
    return FALSE;
}


static void on_class_selected(GtkComboBox *combo_box, gpointer user_data) {
    const gchar *selected_class = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_box));
    GtkWidget *input_class = GTK_WIDGET(user_data);

    
    gtk_entry_set_text(GTK_ENTRY(input_class), selected_class);

    
    gtk_widget_hide(GTK_WIDGET(combo_box));
}
static void on_window_realize1(GtkWidget *widget, gpointer user_data) {
    GtkWidget *combo_box = GTK_WIDGET(user_data);
    gtk_widget_hide(combo_box);  
}

GtkWidget* create_header(GtkWidget **buttons) {
    GtkWidget *header, *logo, *menu_box, *home_button, *ticket_button, *account_button;

    
    header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(header, 1090, 50);

    
    gtk_widget_set_name(header, "header");
    gtk_widget_set_margin_top(header, 10);
    gtk_widget_set_margin_start(header, 20); 
    gtk_widget_set_margin_end(header, 330); 
    gtk_widget_set_margin_bottom(header, 10);

    
    logo = gtk_image_new_from_file("../assets/images/logo.png");
    gtk_box_pack_start(GTK_BOX(header), logo, FALSE, FALSE, 10);
    gtk_widget_set_margin_end(logo, 250); 

    
    menu_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_set_homogeneous(GTK_BOX(menu_box), TRUE);

    
    home_button = gtk_toggle_button_new_with_label("Home");
    ticket_button = gtk_toggle_button_new_with_label("Ticket");
    account_button = gtk_toggle_button_new_with_label("Account");

    
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
        
        "}",
        -1, NULL);

    
    gtk_style_context_add_provider(gtk_widget_get_style_context(home_button), GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(gtk_widget_get_style_context(ticket_button), GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(gtk_widget_get_style_context(account_button), GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    
    gtk_widget_set_size_request(home_button, 100, 40);
    gtk_widget_set_size_request(ticket_button, 100, 40);
    gtk_widget_set_size_request(account_button, 100, 40);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(home_button), TRUE);

    
    gtk_box_pack_start(GTK_BOX(menu_box), home_button, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(menu_box), ticket_button, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(menu_box), account_button, TRUE, TRUE, 10);

    
    gtk_box_pack_start(GTK_BOX(header), menu_box, TRUE, TRUE, 0);

    
    buttons[0] = home_button;
    buttons[1] = ticket_button;
    buttons[2] = account_button;
    buttons[3] = NULL;  

    
    g_signal_connect(home_button, "toggled", G_CALLBACK(on_button_toggled), buttons);
    g_signal_connect(ticket_button, "toggled", G_CALLBACK(on_button_toggled), buttons);
    g_signal_connect(account_button, "toggled", G_CALLBACK(on_button_toggled), buttons);

    return header;
}


GtkWidget* create_input_box(const gchar *placeholder) {
    GtkWidget *input_box = gtk_entry_new();  
    gtk_widget_set_size_request(input_box, 240, 45);  
    gtk_entry_set_placeholder_text(GTK_ENTRY(input_box), placeholder);

    
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


GtkWidget* create_input_box_with_date_picker(const gchar *placeholder) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);  
    GtkWidget *input_box = create_input_box(placeholder);  
    GtkWidget *calendar = gtk_calendar_new();  

    
    gtk_widget_set_size_request(calendar, 240, 100);  

    
    gtk_box_pack_start(GTK_BOX(box), input_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), calendar, FALSE, FALSE, 0);

    
    g_signal_connect(input_box, "focus-in-event", G_CALLBACK(on_input_box_focus_in), calendar);

    
    g_signal_connect(calendar, "day-selected", G_CALLBACK(on_calendar_day_selected), input_box);
    
    g_signal_connect(gtk_widget_get_toplevel(calendar), "realize", G_CALLBACK(on_window_realize), calendar);
    return box;
}


GtkWidget* create_label(const gchar *text) {
    GtkWidget *label = gtk_label_new(text);
    
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

GtkWidget* create_selection_box() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_widget_set_margin_top(box, 400);   
    gtk_widget_set_margin_start(box, 30);  
    gtk_widget_set_margin_end(box, 30);    
    GtkCssProvider *box_provider = gtk_css_provider_new();
    
    for (int i = 0; i < 5; i++) {
        GtkWidget *section_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
        gtk_widget_set_size_request(section_box, 240, -1); 

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

            
            GtkWidget *combo_box = gtk_combo_box_text_new();
            
            
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "Choose Class");
            
            
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "Economy");
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "Business");
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "First Class");

            
            gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box), 0);  

            
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

            
            gtk_box_pack_start(GTK_BOX(section_box), label_class, FALSE, FALSE, 0);
            gtk_box_pack_start(GTK_BOX(section_box), combo_box, FALSE, FALSE, 0);
        }
        else if (i == 4) {
            GtkWidget *search_button = gtk_button_new_with_label("Search Flight");
            gtk_widget_set_size_request(search_button, 192, 48);

            
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
            gtk_widget_set_margin_top(search_button, 35);   
            gtk_box_pack_start(GTK_BOX(section_box), search_button, FALSE, FALSE, 0);
        }

        
        gtk_box_pack_start(GTK_BOX(box), section_box, TRUE, TRUE, 0);
    }

    return box;
}


GtkWidget* create_homepage_window() {
    GtkWidget *window, *header, *main_box, *overlay, *darea, *selection_box;

    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Homepage");

    
    GdkDisplay *display = gdk_display_get_default();
    GdkMonitor *monitor = gdk_display_get_primary_monitor(display);
    GdkRectangle geometry;
    gdk_monitor_get_geometry(monitor, &geometry);
    gint screen_width = geometry.width;
    gint screen_height = geometry.height;

    
    gtk_window_set_default_size(GTK_WINDOW(window), screen_width, screen_height);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

    
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    gtk_widget_set_halign(main_box, GTK_ALIGN_FILL);
    gtk_widget_set_valign(main_box, GTK_ALIGN_FILL);

    
    GtkWidget *buttons[4];
    header = create_header(buttons);
    gtk_box_pack_start(GTK_BOX(main_box), header, FALSE, FALSE, 0);

    
    overlay = gtk_overlay_new();
    gtk_box_pack_start(GTK_BOX(main_box), overlay, TRUE, TRUE, 0);
    gtk_widget_set_halign(overlay, GTK_ALIGN_FILL);
    gtk_widget_set_valign(overlay, GTK_ALIGN_FILL);

    
    darea = gtk_drawing_area_new();
    g_signal_connect(darea, "draw", G_CALLBACK(on_draw_event), NULL);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), darea);
    gtk_widget_set_halign(darea, GTK_ALIGN_FILL);
    gtk_widget_set_valign(darea, GTK_ALIGN_FILL);

    
    selection_box = create_selection_box();
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), selection_box);
    gtk_widget_set_halign(selection_box, GTK_ALIGN_FILL);
    gtk_widget_set_valign(selection_box, GTK_ALIGN_FILL);

    
    gtk_widget_show_all(window);

    return window;
}


void create_homepage_widget () {
    homepage_window = create_homepage_window();
    gtk_main();

}