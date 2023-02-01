---
mytags: myblog
title: GTK With CSS
date: 2023-01-28 15:57:32
categories: [GTK]
tags: [入门]
---

本文主要介绍如何使用 CSS 对 GTK4 进行样式设计

<!-- more -->

GTK 各种CSS属性, 参考[网站 w3cub](https://docs.w3cub.com/gtk~4.0/ch39) 和[官方GTK3例子](https://docs.gtk.org/gtk3/css-overview.html)

main.c 
```c
#include <gtk/gtk.h>

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
    /* 加载 UI 界面文件 */
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "builder.ui", NULL);

    /* 从界面构造器中获取窗口, 并设置为当前应用的窗口 */
    GObject *window = gtk_builder_get_object(builder, "window");
    gtk_window_set_application(GTK_WINDOW(window), app);

    /* 为我们的按钮添加点击响应 */
    GObject *button = gtk_builder_get_object(builder, "button1");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

    button = gtk_builder_get_object(builder, "button2");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

    button = gtk_builder_get_object(builder, "quit");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(quit_cb), window);

    /* 加载 CSS 样式到整个窗口 */
    GdkDisplay *display = gtk_widget_get_display (GTK_WIDGET (window));
    GtkCssProvider *provider = gtk_css_provider_new ();
    gtk_css_provider_load_from_path(provider, "builder.css");
    gtk_style_context_add_provider_for_display (display, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_widget_show(GTK_WIDGET(window));

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
```

界面文件 builder.ui

```xml
<?xml version="1.0" encoding="UTF-8"?>
<interface>
    <object id="window" class="GtkWindow">
        <property name="title">Grid</property>
        <child>
            <object id="grid" class="GtkGrid">
                <child>
                    <object id="button1" class="GtkButton">
                        <property name="label">Button 1</property>
                        <property name="name">myButton</property>
                        <layout>
                            <property name="column">0</property>
                            <property name="row">0</property>
                        </layout>
                    </object>
                </child>
                <child>
                    <object id="button2" class="GtkButton">
                        <property name="label">Button 2</property>
                        <property name="name">myButton</property>
                        <property name="css-classes">myClass</property>
                        <layout>
                            <property name="column">1</property>
                            <property name="row">0</property>
                        </layout>
                    </object>
                </child>
                <child>
                    <object id="quit" class="GtkButton">
                        <property name="label">Quit</property>
                        <style>
                            <class name="myClass" />
                        </style>
                        <layout>
                            <property name="column">0</property>
                            <property name="row">1</property>
                            <property name="column-span">2</property>
                        </layout>
                    </object>
                </child>
            </object>
        </child>
    </object>
</interface>
```

builder.css

```css
button {
    border: 1px solid black;
}

#myButton {                  
    background: #669999;
    text-shadow: 1px 1px 5px black;
    box-shadow: 0px 0px 5px black;
    margin: 20px;
}

.myClass {
    color: red;   
}
```


编译

```sh
$ ls 
main.c builder.css builder.ui
$ gcc `pkg-config --cflags gtk4` main.c `pkg-config --libs gtk4`
```

GTK 的 CSS 有几种基本选择器，如下:

1. 标签选择器
   类似于 HTML 里面的通过标签进行筛选，包含 GTK 内建的各种小部件, 比如 window, button, label.
   标签值是通过 gtk_widget_class_set_css_name 进行设置的, 这个一般在构造函数中就设置好了。如果不是自定义的 widget 的话, GTK4 自带的控件就已经写好了，不需要更改:

   ```c
   gtk_widget_class_set_css_name (widget_class, "button");
   ```
   
   使用标签筛选器

    ```css
    button {
        border: 1px solid black;
    }
    ```
    
2. 类选择器
   要让选择器能够匹配到某一类 Object, 需要在 Object 中添加类属性, 可以直接通过源代码添加:

   ```c
   GtkStyleContext *context = gtk_widget_get_style_context(widget);
   gtk_style_context_add_class(context,"myClass");
   ```

   也可以在 ui 文件中直接添加:

   ```xml
   <object id="button2" class="GtkButton">
        <property name="label">Button 2</property>
        <style>
            <class name="myClass"/>
            <class name="myClass2"/>
            <class name="myClass3"/>
        </style>
   </object>
   ```
   另外, 注意到 `gtk_style_context_add_class` 是 add 也就是说和 HTML 类似的, 即一个元素可以同时添加多个类来描述

   当然如果使用的是 `property`, 同时加入多个 property class 并不能添加多个 class, 但是 property 和 style 组合使用也是可以叠加的, 所以可以先使用 `property` 作为主调, 然后 style 则是附加的修饰.
   ```xml
   <object id="button2" class="GtkButton">
       <property name="label">Button 2</property>
       <property name="css-classes">myClass</property>
   </object>
   ```

   ```xml
   <object id="button2" class="GtkButton">
       <property name="label">Button 2</property>
       <property name="css-classes">myClass</property>
       <style>
            <class name="myClass2"/>
            <class name="myClass3"/>
        </style>
   </object>
   ```

   类选择器使用 `.` 开头, 比如
    ```css
    .myClass {
        color: red;   
    }
    ```

3. id 选择器
   
   所谓的 id 就是 Object 的 name 属性, 可通过下面函数来设置 widget 的 name 属性

   ```
   gtk_widget_set_name
   ```

   或者在 ui 文件中直接添加
   
    ```xml
   <object id="button2" class="GtkButton">
       <property name="label">Button 2</property>
       <property name="name">myButton</property>
   </object>
   ```
    
    css 中的 id 定义好后也是可以同时给多个 Object 使用的, 只要 Object 添加对应的 name 属性即可.

    类选择器使用 `.` 开头, 比如
    ```css
    #myButton {                  
        background: #669999;
        text-shadow: 1px 1px 5px black;
        box-shadow: 0px 0px 5px black;
        margin: 20px;
    }
    ```

### 使用 gtk-builder-tools 预览 ui 文件

```shell
# --css 可选
gtk4-builder-tool preview builder.ui --css=builder.css
# 也可以使用 screenshot 来生成截图
gtk4-builder-tool screenshot builder.ui --css=builder.css --force
```

screenshot 真的很好用, 这样就可以一边写一边预览, 通过 VSCode 快捷键, 写的同时可以同步预览, 唯一比较遗憾的是截图的方式不能体验交互,比如 css 的 `hover` 需要实际运行程序才能知道.

### Tips

一个快速生成拟物风格的[网页](https://neumorphism.io/#e5cccc)

