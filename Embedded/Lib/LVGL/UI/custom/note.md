

# LVGL 8 自定义控件方法

要定义控件只需定义一个 lv_obj_class_t 对象, 然后通过 lv_obj_class_create_obj 来完成创建.

比如:

```c
typedef struct
{
    lv_obj_t obj;
    const char *text;
} uw_demo_t;


const lv_obj_class_t uw_demo_class = {
    .constructor_cb = uw_demo_constructor,
    .destructor_cb  = uw_demo_destructor,
    .event_cb       = uw_demo_event,
    .width_def      = LV_DPI_DEF,
    .height_def     = LV_SIZE_CONTENT,
    .instance_size  = sizeof(uw_demo_t),
    .editable       = LV_OBJ_CLASS_EDITABLE_TRUE,
    .group_def      = LV_OBJ_CLASS_GROUP_DEF_TRUE,
    .base_class     = &lv_obj_class
};
```

其中 

* constructor_cb 是构造控件时调用的构造函数, 主要完成自定义控件中自定义成员的初始化
* destructor_cb 是删除控件时调用的析构函数, 用来释放自定义成员所占用的内存
* event_cb 用于自定义地处理各种事件, 若为空，则默认调用父类的事件处理函数，若不为空则可在 event_cb 再次调用父类事件处理或者忽视父类事件处理函数
* width_def/height_def 定义控件默认宽度/高度, LV_DPI_DEF 是一个像素大小?, LV_SIZE_CONTENT 为自适应孩子大小，但设置的这个在创建完对象后使用
  lv_obj_set_size(); 后将被覆盖
* instance_size 实例大小, 包括了父类 + 自定义内容的大小
* editable 是否可编辑
* group_def 组定义
* base_class 继承自该类

lv_obj_class_t 基本上就是这些内容.

配合 lv_obj_class_xxx_obj 相关函数，我们可以完成对象的创建和初始化等工作:

```c
lv_obj_t *uw_demo_create(lv_obj_t *parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t *obj = lv_obj_class_create_obj(&uw_demo_class, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}
```

lv_obj_class_create_obj 会申请内存并创建对象，创建完成后通过 lv_obj_class_init_obj 来完成对对象的初始化，

可以看以下该函数主要做了哪些事情:

```c
void lv_obj_class_init_obj(lv_obj_t * obj)
{
    lv_obj_mark_layout_as_dirty(obj);
    lv_obj_enable_style_refresh(false);

    lv_theme_apply(obj);
    lv_obj_construct(obj);

    lv_obj_enable_style_refresh(true);
    lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);

    lv_obj_refresh_self_size(obj);

    lv_group_t * def_group = lv_group_get_default();
    if(def_group && lv_obj_is_group_def(obj)) {
        lv_group_add_obj(def_group, obj);
    }

    lv_obj_t * parent = lv_obj_get_parent(obj);
    if(parent) {
        /*Call the ancestor's event handler to the parent to notify it about the new child.
         *Also triggers layout update*/
        lv_event_send(parent, LV_EVENT_CHILD_CHANGED, obj);
        lv_event_send(parent, LV_EVENT_CHILD_CREATED, obj);

        /*Invalidate the area if not screen created*/
        lv_obj_invalidate(obj);
    }
}
```

可以看到 lv_obj_construct(obj); 说明这时对象结构体中定义的构造函数会被调用

下面是完整的 uw_demo.c

