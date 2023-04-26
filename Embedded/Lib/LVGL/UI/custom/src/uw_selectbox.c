/**
 * @file uw_selectbox.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "uw_selectbox.h"
#include "lvgl.h"

#include <string.h>

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS &uw_selectbox_class
#define MY_CLASS_LIST &uw_selectboxlist_class

#define UW_SELECTBOX_PR_NONE 0xFFFF

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_obj_t *uw_selectbox_list_create(lv_obj_t *parent);
static void uw_selectbox_constructor(const lv_obj_class_t *class_p, lv_obj_t *obj);
static void uw_selectbox_destructor(const lv_obj_class_t *class_p, lv_obj_t *obj);
static void uw_selectbox_event(const lv_obj_class_t *class_p, lv_event_t *e);
static void draw_main(lv_event_t *e);

static void uw_selectboxlist_constructor(const lv_obj_class_t *class_p, lv_obj_t *obj);
static void uw_selectboxlist_destructor(const lv_obj_class_t *class_p, lv_obj_t *list_obj);
static void uw_selectbox_list_event(const lv_obj_class_t *class_p, lv_event_t *e);
static void draw_list(lv_event_t *e);

static void draw_box(lv_obj_t *dropdown_obj, lv_draw_ctx_t *draw_ctx, uint16_t id, lv_state_t state);
static void draw_box_label(lv_obj_t *dropdown_obj, lv_draw_ctx_t *draw_ctx, uint16_t id, lv_state_t state);
static lv_res_t btn_release_handler(lv_obj_t *obj);
static lv_res_t list_release_handler(lv_obj_t *list_obj);
static void list_press_handler(lv_obj_t *page);
static uint16_t get_id_on_point(lv_obj_t *dropdown_obj, lv_coord_t y);
static void position_to_selected(lv_obj_t *obj);
static lv_obj_t *get_label(const lv_obj_t *obj);

static int global_list_height = 100;

/**********************
 *  STATIC VARIABLES
 **********************/
/* selectbox 控件的主要部分 */
const lv_obj_class_t uw_selectbox_class = {
    .constructor_cb = uw_selectbox_constructor,
    .destructor_cb = uw_selectbox_destructor,
    .event_cb = uw_selectbox_event,
    .width_def = LV_DPI_DEF,
    .height_def = LV_SIZE_CONTENT,
    .instance_size = sizeof(uw_selectbox_t),
    .editable = LV_OBJ_CLASS_EDITABLE_TRUE,
    .group_def = LV_OBJ_CLASS_GROUP_DEF_TRUE,
    .base_class = &lv_obj_class
};

/* selectbox 中的列表部分, 列表部分会在 open 后被绘制 */
const lv_obj_class_t uw_selectboxlist_class = {
    .constructor_cb = uw_selectboxlist_constructor,
    .destructor_cb = uw_selectboxlist_destructor,
    .event_cb = uw_selectbox_list_event,
    .instance_size = sizeof(uw_selectbox_list_t),
    .base_class = &lv_obj_class
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t *uw_selectbox_create(lv_obj_t *parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t *obj = lv_obj_class_create_obj(&uw_selectbox_class, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

/*=====================
 * Setter functions
 *====================*/

void uw_selectbox_set_text(lv_obj_t *obj, const char *txt)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;
    if (dropdown->text == txt)
        return;

    dropdown->text = txt;

    lv_obj_invalidate(obj);
}

void uw_selectbox_set_options(lv_obj_t *obj, const char *options)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    LV_ASSERT_NULL(options);

    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;

    /*Count the '\n'-s to determine the number of options*/
    dropdown->option_cnt = 0;
    uint32_t i;
    for (i = 0; options[i] != '\0'; i++)
    {
        if (options[i] == '\n')
            dropdown->option_cnt++;
    }
    dropdown->option_cnt++; /*Last option has no `\n`*/
    dropdown->sel_opt_id = 0;
    dropdown->sel_opt_id_orig = 0;

    /*Allocate space for the new text*/
#if LV_USE_ARABIC_PERSIAN_CHARS == 0
    size_t len = strlen(options) + 1;
#else
    size_t len = _lv_txt_ap_calc_bytes_cnt(options) + 1;
#endif

    if (dropdown->options != NULL && dropdown->static_txt == 0)
    {
        lv_mem_free(dropdown->options);
        dropdown->options = NULL;
    }

    dropdown->options = lv_mem_alloc(len);

    LV_ASSERT_MALLOC(dropdown->options);
    if (dropdown->options == NULL)
        return;

#if LV_USE_ARABIC_PERSIAN_CHARS == 0
    strcpy(dropdown->options, options);
#else
    _lv_txt_ap_proc(options, dropdown->options);
#endif

    /*Now the text is dynamically allocated*/
    dropdown->static_txt = 0;

    lv_obj_invalidate(obj);
    if (dropdown->list)
        lv_obj_invalidate(dropdown->list);
}

void uw_selectbox_set_options_static(lv_obj_t *obj, const char *options)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    LV_ASSERT_NULL(options);

    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;

    /*Count the '\n'-s to determine the number of options*/
    dropdown->option_cnt = 0;
    uint32_t i;
    for (i = 0; options[i] != '\0'; i++)
    {
        if (options[i] == '\n')
            dropdown->option_cnt++;
    }
    dropdown->option_cnt++; /*Last option has no `\n`*/
    dropdown->sel_opt_id = 0;
    dropdown->sel_opt_id_orig = 0;

    if (dropdown->static_txt == 0 && dropdown->options != NULL)
    {
        lv_mem_free(dropdown->options);
        dropdown->options = NULL;
    }

    dropdown->static_txt = 1;
    dropdown->options = (char *)options;

    lv_obj_invalidate(obj);
    if (dropdown->list)
        lv_obj_invalidate(dropdown->list);
}

void uw_selectbox_add_option(lv_obj_t *obj, const char *option, uint32_t pos)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    LV_ASSERT_NULL(option);

    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;

    /*Convert static options to dynamic*/
    if (dropdown->static_txt != 0)
    {
        char *static_options = dropdown->options;
        size_t len = strlen(static_options) + 1;

        dropdown->options = lv_mem_alloc(len);
        LV_ASSERT_MALLOC(dropdown->options);
        if (dropdown->options == NULL)
            return;

        strcpy(dropdown->options, static_options);
        dropdown->static_txt = 0;
    }

    /*Allocate space for the new option*/
    size_t old_len = (dropdown->options == NULL) ? 0 : strlen(dropdown->options);
