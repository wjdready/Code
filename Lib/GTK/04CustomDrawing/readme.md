---
mytags: myblog
title: GTK Packing
date: 2023-01-21 21:06:02
categories: [GTK]
tags: [入门]
---

本文主要介绍 GTK 自定义绘制
<!-- more -->

许多小部件，如按钮，它们的所有绘图都是自己完成的。 你只需告诉他们你想要看到的标签，他们就会自动完成绘制按钮轮廓和聚焦矩形等。有时，有必要进行一些定制绘制。 在这种情况下，GtkDrawingArea 可能是合适的小部件。 它提供了一个画布，您可以通过设置其绘制功能在画布上绘制。

窗口小部件的内容通常需要部分或全部重新绘制，例如，当移动另一个窗口并揭示该窗口的一部分时，或者当包含该窗口的窗口被调整大小时。
还可以通过调用 gtk_widget_queue_draw () 显式地重新绘制小部件。 GTK 通过向 draw 函数提供现成的 cairo 上下文来处理大部分细节。

以下示例显示如何将绘制函数与 GtkDrawingArea 一起使用。它比前面的示例稍微复杂一些，因为它还演示了使用事件控制器处理输入事件。

main.c 
```c

```

编译
```
gcc `pkg-config --cflags gtk4` main.c `pkg-config --libs gtk4`
```

* cairo_t 

  cairo_t 包含 rendering device 的当前状态，包括尚未绘制的形状的坐标。
  cairo_t 上下文(即 cairo_t 已经命名的 cairo_t 对象) 是 cairo  的核心内容，
  所有使用 cairo 进行的绘制都是使用 cairo_t 对象完成的

