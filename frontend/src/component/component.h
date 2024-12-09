#ifndef COMPONENT_H
#define COMPONENT_H
#include <gtk/gtk.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>

GtkWidget* create_header(GtkWidget **buttons);
static void on_button_toggled(GtkToggleButton *button, gpointer user_data);
#endif 