#if LV_USE_ARABIC_PERSIAN_CHARS == 0
    size_t ins_len = strlen(option) + 1;
#else
    size_t ins_len = _lv_txt_ap_calc_bytes_cnt(option) + 1;
#endif

    size_t new_len = ins_len + old_len + 2; /*+2 for terminating NULL and possible \n*/
    dropdown->options = lv_mem_realloc(dropdown->options, new_len + 1);
    LV_ASSERT_MALLOC(dropdown->options);
    if (dropdown->options == NULL)
        return;

    dropdown->options[old_len] = '\0';

    /*Find the insert character position*/
    uint32_t insert_pos = old_len;
    if (pos != UW_SELECTBOX_POS_LAST)
    {
        uint32_t opcnt = 0;
        for (insert_pos = 0; dropdown->options[insert_pos] != 0; insert_pos++)
        {
            if (opcnt == pos)
                break;
            if (dropdown->options[insert_pos] == '\n')
                opcnt++;
        }
    }

    /*Add delimiter to existing options*/
    if ((insert_pos > 0) && (pos >= dropdown->option_cnt))
        _lv_txt_ins(dropdown->options, _lv_txt_encoded_get_char_id(dropdown->options, insert_pos++), "\n");

    /*Insert the new option, adding \n if necessary*/
    char *ins_buf = lv_mem_buf_get(ins_len + 2); /*+ 2 for terminating NULL and possible \n*/
    LV_ASSERT_MALLOC(ins_buf);
    if (ins_buf == NULL)
        return;
#if LV_USE_ARABIC_PERSIAN_CHARS == 0
    strcpy(ins_buf, option);
#else
    _lv_txt_ap_proc(option, ins_buf);
#endif
    if (pos < dropdown->option_cnt)
        strcat(ins_buf, "\n");

    _lv_txt_ins(dropdown->options, _lv_txt_encoded_get_char_id(dropdown->options, insert_pos), ins_buf);
    lv_mem_buf_release(ins_buf);

    dropdown->option_cnt++;

    lv_obj_invalidate(obj);
    if (dropdown->list)
        lv_obj_invalidate(dropdown->list);
}

void uw_selectbox_clear_options(lv_obj_t *obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;
    if (dropdown->options == NULL)
        return;

    if (dropdown->static_txt == 0)
        lv_mem_free(dropdown->options);

    dropdown->options = NULL;
    dropdown->static_txt = 0;
    dropdown->option_cnt = 0;

    lv_obj_invalidate(obj);
    if (dropdown->list)
        lv_obj_invalidate(dropdown->list);
}

void uw_selectbox_set_selected(lv_obj_t *obj, uint16_t sel_opt)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;
    if (dropdown->sel_opt_id == sel_opt)
        return;

    dropdown->sel_opt_id = sel_opt < dropdown->option_cnt ? sel_opt : dropdown->option_cnt - 1;
    dropdown->sel_opt_id_orig = dropdown->sel_opt_id;

    lv_obj_invalidate(obj);
}

void uw_selectbox_set_dir(lv_obj_t *obj, lv_dir_t dir)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;
    if (dropdown->dir == dir)
        return;

    dropdown->dir = dir;

    lv_obj_invalidate(obj);
}

void uw_selectbox_set_symbol(lv_obj_t *obj, const void *symbol)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;
    dropdown->symbol = symbol;
    lv_obj_invalidate(obj);
}

void uw_selectbox_set_selected_highlight(lv_obj_t *obj, bool en)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;
    dropdown->selected_highlight = en;
    if (dropdown->list)
        lv_obj_invalidate(dropdown->list);
}

/*=====================
 * Getter functions
 *====================*/

lv_obj_t *uw_selectbox_get_list(lv_obj_t *obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;
    uw_selectbox_list_t *list = (uw_selectbox_list_t *)dropdown->list;

    return (lv_obj_t *)list;
}

const char *uw_selectbox_get_text(lv_obj_t *obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;

    return dropdown->text;
}

const char *uw_selectbox_get_options(const lv_obj_t *obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;
    return dropdown->options == NULL ? "" : dropdown->options;
}

uint16_t uw_selectbox_get_selected(const lv_obj_t *obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;

    return dropdown->sel_opt_id;
}

uint16_t uw_selectbox_get_option_cnt(const lv_obj_t *obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;

    return dropdown->option_cnt;
}

void uw_selectbox_get_selected_str(const lv_obj_t *obj, char *buf, uint32_t buf_size)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;

    uint32_t i;
    uint32_t line = 0;
    size_t txt_len;

    if (dropdown->options)
    {
        txt_len = strlen(dropdown->options);
    }
    else
    {
        buf[0] = '\0';
        return;
    }

    for (i = 0; i < txt_len && line != dropdown->sel_opt_id_orig; i++)
    {
        if (dropdown->options[i] == '\n')
            line++;
    }

    uint32_t c;
    for (c = 0; i < txt_len && dropdown->options[i] != '\n'; c++, i++)
    {
        if (buf_size && c >= buf_size - 1)
        {
            LV_LOG_WARN("uw_selectbox_get_selected_str: the buffer was too small");
            break;
        }
        buf[c] = dropdown->options[i];
    }

    buf[c] = '\0';
}

int32_t uw_selectbox_get_option_index(lv_obj_t *obj, const char *option)
{
    const char *opts = uw_selectbox_get_options(obj);
    uint32_t char_i = 0;
    uint32_t opt_i = 0;
    const char *start = opts;

    while (start[char_i] != '\0')
    {
        for (char_i = 0; (start[char_i] != '\n') && (start[char_i] != '\0'); char_i++)
            ;

        if (memcmp(start, option, LV_MIN(strlen(option), char_i)) == 0)
            return opt_i;
        start = &start[char_i];
        if (start[0] == '\n')
            start++;
        opt_i++;
    }

    return -1;
}

const char *uw_selectbox_get_symbol(lv_obj_t *obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;
    return dropdown->symbol;
}

