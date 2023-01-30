#include <gtk/gtk.h>

static void print_hello(GtkApplication *app)
{
    g_print("Hello World\n");

    /* 加载 UI 界面文件 */
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "builder.ui", NULL);

    /* 从界面构造器中获取窗口, 并设置到当前应用 */
    GObject *window = gtk_builder_get_object(builder, "window");
    gtk_window_set_application(GTK_WINDOW(window), app);

    /* 加载 CSS 样式到整个窗口 */
    GdkDisplay *display = gtk_widget_get_display (GTK_WIDGET (window));
    GtkCssProvider *provider = gtk_css_provider_new ();
    gtk_css_provider_load_from_path(provider, "builder.css");
    gtk_style_context_add_provider_for_display (display, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_widget_show(GTK_WIDGET(window));

    g_object_unref(builder);
}

static void quit_cb(GtkWindow *window)
{
    gtk_window_close(window);
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget * window2 = gtk_application_window_new(app);
    gtk_widget_show(GTK_WIDGET(window2));
    GtkWidget *button = gtk_button_new_with_label("refresh");
    // gtk_window_set_child(GTK_WINDOW(window2), button);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(print_hello), app);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_window_set_child(GTK_WINDOW(window2), box);
    gtk_box_append(GTK_BOX(box), button);

    /* 加载 UI 界面文件 */
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "builder.ui", NULL);

    /* 从界面构造器中获取窗口, 并设置到当前应用 */
    GObject *window = gtk_builder_get_object(builder, "window");
    gtk_window_set_application(GTK_WINDOW(window), app);

    /* 加载 CSS 样式到整个窗口 */
    GdkDisplay *display = gtk_widget_get_display (GTK_WIDGET (window));
    GtkCssProvider *provider = gtk_css_provider_new ();
    gtk_css_provider_load_from_path(provider, "builder.css");
    gtk_style_context_add_provider_for_display (display, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(builder);
}

int main(int argc, char *argv[])
{
    GtkApplication *app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
