#include <gtk/gtk.h>
#include "component.h"
#include <pango/pango.h>
#include <pango/pangocairo.h>
#include "../global/global.h"
#include "../homepage/homepage.h"
void on_home_button_clicked(GtkWidget *widget, gpointer data) {
    if (widget == NULL) {
        g_print("Error: widget is NULL\n");
        return;
    }
    g_print("Home button clicked!\n");
    GtkWidget *homepage_widget = create_homepage_window();
    set_content(homepage_widget);
}

static void on_button_toggled(GtkToggleButton *button, gpointer user_data) {
    GtkWidget **buttons = (GtkWidget **)user_data;
    
    if (buttons == NULL) {
        g_print("Error: buttons array is NULL\n");
        return;
    }
    
    if (gtk_toggle_button_get_active(button)) {
        for (int i = 0; buttons[i] != NULL; i++) {
            if (buttons[i] != GTK_WIDGET(button)) {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttons[i]), FALSE);
            }
        }
    }
}

GtkWidget* create_header(GtkWidget **buttons) {
    GtkWidget *header, *logo, *menu_box, *home_button, *ticket_button, *account_button;

    
    header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(header, 1060, 40);

    
    gtk_widget_set_name(header, "header");
    gtk_widget_set_margin_top(header, 10);
    gtk_widget_set_margin_start(header, 20); 
    gtk_widget_set_margin_end(header, 280); 
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
    g_signal_connect(home_button, "clicked", G_CALLBACK(on_home_button_clicked), NULL);

    return header;
}

