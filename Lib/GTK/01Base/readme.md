---
mytags: myblog
title: GTK Base
date: 2023-01-21 16:20:15
categories: [GTK]
tags: [入门]
---

本文主要介绍
<!-- more -->


GTK 是一个小部件 (Widget) 工具包.  GTK 创建的每个用户界面都由小部件组成. 这是使用 GObject 实现的，GObject 是 C 的一个面向对象的框架.  

其中，窗口 (window) 小部件一般被作为主容器. 然后，通过向窗口添加按钮、下拉菜单、输入字段和其他小部件来构建用户界面. 

如果你正在打算创建一个复杂的用户界面，建议使用 GtkBuilder 及其特定于的 GTK 的标记描述语言，而不是手动组装界面. 

GTK 是事件驱动的. 该工具包侦听事件，如点击按钮，并将事件传递给您的应用程序. 

为了开始我们对 GTK 的介绍，我们将从一个非常简单的应用程序开始. 该程序将创建一个空的 200×200 像素的窗口. 

main.c 
```c
#include <gtk/gtk.h>

static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
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

所有 GTK 应用程序都将包括 gtk/gtk.h, 它声明 GTK 应用程序所需的函数、类型和宏. 即使 GTK 安装了多个头文件，第三方代码也只能直接包含顶级 gtk/gtk.h 头文件. 如果直接包含任何其他标头，则编译器将中止并返回错误. 

在 GTK 应用程序中，main () 函数的作用是创建一个 GtkApplication 对象并运行它. 在本例中，声明了一个名为 app 的 GtkApplication 指针，然后使用 gtk_application_new () 对其进行初始化. 

在创建 GtkApplication 时，您需要选择一个应用程序标识符 (名称), 并将其作为参数传递给 gtk_application_new () . 在本例中，使用了 org.gtk.Example

最后，GApplicationFlages 提供了应用程序功能标志. 其枚举值如下:
```c
typedef enum
{
  G_APPLICATION_FLAGS_NONE GLIB_DEPRECATED_ENUMERATOR_IN_2_74_FOR (G_APPLICATION_DEFAULT_FLAGS),
  G_APPLICATION_DEFAULT_FLAGS GLIB_AVAILABLE_ENUMERATOR_IN_2_74 = 0,
  G_APPLICATION_IS_SERVICE  =          (1 << 0),
  G_APPLICATION_IS_LAUNCHER =          (1 << 1),

  G_APPLICATION_HANDLES_OPEN =         (1 << 2),
  G_APPLICATION_HANDLES_COMMAND_LINE = (1 << 3),
  G_APPLICATION_SEND_ENVIRONMENT    =  (1 << 4),

  G_APPLICATION_NON_UNIQUE =           (1 << 5),

  G_APPLICATION_CAN_OVERRIDE_APP_ID =  (1 << 6),
  G_APPLICATION_ALLOW_REPLACEMENT   =  (1 << 7),
  G_APPLICATION_REPLACE             =  (1 << 8)
} GApplicationFlags;
```

接下来，将激活 (activate) 信号连接到 main () 函数上方的 activate () 函数. 当您的应用程序在下面的行中使用 g_application_run 启动时，将发出激活信号. g_application_run () 调用还将命令行参数 (argc 和 argv 字符串数组) 作为参数. 您的应用程序可以覆盖命令行处理，例如打开在命令行上传递的文件. 

在 g_application_run () 中发送激活信号，然后进入应用程序的 activate () 函数. 这是我们构造 GTK 窗口的地方，以便在启动应用程序时显示一个窗口. 调用 gtk_application_window_new () 将创建一个新的 GtkApplicationWindow 并返回创建的窗口指针，根据平台的不同，窗口将具有 frame、标题栏 ( title bar) 和窗口控制 (window controls) 等功能属性.

使用 gtk_window_set_title () 设置窗口标题. 此函数接受 GtkWindow 指针和字符串作为输入. 因为我们的窗口指针是 GtkWidget 指针，所以我们需要将其强制转换为 GtkWindow；不是通过典型的 C 强制转换 (GtkWindow*) 来转换窗口，而是使用宏 GTK_WINDOW () 来转换窗口. GTK_WINDOW () 将在强制转换之前检查指针是否为 GtkWindow 类的实例，如果检查失败则发出警告. 


最后，使用 gtk_window_set_default_size () 设置窗口大小，然后 GTK 通过 gtk_widget_show () 显示窗口. 


当您关闭窗口时，例如，通过按下 [X] 按钮，g_application_run () 调用返回一个数字，该数字保存在一个名为 Status 的整型变量中. 然后，使用 g_Object_unref () 从内存中释放 GtkApplication 对象. 最后，返回状态整数并退出应用程序. 