bool uw_selectbox_get_selected_highlight(lv_obj_t *obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;
    return dropdown->selected_highlight;
}

lv_dir_t uw_selectbox_get_dir(const lv_obj_t *obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;
    return dropdown->dir;
}

/*=====================
 * Other functions
 *====================*/


/**
 * @brief 实现弹出下拉窗
 * 
 * @param dropdown_obj 
 */
void uw_selectbox_open(lv_obj_t *dropdown_obj)
{
    LV_ASSERT_OBJ(dropdown_obj, MY_CLASS);

    uw_selectbox_t *dropdown = (uw_selectbox_t *)dropdown_obj;
    uw_selectbox_list_t *list = (uw_selectbox_list_t *)dropdown->list;
    lv_obj_t *list_obj = (lv_obj_t *)list;

    lv_obj_add_state(dropdown_obj, LV_STATE_CHECKED);

    lv_obj_set_size(list->cont_hint, LV_PCT(100), lv_obj_get_style_height(list_obj, LV_PART_INDICATOR));
    lv_obj_set_style_border_width(list->cont_hint, lv_obj_get_style_border_width(list_obj, LV_PART_INDICATOR), 0);
    lv_obj_set_style_radius(list->cont_hint, lv_obj_get_style_radius(list_obj, LV_PART_INDICATOR), 0);
    lv_obj_set_style_bg_color(list->cont_hint, lv_obj_get_style_bg_color(list_obj, LV_PART_INDICATOR), 0);
    lv_obj_set_style_bg_opa(list->cont_hint, lv_obj_get_style_bg_opa(list_obj, LV_PART_INDICATOR), 0);
    lv_obj_set_style_pad_bottom(list->cont_hint, lv_obj_get_style_pad_bottom(list_obj, LV_PART_INDICATOR), 0);
    lv_obj_set_style_pad_top(list->cont_hint, lv_obj_get_style_pad_top(list_obj, LV_PART_INDICATOR), 0);
    lv_obj_set_style_pad_left(list->cont_hint, lv_obj_get_style_pad_left(list_obj, LV_PART_INDICATOR), 0);
    lv_obj_set_style_pad_right(list->cont_hint, lv_obj_get_style_pad_right(list_obj, LV_PART_INDICATOR), 0);
    lv_obj_set_style_text_color(list->cont_hint, lv_obj_get_style_text_color(list_obj, LV_PART_INDICATOR), 0);
    lv_obj_set_style_text_font(list->cont_hint, lv_obj_get_style_text_font(list_obj, LV_PART_INDICATOR), 0);
    lv_obj_align(lv_obj_get_child(list->cont_hint, 0), LV_ALIGN_CENTER, 0, - lv_obj_get_style_radius(list_obj, LV_PART_INDICATOR) / 2);

    lv_obj_set_style_border_width(list->cont_list, lv_obj_get_style_border_width(list_obj, LV_PART_CUSTOM_FIRST), 0);
    lv_obj_set_style_radius(list->cont_list, lv_obj_get_style_radius(list_obj, LV_PART_CUSTOM_FIRST), 0);
    lv_obj_set_style_bg_color(list->cont_list, lv_obj_get_style_bg_color(list_obj, LV_PART_CUSTOM_FIRST), 0);
    lv_obj_set_style_bg_opa(list->cont_list, lv_obj_get_style_bg_opa(list_obj, LV_PART_CUSTOM_FIRST), 0);
    lv_obj_set_style_pad_bottom(list->cont_list, lv_obj_get_style_pad_bottom(list_obj, LV_PART_CUSTOM_FIRST), 0);
    lv_obj_set_style_pad_top(list->cont_list, lv_obj_get_style_pad_top(list_obj, LV_PART_CUSTOM_FIRST), 0);
    lv_obj_set_style_pad_left(list->cont_list, lv_obj_get_style_pad_left(list_obj, LV_PART_CUSTOM_FIRST), 0);
    lv_obj_set_style_pad_right(list->cont_list, lv_obj_get_style_pad_right(list_obj, LV_PART_CUSTOM_FIRST), 0);
    lv_obj_set_style_text_color(list->cont_list, lv_obj_get_style_text_color(list_obj, LV_PART_CUSTOM_FIRST), 0);

    /* 设置背景占全屏 */
    lv_obj_set_parent(dropdown->list, lv_obj_get_screen(dropdown_obj));
    lv_obj_set_style_bg_color(dropdown->list, lv_color_make(0, 0, 0), 0);
    lv_obj_set_style_bg_opa(dropdown->list, LV_OPA_30, 0);
    lv_obj_set_style_pad_all(dropdown->list, 0, 0);
    lv_obj_set_size(dropdown->list, LV_PCT(100), LV_PCT(100));
    lv_obj_align(dropdown->list, LV_ALIGN_CENTER, 0, 0);

    /* 列表容器 */
    lv_obj_set_size(list->cont_list, LV_PCT(100), global_list_height);
    lv_obj_align(list->cont_list, LV_ALIGN_BOTTOM_MID, 0, 0);
    
    /* 列表提示栏，显示当前列表标题 */
    lv_obj_align_to(list->cont_hint, list->cont_list, LV_ALIGN_OUT_TOP_MID, 0, lv_obj_get_style_radius(list_obj, LV_PART_INDICATOR));

    /* lv_obj_move_to_index ? */
    lv_obj_move_to_index(dropdown->list, -1);
    lv_obj_clear_flag(dropdown->list, LV_OBJ_FLAG_HIDDEN);

    /* To allow styling the list */
    lv_event_send(dropdown_obj, LV_EVENT_READY, NULL);

    lv_obj_t *label = get_label(dropdown_obj);
    lv_label_set_text_static(label, dropdown->options);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_update_layout(label);

    position_to_selected(dropdown_obj);
}

void uw_selectbox_close(lv_obj_t *obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    lv_obj_clear_state(obj, LV_STATE_CHECKED);
    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;

    dropdown->pr_opt_id = UW_SELECTBOX_PR_NONE;
    lv_obj_add_flag(dropdown->list, LV_OBJ_FLAG_HIDDEN);

    lv_event_send(obj, LV_EVENT_CANCEL, NULL);
}

