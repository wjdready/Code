---
mytags: myblog
title: GTK BoxButton
date: 2023-01-21 20:18:55
categories: [GTK]
tags: [入门]
---
本文主要介绍布局容器

<!-- more -->

在创建应用程序时，您可能希望在一个窗口中放置多个小部件。 这样做时，控制每个小部件的位置和大小就变得很重要。 这就是组装(Packing)的用武之地。

GTK附带了各种布局容器，它们的目的是控制添加到其中的子小部件的布局，例如：

* GtkBox
* GtkGrid
* GtkRevealer
* GtkStack
* GtkOverlay
* GtkPaned
* GtkExpander
* GtkFixed

main.c

```c
#include <gtk/gtk.h>

static void print_hello (GtkWidget *widget, gpointer data)
{
    g_print ("Hello World\n");
}

static void activate (GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *box;

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Window");
    gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

    box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign (box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (box, GTK_ALIGN_CENTER);

    gtk_window_set_child (GTK_WINDOW (window), box);

    button = gtk_button_new_with_label ("Hello World");

    g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_window_destroy), window);

    gtk_box_append (GTK_BOX (box), button);

    gtk_widget_show (window);
}

int main (int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
```

编译

```
gcc `pkg-config --cflags gtk4` main.c `pkg-config --libs gtk4`
```
