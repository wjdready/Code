---
mytags: myblog
title: GTK Building user interfaces
date: 2023-01-27 19:02:44
categories: [GTK]
tags: [入门]
---


本文主要介绍如何使用 XML 构建用户界面
<!-- more -->

当构建一个具有数十或数百个小部件的更复杂的用户界面时，用C代码完成所有设置工作都很麻烦，而且几乎不可能进行更改。
值得庆幸的是，GTK通过使用可由GtkBuilder类解析的XML格式的UI描述，支持将用户界面布局与业务逻辑分离。

main.c 
```c
#include <gtk/gtk.h>
#include <glib/gstdio.h>

static void print_hello(GtkWidget *widget, gpointer data)
{
    g_print("Hello World\n");
}

static void quit_cb(GtkWindow *window)
{
    gtk_window_close(window);
}

static void activate(GtkApplication *app, gpointer user_data)
{
    /* Construct a GtkBuilder instance and load our UI description */
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "builder.ui", NULL);

    /* Connect signal handlers to the constructed widgets. */
    GObject *window = gtk_builder_get_object(builder, "window");
    gtk_window_set_application(GTK_WINDOW(window), app);

    GObject *button = gtk_builder_get_object(builder, "button1");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

    button = gtk_builder_get_object(builder, "button2");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

    button = gtk_builder_get_object(builder, "quit");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(quit_cb), window);

    gtk_widget_show(GTK_WIDGET(window));

    /* We do not need the builder any more */
    g_object_unref(builder);
}

int main(int argc, char *argv[])
{
#ifdef GTK_SRCDIR
    g_chdir(GTK_SRCDIR);
#endif

    GtkApplication *app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
```

编译

```
gcc `pkg-config --cflags gtk4` main.c `pkg-config --libs gtk4`
```

请注意， GtkBuilder 还可用于构造非小部件的对象，如树模型、调整等。这就是我们在此处使用的方法称为Gtk_Builder_Get_Object()的原因，它返回GObject而不是GtkWidget。

通常，您会将完整路径传递给gtk_Builder_Add_from_file()，以使程序的执行独立于当前目录。
安装UI描述和类似数据的常见位置是/usr/share/appname。
还可以将UI描述作为字符串嵌入到源代码中，并使用gtk_Builder_Add_from_string()来加载它。

但是，将UI描述保存在单独的文件中有几个好处：
无需重新编译程序即可对用户界面进行细微调整。
将用户界面代码与应用程序的业务逻辑隔离更容易。
使用复合小部件模板可以更轻松地将您的UI重构为单独的类。

使用GResource可以将两者结合在一起：您可以在源代码存储库中单独保存UI定义文件，然后将它们嵌入到您的应用程序中。

```xml
<?xml version="1.0" encoding="UTF-8"?>
<gresources>
  <gresource prefix="/org/gtk/exampleapp">
    <file preprocess="xml-stripblanks">window.ui</file>
  </gresource>
</gresources>
```