bool uw_selectbox_is_open(lv_obj_t *obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;

    return lv_obj_has_flag(dropdown->list, LV_OBJ_FLAG_HIDDEN) ? false : true;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static lv_obj_t *uw_selectbox_list_create(lv_obj_t *parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t *obj = lv_obj_class_create_obj(&uw_selectboxlist_class, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

/**
 * @brief 构造函数
 *
 * @param class_p
 * @param obj
 */
static void uw_selectbox_constructor(const lv_obj_class_t *class_p, lv_obj_t *obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;

    /*Initialize the allocated 'ext'*/
    dropdown->list = NULL;
    dropdown->options = NULL;
    dropdown->symbol = LV_SYMBOL_DOWN;
    dropdown->text = NULL;
    dropdown->static_txt = 1;
    dropdown->selected_highlight = 1;
    dropdown->sel_opt_id = 0;
    dropdown->sel_opt_id_orig = 0;
    dropdown->pr_opt_id = UW_SELECTBOX_PR_NONE;
    dropdown->option_cnt = 0;
    dropdown->dir = LV_DIR_BOTTOM;

    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    uw_selectbox_set_options_static(obj, "Option 1\nOption 2\nOption 3");

    dropdown->list = uw_selectbox_list_create(lv_obj_get_screen(obj));
    uw_selectbox_list_t *list = (uw_selectbox_list_t *)dropdown->list;
    list->dropdown = obj;

    LV_TRACE_OBJ_CREATE("finished");
}

/**
 * @brief 析构函数
 *
 * 对象被删除时调用该函数完成对象删除
 *
 * @param class_p
 * @param obj
 */
static void uw_selectbox_destructor(const lv_obj_class_t *class_p, lv_obj_t *obj)
{
    LV_UNUSED(class_p);
    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;

    if (dropdown->list)
    {
        lv_obj_del(dropdown->list);
        dropdown->list = NULL;
    }

    if (!dropdown->static_txt)
    {
        lv_mem_free(dropdown->options);
        dropdown->options = NULL;
    }
}

static void list_cont_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    uw_selectbox_list_t *list = (uw_selectbox_list_t *)lv_obj_get_parent(obj);
    uw_selectbox_t *dropdown = (uw_selectbox_t *)list->dropdown;
    lv_obj_t *dropdown_obj = (lv_obj_t *)list->dropdown;
	
    if(code == LV_EVENT_PRESSED)
    {
        lv_indev_t *indev = lv_indev_get_act();
        if (indev && (lv_indev_get_type(indev) == LV_INDEV_TYPE_POINTER || lv_indev_get_type(indev) == LV_INDEV_TYPE_BUTTON))
        {
            lv_point_t p;
            lv_indev_get_point(indev, &p);

            /* 获取当前按下点的位置 */
            dropdown->pr_opt_id = get_id_on_point(dropdown_obj, p.y);
            lv_obj_invalidate(obj);
        }
    }

    else if(code == LV_EVENT_RELEASED)
    {
        if (lv_indev_get_scroll_obj(lv_indev_get_act()) == NULL)
        {
            list_release_handler(obj);
        }
    }

    /* 开始滚动时取消选择 */
    else if (code == LV_EVENT_SCROLL_BEGIN)
    {
        dropdown->pr_opt_id = UW_SELECTBOX_PR_NONE;
        lv_obj_invalidate((lv_obj_t *)list);
    }
}


static void hint_cont_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    /* 实现拖拽列表 */
    if(code == LV_EVENT_PRESSING)
    {
        lv_indev_t* indev = lv_indev_get_act();
        lv_point_t p;
        lv_indev_get_point(indev, &p);
        uw_selectbox_list_t *list = (uw_selectbox_list_t *)lv_obj_get_parent(obj);
        uw_selectbox_t *dropdown = (uw_selectbox_t *)list->dropdown;
        lv_obj_t *dropdown_obj = (lv_obj_t *)list->dropdown;

        int move_h = 0;
        
        /* 计算应该移动的高度 */
        if(p.y < obj->coords.y1)
        {
            move_h = obj->coords.y1 - p.y;
        }

        else if(p.y > obj->coords.y2)
        {
            move_h = obj->coords.y2 - p.y;
        }

        if(move_h != 0)
        {
            int h = lv_obj_get_height(list->cont_list);
            global_list_height = h + move_h;
            int screen_h = lv_obj_get_height(lv_obj_get_screen(dropdown_obj));
            int hint_cont_height = lv_obj_get_height(list->cont_hint);

            /* 高度最高不超过屏幕高度 */
            if(global_list_height > (screen_h - hint_cont_height))
                global_list_height = screen_h - hint_cont_height;

            lv_obj_set_height(list->cont_list, global_list_height);
            lv_obj_align_to(list->cont_hint, list->cont_list, LV_ALIGN_OUT_TOP_MID, 0, lv_obj_get_style_radius((lv_obj_t*)list, LV_PART_INDICATOR));
            lv_obj_invalidate(dropdown->list);

            /* 在此执行该函数可保证高度变化时页面中心总是在当前选中的列表项 */
            position_to_selected(dropdown_obj);
        }
    }
}

/**
 * @brief 列表的构造函数
 *
 * @param class_p
 * @param obj
 */
static void uw_selectboxlist_constructor(const lv_obj_class_t *class_p, lv_obj_t *obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);

    uw_selectbox_list_t *list = (uw_selectbox_list_t *)obj;

    /* 标题栏 */
    list->cont_hint = lv_obj_create((lv_obj_t *)list);

    lv_obj_t *label_hint = lv_label_create(list->cont_hint);
    lv_obj_add_event_cb(list->cont_hint, hint_cont_event_cb, LV_EVENT_ALL, NULL);

    /* 列表 */
    list->cont_list = lv_obj_create((lv_obj_t *)list);
    lv_obj_clear_flag(list->cont_list, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(list->cont_list, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_add_flag(list->cont_list, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(list->cont_list, LV_PCT(100), 300);
    lv_obj_add_event_cb(list->cont_list, list_cont_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_t *label_list = lv_label_create(list->cont_list);
    lv_obj_align(label_list, LV_ALIGN_CENTER, 0, 0);

    LV_TRACE_OBJ_CREATE("finished");
}

/**
 * @brief 列表的析构函数
 *
 * @param class_p
 * @param list_obj
 */
static void uw_selectboxlist_destructor(const lv_obj_class_t *class_p, lv_obj_t *list_obj)
{
    LV_UNUSED(class_p);
    uw_selectbox_list_t *list = (uw_selectbox_list_t *)list_obj;
    lv_obj_t *dropdown_obj = list->dropdown;
    uw_selectbox_t *dropdown = (uw_selectbox_t *)dropdown_obj;
    dropdown->list = NULL;
}

/**
 * @brief 事件处理函数
 *
 * @param class_p
 * @param e
 */
static void uw_selectbox_event(const lv_obj_class_t *class_p, lv_event_t *e)
{
    LV_UNUSED(class_p);

    lv_res_t res;

    /* 调用父类事件处理 */
    res = lv_obj_event_base(MY_CLASS, e);
    if (res != LV_RES_OK)
        return;

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;

    /* 处理聚焦事件 */
    if (code == LV_EVENT_FOCUSED)
    {
        lv_group_t *g = lv_obj_get_group(obj);
        bool editing = lv_group_get_editing(g);
        lv_indev_type_t indev_type = lv_indev_get_type(lv_indev_get_act());

        /*Encoders need special handling*/
        if (indev_type == LV_INDEV_TYPE_ENCODER)
        {
            /*Open the list if editing*/
            if (editing)
            {
                uw_selectbox_open(obj);
            }
            /*Close the list if navigating*/
            else
            {
                dropdown->sel_opt_id = dropdown->sel_opt_id_orig;
                uw_selectbox_close(obj);
            }
        }
    }

    /* 聚焦消失, 则关闭下拉窗口 */
    // else if (code == LV_EVENT_DEFOCUSED || code == LV_EVENT_LEAVE)
    // {
    //     uw_selectbox_close(obj);
    // }

    /* 按键释放 */
    else if (code == LV_EVENT_RELEASED)
    {
        res = btn_release_handler(obj);
        if (res != LV_RES_OK)
            return;
    }

    /* 样式改变 */
    else if (code == LV_EVENT_STYLE_CHANGED)
    {
        lv_obj_refresh_self_size(obj);
    }

    /* 大小改变 */
    else if (code == LV_EVENT_SIZE_CHANGED)
    {
        lv_obj_refresh_self_size(obj);
    }

    /* 获取大小? */
    else if (code == LV_EVENT_GET_SELF_SIZE)
    {
        lv_point_t *p = lv_event_get_param(e);
        const lv_font_t *font = lv_obj_get_style_text_font(obj, LV_PART_MAIN);
        p->y = lv_font_get_line_height(font);
    }

    /* 处理按键事件 */
    else if (code == LV_EVENT_KEY)
    {
        char c = *((char *)lv_event_get_param(e));
        if (c == LV_KEY_RIGHT || c == LV_KEY_DOWN)
        {
            if (!uw_selectbox_is_open(obj))
            {
                uw_selectbox_open(obj);
            }

            else if (dropdown->sel_opt_id + 1 < dropdown->option_cnt)
            {
                dropdown->sel_opt_id++;
                position_to_selected(obj);
            }

            /* 到达末尾，则滚动到开头 */
            else
            {
                dropdown->sel_opt_id = 0;
                position_to_selected(obj);
            }
        }

        else if (c == LV_KEY_LEFT || c == LV_KEY_UP)
        {
            if (!uw_selectbox_is_open(obj))
            {
                uw_selectbox_open(obj);
            }

            else if (dropdown->sel_opt_id > 0)
            {
                dropdown->sel_opt_id--;
                position_to_selected(obj);
            }

            /* 到达开头则滚动到末尾 */
            else
            {
                dropdown->sel_opt_id = dropdown->option_cnt - 1;
                position_to_selected(obj);
            }
        }

        else if (c == LV_KEY_ESC)
        {
            dropdown->sel_opt_id = dropdown->sel_opt_id_orig;
            uw_selectbox_close(obj);
        }

        else if (c == LV_KEY_ENTER)
        {
            /* Handle the ENTER key only if it was send by an other object.
             * Do no process it if ENTER is sent by the dropdown because it's handled in LV_EVENT_RELEASED */
            lv_obj_t *indev_obj = lv_indev_get_obj_act();
            if (indev_obj != obj)
            {
                res = btn_release_handler(obj);
                if (res != LV_RES_OK)
                    return;
            }
        }
    }

    /* 绘制事件 */
    else if (code == LV_EVENT_DRAW_MAIN)
    {
        draw_main(e);
    }
}

/**
 * @brief 列表部分的事件处理, 处理点击从列表中选择列表项
 *
 * @param class_p
 * @param e
 */
static void uw_selectbox_list_event(const lv_obj_class_t *class_p, lv_event_t *e)
{
    LV_UNUSED(class_p);

    lv_res_t res;

    /*Call the ancestor's event handler*/
    lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_DRAW_POST)
    {
        res = lv_obj_event_base(MY_CLASS_LIST, e);
        if (res != LV_RES_OK)
            return;
    }
    lv_obj_t *list = lv_event_get_target(e);
    lv_obj_t *dropdown_obj = ((uw_selectbox_list_t *)list)->dropdown;
    uw_selectbox_t *dropdown = (uw_selectbox_t *)dropdown_obj;

    /* 点击空白处时释放按键则关闭下拉窗 */
    if (code == LV_EVENT_RELEASED)
    {
        if (lv_indev_get_scroll_obj(lv_indev_get_act()) == NULL)
        {
            // list_release_handler(list);
            uw_selectbox_close(dropdown_obj);
        }
    }

    else if (code == LV_EVENT_DRAW_POST)
    {
        draw_list(e);
        res = lv_obj_event_base(MY_CLASS_LIST, e);
        if (res != LV_RES_OK)
            return;
    }
}

/* 绘制主要部分 */
static void draw_main(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;
    lv_draw_ctx_t *draw_ctx = lv_event_get_draw_ctx(e);

    lv_coord_t border_width = lv_obj_get_style_border_width(obj, LV_PART_MAIN);
    lv_coord_t left = lv_obj_get_style_pad_left(obj, LV_PART_MAIN) + border_width;
    lv_coord_t right = lv_obj_get_style_pad_right(obj, LV_PART_MAIN) + border_width;
    lv_coord_t top = lv_obj_get_style_pad_top(obj, LV_PART_MAIN) + border_width;

    lv_draw_label_dsc_t symbol_dsc;
    lv_draw_label_dsc_init(&symbol_dsc);
    lv_obj_init_draw_label_dsc(obj, LV_PART_INDICATOR, &symbol_dsc);

    /*If no text specified use the selected option*/
    const char *opt_txt;
    if (dropdown->text)
        opt_txt = dropdown->text;
    else
    {
        char *buf = lv_mem_buf_get(128);
        uw_selectbox_get_selected_str(obj, buf, 128);
        opt_txt = buf;
    }

    bool symbol_to_left = false;
    if (dropdown->dir == LV_DIR_LEFT)
        symbol_to_left = true;
    if (lv_obj_get_style_base_dir(obj, LV_PART_MAIN) == LV_BASE_DIR_RTL)
        symbol_to_left = true;

    if (dropdown->symbol)
    {
        lv_img_src_t symbol_type = lv_img_src_get_type(dropdown->symbol);
        lv_coord_t symbol_w;
        lv_coord_t symbol_h;
        if (symbol_type == LV_IMG_SRC_SYMBOL)
        {
            lv_point_t size;
            lv_txt_get_size(&size, dropdown->symbol, symbol_dsc.font, symbol_dsc.letter_space, symbol_dsc.line_space,
                            LV_COORD_MAX, symbol_dsc.flag);
            symbol_w = size.x;
            symbol_h = size.y;
        }
        else
        {
            lv_img_header_t header;
            lv_res_t res = lv_img_decoder_get_info(dropdown->symbol, &header);
            if (res == LV_RES_OK)
            {
                symbol_w = header.w;
                symbol_h = header.h;
            }
            else
            {
                symbol_w = -1;
                symbol_h = -1;
            }
        }

        lv_area_t symbol_area;
        if (symbol_to_left)
        {
            symbol_area.x1 = obj->coords.x1 + left;
            symbol_area.x2 = symbol_area.x1 + symbol_w - 1;
        }
        else
        {
            symbol_area.x1 = obj->coords.x2 - right - symbol_w;
            symbol_area.x2 = symbol_area.x1 + symbol_w - 1;
        }

        if (symbol_type == LV_IMG_SRC_SYMBOL)
        {
            symbol_area.y1 = obj->coords.y1 + top;
            symbol_area.y2 = symbol_area.y1 + symbol_h - 1;
            lv_draw_label(draw_ctx, &symbol_dsc, &symbol_area, dropdown->symbol, NULL);
        }
        else
        {
            symbol_area.y1 = obj->coords.y1 + (lv_obj_get_height(obj) - symbol_h) / 2;
            symbol_area.y2 = symbol_area.y1 + symbol_h - 1;
            lv_draw_img_dsc_t img_dsc;
            lv_draw_img_dsc_init(&img_dsc);
            lv_obj_init_draw_img_dsc(obj, LV_PART_INDICATOR, &img_dsc);
            img_dsc.pivot.x = symbol_w / 2;
            img_dsc.pivot.y = symbol_h / 2;
            img_dsc.angle = lv_obj_get_style_transform_angle(obj, LV_PART_INDICATOR);
            lv_draw_img(draw_ctx, &img_dsc, &symbol_area, dropdown->symbol);
        }
    }

    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    lv_obj_init_draw_label_dsc(obj, LV_PART_MAIN, &label_dsc);

    lv_point_t size;
    lv_txt_get_size(&size, opt_txt, label_dsc.font, label_dsc.letter_space, label_dsc.line_space, LV_COORD_MAX,
                    label_dsc.flag);

    lv_area_t txt_area;
    txt_area.y1 = obj->coords.y1 + top;
    txt_area.y2 = txt_area.y1 + size.y;
    /* 如果没有 symbol 则中心对齐 */
    if (dropdown->symbol == NULL)
    {
        txt_area.x1 = obj->coords.x1 + (lv_obj_get_width(obj) - size.x) / 2;
        txt_area.x2 = txt_area.x1 + size.x;
    }
    else
    {
        /*Text to the right*/
        if (symbol_to_left)
        {
            txt_area.x1 = obj->coords.x2 - right - size.x;
            txt_area.x2 = txt_area.x1 + size.x;
        }
        else
        {
            txt_area.x1 = obj->coords.x1 + left;
            txt_area.x2 = txt_area.x1 + size.x;
        }
    }
    lv_draw_label(draw_ctx, &label_dsc, &txt_area, opt_txt, NULL);

    if (dropdown->text == NULL)
    {
        lv_mem_buf_release((char *)opt_txt);
    }
}

/**
 * @brief 绘制列表部分, 主要处理聚焦和绘制选中的高亮
 *
 * @param e
 */
static void draw_list(lv_event_t *e)
{
    lv_obj_t *list_obj = lv_event_get_target(e);
    uw_selectbox_list_t *list = (uw_selectbox_list_t *)list_obj;
    lv_obj_t *dropdown_obj = list->dropdown;
    uw_selectbox_t *dropdown = (uw_selectbox_t *)dropdown_obj;
    lv_draw_ctx_t *draw_ctx = lv_event_get_draw_ctx(e);

    /* Clip area might be too large too to shadow but
    * the selected option can be drawn on only the background*/
    lv_area_t clip_area_core;
    bool has_common;
    has_common = _lv_area_intersect(&clip_area_core, draw_ctx->clip_area, &list->cont_list->coords);
    if (has_common)
    {
        const lv_area_t *clip_area_ori = draw_ctx->clip_area;
        draw_ctx->clip_area = &clip_area_core;
        if (dropdown->selected_highlight)
        {
            if (dropdown->pr_opt_id == dropdown->sel_opt_id)
            {
                draw_box(dropdown_obj, draw_ctx, dropdown->pr_opt_id, LV_STATE_CHECKED | LV_STATE_PRESSED);
                draw_box_label(dropdown_obj, draw_ctx, dropdown->pr_opt_id, LV_STATE_CHECKED | LV_STATE_PRESSED);
            }
            else
            {
                draw_box(dropdown_obj, draw_ctx, dropdown->pr_opt_id, LV_STATE_PRESSED);
                draw_box_label(dropdown_obj, draw_ctx, dropdown->pr_opt_id, LV_STATE_PRESSED);
                draw_box(dropdown_obj, draw_ctx, dropdown->sel_opt_id, LV_STATE_CHECKED);
                draw_box_label(dropdown_obj, draw_ctx, dropdown->sel_opt_id, LV_STATE_CHECKED);
            }
        }
        else
        {
            draw_box(dropdown_obj, draw_ctx, dropdown->pr_opt_id, LV_STATE_PRESSED);
            draw_box_label(dropdown_obj, draw_ctx, dropdown->pr_opt_id, LV_STATE_PRESSED);
        }
        draw_ctx->clip_area = clip_area_ori;
    }
}

/**
 * @brief 绘制盒子
 *
 * @param dropdown_obj
 * @param draw_ctx
 * @param id
 * @param state
 */
static void  draw_box(lv_obj_t *dropdown_obj, lv_draw_ctx_t *draw_ctx, uint16_t id, lv_state_t state)
{
    if (id == UW_SELECTBOX_PR_NONE)
        return;

    uw_selectbox_t *dropdown = (uw_selectbox_t *)dropdown_obj;
    uw_selectbox_list_t *list = (uw_selectbox_list_t *)dropdown->list;
    lv_obj_t *list_obj = list->cont_list;
    lv_state_t state_ori = list_obj->state;

    if (state != list_obj->state)
    {
        list_obj->state = state;
        list_obj->skip_trans = 1;
    }

    /*Draw a rectangle under the selected item*/
    const lv_font_t *font = lv_obj_get_style_text_font((lv_obj_t *)list, LV_PART_SELECTED);
    lv_coord_t line_space = lv_obj_get_style_text_line_space((lv_obj_t *)list, LV_PART_SELECTED);
    lv_coord_t font_h = lv_font_get_line_height(font);

    /* 对选中的选项添加矩形框 */
    lv_obj_t *label = get_label(dropdown_obj);
    lv_area_t rect_area;
    rect_area.y1 = label->coords.y1;
    rect_area.y1 += id * (font_h + line_space);
    rect_area.y1 -= line_space / 2;

    rect_area.y2 = rect_area.y1 + font_h + line_space - 1;
    rect_area.x1 = list->cont_list->coords.x1;
    rect_area.x2 = list->cont_list->coords.x2;

    lv_draw_rect_dsc_t sel_rect;
    lv_draw_rect_dsc_init(&sel_rect);
    lv_obj_init_draw_rect_dsc((lv_obj_t *)list, LV_PART_SELECTED, &sel_rect);
    lv_draw_rect(draw_ctx, &sel_rect, &rect_area);

    list_obj->state = state_ori;
    list_obj->skip_trans = 0;
}

static void draw_box_label(lv_obj_t *dropdown_obj, lv_draw_ctx_t *draw_ctx, uint16_t id, lv_state_t state)
{
    if (id == UW_SELECTBOX_PR_NONE)
        return;

    uw_selectbox_t *dropdown = (uw_selectbox_t *)dropdown_obj;
    uw_selectbox_list_t *list = (uw_selectbox_list_t *)dropdown->list;
    lv_obj_t *list_obj = list->cont_list;
    lv_state_t state_orig = list_obj->state;

    if (state != list_obj->state)
    {
        list_obj->state = state;
        list_obj->skip_trans = 1;
    }

    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    lv_obj_init_draw_label_dsc((lv_obj_t *)list, LV_PART_SELECTED, &label_dsc);

    label_dsc.line_space =
        lv_obj_get_style_text_line_space((lv_obj_t *)list, LV_PART_SELECTED); /*Line space should come from the list*/

    lv_obj_t *label = get_label(dropdown_obj);
    if (label == NULL)
        return;

    lv_coord_t font_h = lv_font_get_line_height(label_dsc.font);

    lv_area_t area_sel;
    area_sel.y1 = label->coords.y1;
    area_sel.y1 += id * (font_h + label_dsc.line_space);
    area_sel.y1 -= label_dsc.line_space / 2;

    area_sel.y2 = area_sel.y1 + font_h + label_dsc.line_space - 1;
    area_sel.x1 = list_obj->coords.x1;
    area_sel.x2 = list_obj->coords.x2;
    lv_area_t mask_sel;
    bool area_ok;
    area_ok = _lv_area_intersect(&mask_sel, draw_ctx->clip_area, &area_sel);
    if (area_ok)
    {
        const lv_area_t *clip_area_ori = draw_ctx->clip_area;
        draw_ctx->clip_area = &mask_sel;
        lv_draw_label(draw_ctx, &label_dsc, &label->coords, lv_label_get_text(label), NULL);
        draw_ctx->clip_area = clip_area_ori;
    }
    list_obj->state = state_orig;
    list_obj->skip_trans = 0;
}

static lv_res_t btn_release_handler(lv_obj_t *obj)
{
    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;
    lv_indev_t *indev = lv_indev_get_act();
    if (lv_indev_get_scroll_obj(indev) == NULL)
    {
        if (uw_selectbox_is_open(obj))
        {
            uw_selectbox_close(obj);
            if (dropdown->sel_opt_id_orig != dropdown->sel_opt_id)
            {
                dropdown->sel_opt_id_orig = dropdown->sel_opt_id;
                lv_res_t res;
                uint32_t id = dropdown->sel_opt_id; /*Just to use uint32_t in event data*/
                res = lv_event_send(obj, LV_EVENT_VALUE_CHANGED, &id);
                if (res != LV_RES_OK)
                    return res;
                lv_obj_invalidate(obj);
            }
            lv_indev_type_t indev_type = lv_indev_get_type(indev);
            if (indev_type == LV_INDEV_TYPE_ENCODER)
            {
                lv_group_set_editing(lv_obj_get_group(obj), false);
            }
        }
        else
        {
            uw_selectbox_open(obj);
        }
    }
    else
    {
        dropdown->sel_opt_id = dropdown->sel_opt_id_orig;
        lv_obj_invalidate(obj);
    }
    return LV_RES_OK;
}

/**
 * Called when a drop down list is released to open it or set new option
 * @param list pointer to the drop down list's list
 * @return LV_RES_INV if the list is not being deleted in the user callback. Else LV_RES_OK
 */
static lv_res_t list_release_handler(lv_obj_t *list_cont)
{
    lv_obj_get_parent(list_cont);
    uw_selectbox_list_t *list = (uw_selectbox_list_t *)lv_obj_get_parent(list_cont);
    lv_obj_t *dropdown_obj = list->dropdown;
    uw_selectbox_t *dropdown = (uw_selectbox_t *)dropdown_obj;

    lv_indev_t *indev = lv_indev_get_act();
    /*Leave edit mode once a new item is selected*/
    if (lv_indev_get_type(indev) == LV_INDEV_TYPE_ENCODER)
    {
        dropdown->sel_opt_id_orig = dropdown->sel_opt_id;
        lv_group_t *g = lv_obj_get_group(dropdown_obj);
        if (lv_group_get_editing(g))
        {
            lv_group_set_editing(g, false);
        }
    }

    /*Search the clicked option (For KEYPAD and ENCODER the new value should be already set)*/
    if (lv_indev_get_type(indev) == LV_INDEV_TYPE_POINTER || lv_indev_get_type(indev) == LV_INDEV_TYPE_BUTTON)
    {
        lv_point_t p;
        lv_indev_get_point(indev, &p);
        dropdown->sel_opt_id = get_id_on_point(dropdown_obj, p.y);
        dropdown->sel_opt_id_orig = dropdown->sel_opt_id;
    }

    uw_selectbox_close(dropdown_obj);

    /*Invalidate to refresh the text*/
    if (dropdown->text == NULL)
        lv_obj_invalidate(dropdown_obj);

    uint32_t id = dropdown->sel_opt_id; /*Just to use uint32_t in event data*/
    lv_res_t res = lv_event_send(dropdown_obj, LV_EVENT_VALUE_CHANGED, &id);
    if (res != LV_RES_OK)
        return res;

    return LV_RES_OK;
}

/**
 * @brief 处理列表按下时的事件
 * 
 * @param list_obj 
 */
static void list_press_handler(lv_obj_t *list_cont)
{   
    lv_obj_get_parent(list_cont);
    uw_selectbox_list_t *list = (uw_selectbox_list_t *)lv_obj_get_parent(list_cont);
    lv_obj_t *dropdown_obj = list->dropdown;
    uw_selectbox_t *dropdown = (uw_selectbox_t *)dropdown_obj;

    lv_indev_t *indev = lv_indev_get_act();
    if (indev && (lv_indev_get_type(indev) == LV_INDEV_TYPE_POINTER || lv_indev_get_type(indev) == LV_INDEV_TYPE_BUTTON))
    {
        lv_point_t p;
        lv_indev_get_point(indev, &p);
        /* 获取当前按下点的位置 */
        dropdown->pr_opt_id = get_id_on_point(dropdown_obj, p.y);
        lv_obj_invalidate(list_cont);
    }
}

/**
 * @brief 根据坐标获取 options 的 id
 * 
 * @param dropdown_obj 
 * @param y 
 * @return uint16_t 
 */
static uint16_t get_id_on_point(lv_obj_t *dropdown_obj, lv_coord_t y)
{
    uw_selectbox_t *dropdown = (uw_selectbox_t *)dropdown_obj;
    lv_obj_t *label = get_label(dropdown_obj);
    if (label == NULL)
        return 0;
    y -= label->coords.y1;

    const lv_font_t *font = lv_obj_get_style_text_font(label, LV_PART_MAIN);
    lv_coord_t font_h = lv_font_get_line_height(font);
    lv_coord_t line_space = lv_obj_get_style_text_line_space(label, LV_PART_MAIN);

    y += line_space / 2;
    lv_coord_t h = font_h + line_space;

    uint16_t opt = y / h;

    if (opt >= dropdown->option_cnt)
        opt = dropdown->option_cnt - 1;
    return opt;
}

/**
 * Set the position of list when it is closed to show the selected item
 * @param ddlist pointer to a drop down list
 */
static void position_to_selected(lv_obj_t *dropdown_obj)
{
    uw_selectbox_t *dropdown = (uw_selectbox_t *)dropdown_obj;
    uw_selectbox_list_t *list = (uw_selectbox_list_t *)dropdown->list;

    lv_obj_t *label = get_label(dropdown_obj);
    if (label == NULL)
        return;

    /* 当 label 高度小于容器高度时将 label 对齐到中心 */
    if (lv_obj_get_height(label) <= lv_obj_get_content_height(list->cont_list))
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    
    /* 否则依然保持在顶部，因为拖动 hint_cont 时会导致重置对齐到中心, 有再拖回去的可能，所以这里重新设置对齐方式 */
    else
        lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

    lv_coord_t h = lv_obj_get_height(list->cont_list);
    const lv_font_t *font = lv_obj_get_style_text_font(label, LV_PART_MAIN);
    lv_coord_t font_h = lv_font_get_line_height(font);
    lv_coord_t line_space = lv_obj_get_style_text_line_space(label, LV_PART_MAIN);

    lv_coord_t unit_h = font_h + line_space;
    // lv_coord_t line_y1 =  dropdown->sel_opt_id * unit_h;
    /* 该滚动量将使得当存在很多个选项时，列表的光标总是指向中心 */
    lv_coord_t line_y1 = dropdown->sel_opt_id * unit_h - (h - font_h)/ 2;
    
    /* 滚动容器页面到指定位置 */
    lv_obj_scroll_to_y(list->cont_list, line_y1, LV_ANIM_OFF);
    lv_obj_invalidate(dropdown->list);
}

/**
 * @brief 获取标签
 * 
 * @param obj 
 * @return lv_obj_t* 
 */
static lv_obj_t *get_label(const lv_obj_t *obj)
{
    uw_selectbox_t *dropdown = (uw_selectbox_t *)obj;
    if (dropdown->list == NULL)
        return NULL;

    uw_selectbox_list_t *list = (uw_selectbox_list_t *)dropdown->list;
    if (list->cont_list == NULL)
        return NULL;

    return lv_obj_get_child(list->cont_list, 0);
}