```c
#include "uw_demo.h"

#define MY_CLASS &uw_demo_class

static void uw_demo_constructor(const lv_obj_class_t *class_p, lv_obj_t *obj);
static void uw_demo_destructor(const lv_obj_class_t *class_p, lv_obj_t *obj);
static void uw_demo_event(const lv_obj_class_t *class_p, lv_event_t *e);
static void draw_main(lv_event_t * e);

typedef struct
{
    lv_obj_t obj;
    const char *text;
} uw_demo_t;

const lv_obj_class_t uw_demo_class = {
    .constructor_cb = uw_demo_constructor,
    .destructor_cb  = uw_demo_destructor,
    .event_cb       = uw_demo_event,
    .width_def      = LV_DPI_DEF,
    .height_def     = LV_SIZE_CONTENT,
    .instance_size  = sizeof(uw_demo_t),
    .editable       = LV_OBJ_CLASS_EDITABLE_TRUE,
    .group_def      = LV_OBJ_CLASS_GROUP_DEF_TRUE,
    .base_class     = &lv_obj_class
};

lv_obj_t *uw_demo_create(lv_obj_t *parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t *obj = lv_obj_class_create_obj(&uw_demo_class, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

/**
 * @brief 构造函数, 执行 lv_obj_class_init_obj 时会调用该函数
 *
 * @param class_p
 * @param obj
 */
static void uw_demo_constructor(const lv_obj_class_t *class_p, lv_obj_t *obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
}

/**
 * @brief 析构函数
 *
 * @param class_p
 * @param obj
 */
static void uw_demo_destructor(const lv_obj_class_t *class_p, lv_obj_t *obj)
{
}

/**
 * @brief 事件处理函数
 *
 * @param class_p
 * @param e
 */
static void uw_demo_event(const lv_obj_class_t *class_p, lv_event_t *e)
{
    lv_res_t res;

    /* 调用父对象事件处理函数 */
    res = lv_obj_event_base(MY_CLASS, e);
    if (res != LV_RES_OK)
        return;

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    uw_demo_t * demo = (uw_demo_t *)obj;
    
    if(code == LV_EVENT_PRESSED)
    {
        if(demo->text)
        {
            LV_LOG_WARN("%s", demo->text);
        }
    }

    else if(code == LV_EVENT_DRAW_MAIN) 
    {
        draw_main(e);
    }
}


/* 绘制主要部分 */
static void draw_main(lv_event_t * e)
{   
    lv_obj_t * obj = lv_event_get_target(e);
    uw_demo_t * demo = (uw_demo_t *)obj;
    lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);

    /* 获取样式属性 */
    lv_coord_t border_width = lv_obj_get_style_border_width(obj, LV_PART_MAIN);
    lv_coord_t left = lv_obj_get_style_pad_left(obj, LV_PART_MAIN) + border_width;
    lv_coord_t right = lv_obj_get_style_pad_right(obj, LV_PART_MAIN) + border_width;
    lv_coord_t top = lv_obj_get_style_pad_top(obj, LV_PART_MAIN) + border_width;

    /* 从 obj 中提取 label 描述符 */
    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    lv_obj_init_draw_label_dsc(obj, LV_PART_MAIN, &label_dsc);

    /* 根据 label 描述符计算 text 占的大小 */
    lv_point_t size;
    lv_txt_get_size(&size, demo->text, label_dsc.font, label_dsc.letter_space, label_dsc.line_space, LV_COORD_MAX, label_dsc.flag);

    /* 在对象上绘制标签 */
    lv_area_t txt_area;
    txt_area.y1 = obj->coords.y1 + top;
    txt_area.y2 = txt_area.y1 + size.y;

    txt_area.x1 = obj->coords.x2 - right - size.x;
    txt_area.x2 = txt_area.x1 + size.x;

    // txt_area.x1 = obj->coords.x1 + left;
    // txt_area.x2 = txt_area.x1 + size.x;

    lv_draw_label(draw_ctx, &label_dsc, &txt_area, demo->text, NULL);
}

void uw_demo_set_text(lv_obj_t *obj, const char *text)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    LV_ASSERT_NULL(text);

    uw_demo_t * demo = (uw_demo_t *)obj;
    demo->text = text;
}
```

使用 ui_demo 控件

```c
lv_obj_t *obj = uw_demo_create(lv_scr_act());
lv_obj_set_style_bg_color(obj, lv_color_make(255, 0, 0), 0);
lv_obj_set_style_bg_color(obj, lv_color_make(0, 0, 255), LV_STATE_PRESSED);
lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);
lv_obj_set_style_border_width(obj, 1, 0);
lv_obj_set_style_border_color(obj, lv_color_make(255, 0, 0), 0);
lv_obj_set_style_border_opa(obj, LV_OPA_100, 0);
lv_obj_set_style_radius(obj, 10, 0);

lv_obj_set_size(obj, 300, 200);
uw_demo_set_text(obj, "Hello World, I am demo!\n");
```

