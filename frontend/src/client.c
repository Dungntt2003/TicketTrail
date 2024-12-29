#include <gtk/gtk.h>
#include "booklist/booklist.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Booklist");
    gtk_window_fullscreen(GTK_WINDOW(window));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *booklist_screen = create_booklist_window();
    gtk_container_add(GTK_CONTAINER(window), booklist_screen);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
