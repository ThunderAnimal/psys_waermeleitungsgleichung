#include <gtk/gtk.h>

void print_msg(char *message) {

    printf("%s\n",message);
}

void start_proc(GtkWidget *widget, gpointer window) {

    print_msg("START PROC");
    char path[1035];
    FILE *fp = popen("../serial/head_conduction 0000.1 15 0.25 1", "r");
    if (fp == NULL) {
        print_msg("Failed to run command");
        return;
    }

    /* Read the output a line at a time - output it. */
    while (fgets(path, sizeof(path)-1, fp) != NULL) {
        printf("%s", path);
    }

    /* close */
    pclose(fp);
}


void end_window(GtkWidget * widget, gpointer daten){

}

int main(int argc, char *argv[]) {

    //WINDOW
    GtkWidget *window;

    //CONTAINERS
    GtkWidget *halign;

    //USER INPUT WIDGETS
    GtkWidget *button;

    //INT GTK
    gtk_init(&argc, &argv);

    //SET UP - Window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Head Conduction");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 480);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    //SET UP - Button
    button = gtk_button_new_with_label("Click me!");
    gtk_widget_set_tooltip_text(button, "Try me!!!!");

    //SET UP LAYOUT - CONTAINERS
    halign = gtk_alignment_new(0, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(halign), button);
    gtk_container_add(GTK_CONTAINER(window), halign);

    //Define Events
    g_signal_connect(button, "clicked",
                     G_CALLBACK(start_proc), NULL);

    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    //Start UI
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}