## 控件的绘制

可以看到控件的绘制主要在 draw_main 中完成，该函数是响应了 LV_EVENT_DRAW_MAIN 函数, 通过 lv_draw_xxx 系列函数我么可以绘制出多种图形.

当然, 在创建对象后为对象设置事件回调中也可以通过处理 LV_EVENT_DRAW_MAIN 来完成绘制，但这只对该对象有效。

这个用在在个人定义的一些图表上很方便啊，只需控件提供基本的框架和绘图区域即可自我发挥了。

下面将在 draw_main 中分别进行练习各种绘制:

### 1. lv_draw_label 绘制标签

```c
static void draw_main(lv_event_t * e)
{   
    lv_obj_t * obj = lv_event_get_target(e);
    uw_demo_t * demo = (uw_demo_t *)obj;
    lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);

    /* 获取样式属性 */
    lv_coord_t border_width = lv_obj_get_style_border_width(obj, LV_PART_MAIN);
    lv_coord_t left = lv_obj_get_style_pad_left(obj, LV_PART_MAIN) + border_width;
    lv_coord_t right = lv_obj_get_style_pad_right(obj, LV_PART_MAIN) + border_width;
    lv_coord_t top = lv_obj_get_style_pad_top(obj, LV_PART_MAIN) + border_width;

    /* 从 obj 中提取 label 描述符 */
    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    lv_obj_init_draw_label_dsc(obj, LV_PART_MAIN, &label_dsc);

    /* 根据 label 描述符计算 text 占的大小 */
    lv_point_t size;
    lv_txt_get_size(&size, demo->text, label_dsc.font, label_dsc.letter_space, label_dsc.line_space, LV_COORD_MAX, label_dsc.flag);

    /* 在对象上绘制标签 */
    lv_area_t txt_area;
    txt_area.y1 = obj->coords.y1 + top;
    txt_area.y2 = txt_area.y1 + size.y;

    txt_area.x1 = obj->coords.x2 - right - size.x;
    txt_area.x2 = txt_area.x1 + size.x;

    // txt_area.x1 = obj->coords.x1 + left;
    // txt_area.x2 = txt_area.x1 + size.x;

    lv_draw_label(draw_ctx, &label_dsc, &txt_area, demo->text, NULL);
}
```

### 2. lv_draw_line 绘制线条

