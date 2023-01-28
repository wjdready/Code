---
mytags: myblog
title: GTK Building user interfaces
date: 2023-01-27 19:13:00
categories: [GTK]
tags: [入门]
---

本文将从头开始构建一个简单的应用程序，随着时间的推移添加越来越多的部分。
<!-- more -->

### 第一步: 一个简单的程序
main.c 
```c
#include <gtk/gtk.h>

#include "exampleapp.h"

int main (int argc, char *argv[])
{
    return g_application_run (G_APPLICATION (example_app_new ()), argc, argv);
}
```
所有应用程序逻辑都在应用程序类中，它是 GtkApplication 的子类。
我们的示例还没有任何有趣的功能。它所做的一切就是在没有参数的情况下激活时打开一个窗口，如果它是带参数启动的，则打开给它的文件。
为了处理这两种情况，我们覆盖了 activate() vfunc 和 open 虚函数，前者在没有命令行参数的情况下启动应用程序时调用，后者在使用命令行参数启动应用程序时调用。
要了解有关GApplication入口点的更多信息，请参考GIO文档。

```

```
编译

```
gcc `pkg-config --cflags gtk4` main.c `pkg-config --libs gtk4`
```
