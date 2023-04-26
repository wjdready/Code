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
    const char *src;
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

    // lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
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

    else if(code == LV_EVENT_DRAW_POST) 
    {
        draw_main(e);
    }
}


void print_area(const char *text, lv_area_t *a)
{
    LV_LOG_WARN("%s: (%d, %d) -- (%d, %d)", text, a->x1, a->y1, a->x2, a->y2);
}

/* 绘制主要部分 */
static void draw_main(lv_event_t * e)
{   
    lv_obj_t * obj = lv_event_get_target(e);
    uw_demo_t * demo = (uw_demo_t *)obj;
    lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);
    lv_obj_t *label = lv_obj_get_child(obj, 0);

    /* 获取样式属性 */
    lv_coord_t border_width = lv_obj_get_style_border_width(obj, LV_PART_MAIN);
    lv_coord_t left = lv_obj_get_style_pad_left(obj, LV_PART_MAIN) + border_width;
    lv_coord_t right = lv_obj_get_style_pad_right(obj, LV_PART_MAIN) + border_width;
    lv_coord_t top = lv_obj_get_style_pad_top(obj, LV_PART_MAIN) + border_width;

    /* 从 obj 中提取 label 描述符 */
    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    lv_obj_init_draw_label_dsc(label, LV_PART_MAIN, &label_dsc);

    /* 根据 label 描述符计算 text 占的大小 */
    lv_point_t size;
    lv_txt_get_size(&size, demo->text, label_dsc.font, label_dsc.letter_space, label_dsc.line_space, LV_COORD_MAX, label_dsc.flag);

    /* 在对象上绘制标签 */
    lv_area_t txt_area;
    txt_area.y1 = obj->coords.y1 + top + 150;
    txt_area.y2 = txt_area.y1 + size.y;

    txt_area.x1 = obj->coords.x2 - right - size.x;
    txt_area.x2 = txt_area.x1 + size.x;

    // txt_area.x1 = obj->coords.x1 + left;
    // txt_area.x2 = txt_area.x1 + size.x;

    lv_coord_t font_h = lv_font_get_line_height(label_dsc.font);
    lv_coord_t line_space = lv_obj_get_style_text_line_space(obj, 0);


    lv_area_t area_sel;
    area_sel.y1 = label->coords.y1;
    area_sel.y1 += 6 * (font_h + line_space);
    area_sel.y1 -= line_space / 2;

    area_sel.y2 = area_sel.y1 + font_h + line_space - 1;
    area_sel.x1 = obj->coords.x1;
    area_sel.x2 = obj->coords.x2;

    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.border_width = 1;
    rect_dsc.border_opa = LV_OPA_100;
    lv_draw_rect(draw_ctx, &rect_dsc, &area_sel);

    lv_area_t mask_sel;
    bool area_ok;
    /* 获取两个区域的公共部分 */
    area_ok = _lv_area_intersect(&mask_sel, draw_ctx->clip_area, &area_sel);
    print_area("draw_ctx", &draw_ctx->clip_area);
    print_area("area_sel", &txt_area);
    print_area("label->coords", &label->coords);
    LV_LOG_WARN("\n\n");
    // if (area_ok)
    // {
        const lv_area_t *clip_area_ori = draw_ctx->clip_area;

        /* obj 上下文中只绘制指定部分 */
        // draw_ctx->clip_area = &mask_sel;
        lv_draw_label(draw_ctx, &label_dsc, &label->coords, demo->text, NULL);
        // LV_LOG_WARN(">> %s", lv_label_get_text(label));
        // draw_ctx->clip_area = clip_area_ori;
    // }
}

void uw_demo_set_text(lv_obj_t *obj, const char *text)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    LV_ASSERT_NULL(text);

    uw_demo_t * demo = (uw_demo_t *)obj;
    demo->text = text;
}

void uw_demo_set_src(lv_obj_t *obj, const void *src)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    LV_ASSERT_NULL(src);

    uw_demo_t * demo = (uw_demo_t *)obj;
    demo->src = src;
}