```c
/* 绘制主要部分 */
static void draw_main(lv_event_t * e)
{   
    lv_obj_t * obj = lv_event_get_target(e);

    /* 获取对象所在屏幕的区域, 也可以直接通过 obj->coords.y1 访问 */
    lv_area_t area;
    lv_obj_get_coords(obj, &area);

    /* 从对象中提取线条属性 */
    lv_draw_line_dsc_t line_dsc;
    lv_draw_line_dsc_init(&line_dsc);
    lv_obj_init_draw_line_dsc(obj, LV_PART_MAIN, &line_dsc);

    lv_coord_t w = lv_obj_get_width(obj);
    lv_coord_t h = lv_obj_get_height(obj);

    /* 获取绘制上下文 */
    lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);
    lv_point_t p1, p2;

    /* 绘制两条对角线 */
    p1.x = area.x1;
    p1.y = area.y1;
    p2.x = area.x2;
    p2.y = area.y2;
    lv_draw_line(draw_ctx, &line_dsc, &p1, &p2);

    p1.x = area.x1;
    p1.y = area.y2;
    p2.x = area.x2;
    p2.y = area.y1;
    lv_draw_line(draw_ctx, &line_dsc, &p1, &p2);
    
    /* 绘制纵横交叉的网格 */
    p1.x = area.x1;
    p1.y = area.y2;
    p2.x = area.x1;
    p2.y = area.y1;
    for (int i = 0; p1.x < area.x2; i++)
    {
        p1.x += 10;
        p2.x += 10;
        lv_draw_line(draw_ctx, &line_dsc, &p1, &p2);
    }

    p1.x = area.x1;
    p1.y = area.y1;
    p2.x = area.x2;
    p2.y = area.y1;
    for (int i = 0; p1.y < area.y2; i++)
    {
        p1.y += 10;
        p2.y += 10;
        lv_draw_line(draw_ctx, &line_dsc, &p1, &p2);
    }

    /* 绘制对角网格 */
    p1.x = area.x1;
    p1.y = area.y1;
    p2.x = area.x1;
    p2.y = area.y2;
    int inc_x = w * 10 / h;
    for (int i = 0; p1.y < area.y2; i++)
    {
        p1.y += 10;
        p2.x += inc_x;
        lv_draw_line(draw_ctx, &line_dsc, &p1, &p2);
    }

    p1.x = area.x1;
    p1.y = area.y1;
    p2.x = area.x2;
    p2.y = area.y1;
    for (int i = 0; p2.y < area.y2; i++)
    {
        p1.x += inc_x;
        p2.y += 10;
        lv_draw_line(draw_ctx, &line_dsc, &p1, &p2);
    }

    /* 绘制正弦信号 */
    p1.x = area.x1;
    p1.y = area.y2 - 100;
    p2.x = area.x1;
    p2.y = area.y2 - 100;
    float th = 0.1;

    for (int i = 0; p1.x < area.x2; i++)
    {
        p2.y = area.y2 - sinf(th)*50 - 100;
        th += 0.2;
        p2.x += 1;
        lv_draw_line(draw_ctx, &line_dsc, &p1, &p2);
        p1 = p2;
    }
}
```

可以看到除了常规参数: 上下文和属性描述,  lv_draw_line 参数主要是两个坐标点用于连成一条线

从 lv_obj_init_draw_arc_dsc 中可提取出 lv_obj_set_style_arc_xxx 的相关属性, 然后赋值给 arc_dsc。

然后使用控件时通过 lv_obj_set_style_line_color 和 lv_obj_set_style_line_width 可设置线条颜色和宽度等属性

```c
lv_obj_t *obj = uw_demo_create(lv_scr_act());
lv_obj_set_style_line_color(obj, lv_color_make(0, 0, 0), 0);
lv_obj_set_style_line_width(obj, 1, 0);
lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
```

### 3. lv_draw_arc 绘制圆弧

```c
static void get_center(const lv_obj_t * obj, lv_point_t * center, lv_coord_t * arc_r)
{
    lv_coord_t left_bg = lv_obj_get_style_pad_left(obj, LV_PART_MAIN);
    lv_coord_t right_bg = lv_obj_get_style_pad_right(obj, LV_PART_MAIN);
    lv_coord_t top_bg = lv_obj_get_style_pad_top(obj, LV_PART_MAIN);
    lv_coord_t bottom_bg = lv_obj_get_style_pad_bottom(obj, LV_PART_MAIN);

    lv_coord_t r = (LV_MIN(lv_obj_get_width(obj) - left_bg - right_bg,
                           lv_obj_get_height(obj) - top_bg - bottom_bg)) / 2;

    center->x = obj->coords.x1 + r + left_bg;
    center->y = obj->coords.y1 + r + top_bg;

    if(arc_r) *arc_r = r;
}

/* 绘制主要部分 */
static void draw_main(lv_event_t * e)
{   
    lv_obj_t * obj = lv_event_get_target(e);
    lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);

    /* 获取对象所在屏幕的区域, 也可以直接通过 obj->coords.y1 访问 */
    lv_area_t area;
    lv_obj_get_coords(obj, &area);
    
    lv_point_t center;
    lv_coord_t arc_r;
    get_center(obj, &center, &arc_r);

    /* 获取对象的属性描述 */
    lv_draw_arc_dsc_t arc_dsc;
    lv_draw_arc_dsc_init(&arc_dsc);
    lv_obj_init_draw_arc_dsc(obj, LV_PART_MAIN, &arc_dsc);

    lv_draw_arc(draw_ctx, &arc_dsc, &center, arc_r, 0, 360);
}
```

