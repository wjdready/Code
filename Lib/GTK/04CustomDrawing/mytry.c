#include <gtk/gtk.h>

/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;

/* 清除 surface 为白色 */
static void clear_surface(void)
{
    cairo_t *cr;

    cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_destroy(cr);
}

/* 创建一个适当大小的新 surface 来存储我们的涂鸦 */
static void resize_cb(GtkWidget *widget, int width, int height, gpointer data)
{
    if (surface)
    {
        cairo_surface_destroy(surface);
        surface = NULL;
    }

    if (gtk_native_get_surface(gtk_widget_get_native(widget)))
    {
        surface = gdk_surface_create_similar_surface(gtk_native_get_surface(gtk_widget_get_native(widget)),
            CAIRO_CONTENT_COLOR, gtk_widget_get_width(widget), gtk_widget_get_height(widget));

        /* 初始化 surface 为白色 */
        clear_surface();
    }
}

/**
 * 从表面重新绘制屏幕。
 * 请注意，DRAW回调接收一个随时可用的 cairo_t，
 * 它已经被裁剪为仅绘制小部件的暴露区域
 */
static void draw_cb(GtkDrawingArea *drawing_area, cairo_t *cr, int width, int height, gpointer data)
{
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);
}

/* 在 surface 上的给定位置绘制图形 */
static void draw_brush(GtkWidget *widget, double x, double y)
{
    cairo_t *cr;

    /* 在我们存储状态的表面上涂抹颜料 */
    cr = cairo_create(surface);

    int r = 32;
    cairo_arc(cr, x - r / 2, y -  r / 2 , r, 0, 3.1415926);
    cairo_fill(cr);

    cairo_destroy(cr);

    /* 现在，使绘图区域无效。 */
    gtk_widget_queue_draw(widget);
}

static double start_x;
static double start_y;

/* 鼠标右击开始 */
static void drag_begin(GtkGestureDrag *gesture, double x, double y, GtkWidget *area)
{
    start_x = x;
    start_y = y;

    g_print("drag_begin (%.1f, %.1f)\n", x, y);

    draw_brush(area, x, y);
}

/* 按下并移动鼠标过程 */
static void drag_update(GtkGestureDrag *gesture, double x, double y, GtkWidget *area)
{
    g_print("drag_update (%.1f, %.1f)\n", x, y);

    draw_brush(area, start_x + x, start_y + y);
}

/* 结束动作 */
static void drag_end(GtkGestureDrag *gesture, double x, double y, GtkWidget *area)
{
    g_print("drag_end (%.1f, %.1f)\n", x, y);

    draw_brush(area, start_x + x, start_y + y);
}

/* 鼠标右键按下回调 */
static void pressed(GtkGestureClick *gesture, int n_press, double x, double y, GtkWidget *area)
{
    clear_surface();

    /* 重新绘制区域 */
    gtk_widget_queue_draw(area);
}

/* 关闭窗口 */
static void close_window(void)
{
    if (surface)
        cairo_surface_destroy(surface);
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *frame;
    GtkWidget *drawing_area;
    GtkGesture *drag;
    GtkGesture *press;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Drawing Area");

    g_signal_connect(window, "destroy", G_CALLBACK(close_window), NULL);

    frame = gtk_frame_new(NULL);
    gtk_window_set_child(GTK_WINDOW(window), frame);

    drawing_area = gtk_drawing_area_new();
    /* set a minimum size */
    gtk_widget_set_size_request(drawing_area, 100, 100);

    gtk_frame_set_child(GTK_FRAME(frame), drawing_area);

    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), draw_cb, NULL, NULL);

    g_signal_connect_after(drawing_area, "resize", G_CALLBACK(resize_cb), NULL);

    /* 创建手势控制器-鼠标拖动设置*/
    drag = gtk_gesture_drag_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(drag), GDK_BUTTON_PRIMARY);
    gtk_widget_add_controller(drawing_area, GTK_EVENT_CONTROLLER(drag));
    g_signal_connect(drag, "drag-begin", G_CALLBACK(drag_begin), drawing_area);
    g_signal_connect(drag, "drag-update", G_CALLBACK(drag_update), drawing_area);
    g_signal_connect(drag, "drag-end", G_CALLBACK(drag_end), drawing_area);

    /* 创建手势控制器-鼠标按下 */
    press = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(press), GDK_BUTTON_SECONDARY);
    gtk_widget_add_controller(drawing_area, GTK_EVENT_CONTROLLER(press));

    g_signal_connect(press, "pressed", G_CALLBACK(pressed), drawing_area);

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
