---
mytags: myblog
title: GTK Packing
date: 2023-01-21 21:06:02
categories: [GTK]
tags: [入门]
---

本文主要介绍 GTK 容器布局
<!-- more -->

main.c 
```c
#include <gtk/gtk.h>

static void print_hello(GtkWidget *widget, gpointer data)
{
    g_print("Hello World\n");
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;

    /* 创建新窗口，并设置其标题 */
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Window");

    /* 在这里我们构造了一个容器，它将包装我们的按钮 */
    grid = gtk_grid_new();

    /* 把容器装在 window 内 */
    gtk_window_set_child(GTK_WINDOW(window), grid);

    /* 将第一个按钮放在网格单元格(0，0)中，并使其在水平和垂直方向上仅填充1个单元格(即 no spanning) */
    button = gtk_button_new_with_label("Button 1");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

    /* 将第二个按钮放置在网格单元格(1，0)中，并使其仅水平和垂直填充1个单元格(即 no spanning) */
    button = gtk_button_new_with_label("Button 2");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);

    /* 将Quit按钮放置在网格单元(0，1)中，并使其跨过(span)2列。 */
    button = gtk_button_new_with_label("Quit");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), window);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 1);

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
```

编译
```
gcc `pkg-config --cflags gtk4` main.c `pkg-config --libs gtk4`
```