可以看到 lv_draw_arc 参数主要是中心坐标点和、起始角度、结束角度

从 lv_obj_init_draw_arc_dsc 中可提取出 lv_obj_set_style_arc_xxx 的相关属性, 然后赋值给 arc_dsc。

然后使用控件时通过 lv_obj_set_style_arc_color 和 lv_obj_set_style_arc_width 可设置线条颜色和宽度等属性

```c
lv_obj_t *obj = uw_demo_create(lv_scr_act());
lv_obj_set_style_arc_color(obj, lv_color_make(0, 0, 0), 0);
lv_obj_set_style_arc_width(obj, 1, 0);
lv_obj_set_size(obj, 100, 100);
```

### 4. lv_draw_rect 绘制矩形

```c
static void draw_main(lv_event_t * e)
{   
    lv_obj_t * obj = lv_event_get_target(e);
    lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);

    /* 获取对象所在屏幕的区域, 也可以直接通过 obj->coords.y1 访问 */
    lv_area_t area;
    lv_obj_get_coords(obj, &area);
    
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &rect_dsc);
    rect_dsc.radius = 5;

    lv_area_t rect_area;
    rect_area.x1 = area.x1 + 5;
    rect_area.x2 = area.x2 - 5;
    rect_area.y1 = area.y1 + 5;
    rect_area.y2 = area.y2 - 5;

    lv_draw_rect(draw_ctx, &rect_dsc, &rect_area);
}
```
lv_obj_init_draw_rect_dsc 函数主要是提取了以下属性

```c
draw_dsc->bg_opa = (opa * draw_dsc->shadow_opa) >> 8;
draw_dsc->bg_img_opa = (opa * draw_dsc->shadow_opa) >> 8;
draw_dsc->border_opa = (opa * draw_dsc->shadow_opa) >> 8;
draw_dsc->outline_opa = (opa * draw_dsc->shadow_opa) >> 8;
draw_dsc->shadow_opa = (opa * draw_dsc->shadow_opa) >> 8;
```

因此要设置矩形宽度颜色等，可设置:

```c
lv_obj_t *obj = uw_demo_create(lv_scr_act());
lv_obj_set_style_bg_color(obj, lv_color_make(255, 0, 0), 0);
lv_obj_set_style_bg_opa(obj, LV_OPA_50, 0);
lv_obj_set_style_border_color(obj, lv_color_make(0, 0, 0), 0);
lv_obj_set_style_border_width(obj, 1, 0);
lv_obj_set_size(obj, 100, 100);
```

但这会带来一个问题, 那就是父类 obj 的外框也画了出来, 当然我们可以选择忽视不绘制父类, 即取消父类的事件处理, `lv_obj_event_base(MY_CLASS, e);` 

还有另一种方法是采用 PART 的方式, 即该矩形的属性不属于 LV_PART_MAIN, 这样在提取样式属性可以通过特定的 PART 进行提取，避免了和 LV_PART_MAIN 冲突.

### 5. lv_draw_polygon 多边形

```c
/* 绘制主要部分 */
static void draw_main(lv_event_t * e)
{   
    lv_obj_t * obj = lv_event_get_target(e);
    lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);

    /* 获取对象所在屏幕的区域, 也可以直接通过 obj->coords.y1 访问 */
    lv_area_t area;
    lv_obj_get_coords(obj, &area);
    
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &rect_dsc);

    lv_point_t points[3];

    points[0].x = area.x1 + 5;
    points[0].y = area.y1 + 5;
    
    points[1].x = area.x2 - 5;
    points[1].y = area.y1 + 5;

    points[2].x = area.x1 + 5 + lv_obj_get_width(obj) / 2;
    points[2].y = area.y2 - 5;

    lv_draw_polygon(draw_ctx, &rect_dsc, points, 3);
}
```

多边形的 dsc 和 rect 一样, 因此设置对象样式也基本一致.

### 6. lv_draw_img 绘制图形

图形资源通过 void * 来索引，分为3类: 字面量 SYMBOL、图片的文件路径、lvgl 二进制图形的内存地址

