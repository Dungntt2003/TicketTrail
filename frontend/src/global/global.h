#ifndef GLOBAL_H
#define GLOBAL_H
#include <gtk/gtk.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>
#define MAX_LENGTH 1024
#define M_PI 3.14159265358979323846

extern GtkWidget *entry_email, *entry_password, *label_status, *window;
extern char buffer[MAX_LENGTH];
extern int sock;

#endif 