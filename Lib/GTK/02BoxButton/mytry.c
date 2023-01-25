#include <gtk/gtk.h>

static void print_hello(GtkWidget *widget, gpointer data)
{
    if(data)
        g_print("%s\n", data);
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *box_v, *box_h;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 720, 480);

    box_v = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_halign(box_v, GTK_ALIGN_START);
    gtk_widget_set_valign(box_v, GTK_ALIGN_CENTER);
    
    box_h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_halign(box_h, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box_h, GTK_ALIGN_END);

    for (int i = 0; i < 10; i++)
    {
        char buf[32];
        snprintf(buf, 32, "Button%d", i);

        button = gtk_button_new_with_label(buf);
        g_signal_connect(button, "clicked", G_CALLBACK(print_hello), (void*)gtk_button_get_label(GTK_BUTTON(button)));
        gtk_box_append(GTK_BOX(box_v), button);

        button = gtk_button_new_with_label(buf);
        g_signal_connect(button, "clicked", G_CALLBACK(print_hello), (void*)gtk_button_get_label(GTK_BUTTON(button)));
        gtk_box_append(GTK_BOX(box_h), button);
    }

    /* window child 只能有一个? */
    gtk_window_set_child(GTK_WINDOW(window), box_v);
    // gtk_window_set_child(GTK_WINDOW(window), box_h);

    gtk_widget_show(window);
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