```c
/* 绘制主要部分 */
static void draw_main(lv_event_t * e)
{   
    lv_obj_t * obj = lv_event_get_target(e);
    lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);

    uw_demo_t *demo = (uw_demo_t *)obj;
    lv_img_src_t src_type = lv_img_src_get_type(demo->src);

    if(demo->src == NULL)
        return ;

    /* 图片 src 是 symbol 类型 */
    if(src_type == LV_IMG_SRC_SYMBOL)
    {
        lv_draw_label_dsc_t label_dsc;
        lv_draw_label_dsc_init(&label_dsc);
        lv_obj_init_draw_label_dsc(obj, LV_PART_MAIN, &label_dsc);

        lv_point_t size;
        lv_txt_get_size(&size, demo->src, label_dsc.font, label_dsc.letter_space, label_dsc.line_space, LV_COORD_MAX, label_dsc.flag);

        /* 设置绘制区域为正中心 */
        lv_area_t draw_area;
        draw_area.x1 = obj->coords.x1 + (obj->coords.x2 - obj->coords.x1 - size.x) / 2;
        draw_area.x2 = draw_area.x1 + size.x;
        draw_area.y1 = obj->coords.y1 + (obj->coords.y2 - obj->coords.y1 - size.y) / 2;
        draw_area.y2 = draw_area.y1 + size.y;
        lv_draw_label(draw_ctx, &label_dsc, &draw_area, demo->src, NULL);
    }

    /* 图片 src 是 二进制类型 */
    else if (src_type == LV_IMG_SRC_VARIABLE)
    {
        lv_draw_img_dsc_t img_dsc;
        lv_draw_img_dsc_init(&img_dsc);
        lv_obj_init_draw_img_dsc(obj, LV_PART_MAIN, &img_dsc);
        
        /* 获取图片大小信息 */
        lv_img_header_t header;
        lv_img_decoder_get_info(demo->src, &header);

        /* 放在中心 */
        lv_area_t draw_area;
        draw_area.x1 = obj->coords.x1 + (obj->coords.x2 - obj->coords.x1 - header.w) / 2;
        draw_area.x2 = draw_area.x1 + header.w - 1;
        draw_area.y1 = obj->coords.y1 + (obj->coords.y2 - obj->coords.y1 - header.h) / 2;
        draw_area.y2 = draw_area.y1 + header.h - 1;

        /* 设置旋转属性 */
        img_dsc.pivot.x = (header.w - 1) / 2;
        img_dsc.pivot.y = (header.h - 1) / 2;
        img_dsc.angle = lv_obj_get_style_transform_angle(obj, LV_PART_MAIN);

        lv_draw_img(draw_ctx, &img_dsc, &draw_area, demo->src);
    }
}
```

测试用例如下:

```c
lv_obj_t *obj = uw_demo_create(lv_scr_act());
lv_obj_set_style_bg_color(obj, lv_color_make(255, 0, 0), 0);
lv_obj_set_style_bg_opa(obj, LV_OPA_50, 0);

lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);

lv_obj_set_style_text_color(obj, lv_color_make(0, 0, 0), 0);
lv_obj_set_style_text_opa(obj, LV_OPA_COVER, 0);

/* 更改图片颜色 */
lv_obj_set_style_img_recolor(obj, lv_color_make(0, 0, 255), 0);
lv_obj_set_style_img_recolor_opa(obj, LV_OPA_COVER, 0);

LV_IMG_DECLARE(img_cogwheel_argb);

uw_demo_set_text(obj, "Hello I am demo!");
uw_demo_set_src(obj, &img_cogwheel_argb);
// uw_demo_set_src(obj, LV_SYMBOL_AUDIO);

lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
```

### 7. lv_draw_layer 绘制图层

图层的概念该怎么理解和使用? 这个还没弄懂，下面代码有 bug:

```c
/* 绘制主要部分 */
static void draw_main(lv_event_t * e)
{   
    lv_obj_t * obj = lv_event_get_target(e);
    lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);
    uw_demo_t *demo = (uw_demo_t *)obj;

    lv_draw_img_dsc_t draw_dsc;
    lv_draw_img_dsc_init(&draw_dsc);
    lv_obj_init_draw_img_dsc(obj, LV_PART_MAIN, &draw_dsc);

    /* 获取图片大小信息 */
    lv_img_header_t header;
    lv_img_decoder_get_info(demo->src, &header);

    /* 放在中心 */
    lv_area_t draw_area;
    draw_area.x1 = obj->coords.x1 + (obj->coords.x2 - obj->coords.x1 - header.w) / 2;
    draw_area.x2 = draw_area.x1 + header.w - 1;
    draw_area.y1 = obj->coords.y1 + (obj->coords.y2 - obj->coords.y1 - header.h) / 2;
    draw_area.y2 = draw_area.y1 + header.h - 1;

    /* 创建一个图层 */
    lv_draw_layer_ctx_t * layer_ctx = lv_draw_layer_create(draw_ctx, &draw_area, LV_DRAW_LAYER_FLAG_HAS_ALPHA);

    /* 图层混合 */
    lv_draw_layer_blend(draw_ctx, layer_ctx, &draw_dsc);
    
    lv_draw_layer_destroy(draw_ctx, layer_ctx);
}
```
### 8. lv_draw_mask 蒙板

蒙板没找到例程, 但是 canvas 提供了绘图功能，

```c
void lv_example_canvas_1(void)
{
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.radius = 10;
    rect_dsc.bg_opa = LV_OPA_COVER;
    rect_dsc.bg_grad.dir = LV_GRAD_DIR_HOR;
    rect_dsc.bg_grad.stops[0].color = lv_palette_main(LV_PALETTE_RED);
    rect_dsc.bg_grad.stops[1].color = lv_palette_main(LV_PALETTE_BLUE);
    rect_dsc.border_width = 2;
    rect_dsc.border_opa = LV_OPA_90;
    rect_dsc.border_color = lv_color_white();
    rect_dsc.shadow_width = 5;
    rect_dsc.shadow_ofs_x = 5;
    rect_dsc.shadow_ofs_y = 5;

    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.color = lv_palette_main(LV_PALETTE_ORANGE);

    static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];

    lv_obj_t * canvas = lv_canvas_create(lv_scr_act());
    lv_canvas_set_buffer(canvas, cbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
    lv_obj_center(canvas);
    lv_canvas_fill_bg(canvas, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_OPA_COVER);

    lv_canvas_draw_rect(canvas, 70, 60, 100, 70, &rect_dsc);

    lv_canvas_draw_text(canvas, 40, 20, 100, &label_dsc, "Some text on text canvas");

    /*Test the rotation. It requires another buffer where the original image is stored.
     *So copy the current image to buffer and rotate it to the canvas*/
    static lv_color_t cbuf_tmp[CANVAS_WIDTH * CANVAS_HEIGHT];
    memcpy(cbuf_tmp, cbuf, sizeof(cbuf_tmp));
    lv_img_dsc_t img;
    img.data = (void *)cbuf_tmp;
    img.header.cf = LV_IMG_CF_TRUE_COLOR;
    img.header.w = CANVAS_WIDTH;
    img.header.h = CANVAS_HEIGHT;

    lv_canvas_fill_bg(canvas, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_OPA_COVER);
    lv_canvas_transform(canvas, &img, 120, LV_IMG_ZOOM_NONE, 0, 0, CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, true);
}

```

### 9. lv_draw_transform 变换

无

## LV_EVENT_DRAW_MAIN 和 LV_EVENT_DRAW_POST 的区别


LV_EVENT_DRAW_MAIN 是绘制主要部分，LV_EVENT_DRAW_POST 是绘制控件的内容。

当主要内容绘制完成后才会调用 LV_EVENT_DRAW_POST, 先绘制父亲的 MAIN 再绘制孩子的 MAIN, 当所有父子层级 MAIN 绘制完后才开始绘制 LV_EVENT_DRAW_POST 

# 自带控件解析


## lv_dropdown 


