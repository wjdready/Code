

# LVGL 8 样式

通过定义一个样式, 然后初始化, 再对该样式进行配置即可应用到各种控件, 样式中可设置对象的宽高，位置，pad, 背景渐变 等， 这些属性都源于 CSS.

```c
static lv_style_t style;
lv_style_init(&style);

lv_style_set_radius(&style, 5);

lv_obj_t * obj = lv_obj_create(lv_scr_act());
lv_obj_add_style(obj, &style, 0);
```

类似于 HTML + CSS, 布局代码应该只用来打框架，具体颜色背景大小之类根据样式的不同而不同.

lv_obj_add_style 函数的第三个参数用来设置 PART 和 STATE, 即样式应用到控件的哪个部分以及控件在什么状态激活这个样式. 为0时表示主部分和默认状态.

```c
lv_obj_add_style(obj, &style_indic_pr, LV_PART_INDICATOR | LV_STATE_PRESSED);
```

## 控件特有样式

除了公共样式，有一些样式是某个控件特有的:

```c
// arc 控件
lv_style_set_arc_color(&style, lv_palette_main(LV_PALETTE_RED));
lv_style_set_arc_width(&style, 4);

// img 控件
lv_style_set_img_recolor(&style, lv_palette_main(LV_PALETTE_BLUE));
lv_style_set_img_recolor_opa(&style, LV_OPA_50);
```

## transition

LVGL 样式支持类似于 CSS 中的 transition，可通过设置样式来实现颜色、透明度、外型的动态变换.

例如按下后设置不同 transition:

```c
lv_obj_add_style(obj, &style_def, 0);
lv_obj_add_style(obj, &style_pr, LV_STATE_PRESSED);
```

## 多个样式

多个样式可以同时添加到同一个控件，最终呈现的效果就是这些样式属性的集合. 因此可以设置一个 Base 样式，然后不同控件再根据场景不同再添加新的样式。

## 本地样式

本地样式, 直接在框架代码中设置样式, 类似于直接在 html 中设置样式属性, 本地样式函数格式 `lv_obj_set_style_`, 例如:

```c
lv_obj_set_style_bg_color
```

## 主题:

主题就是一组样式集合, 可通过切换主题实现不同的样式效果。

```c
static lv_style_t style_btn;

/*Will be called when the styles of the base theme are already added
  to add new styles*/
static void new_theme_apply_cb(lv_theme_t * th, lv_obj_t * obj)
{
    LV_UNUSED(th);

    if(lv_obj_check_type(obj, &lv_btn_class)) {
        lv_obj_add_style(obj, &style_btn, 0);
    }
}

static void new_theme_init_and_set(void)
{
    /*Initialize the styles*/
    lv_style_init(&style_btn);
    lv_style_set_bg_color(&style_btn, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_border_color(&style_btn, lv_palette_darken(LV_PALETTE_GREEN, 3));
    lv_style_set_border_width(&style_btn, 3);

    /*Initialize the new theme from the current theme*/
    lv_theme_t * th_act = lv_disp_get_theme(NULL);
    static lv_theme_t th_new;
    th_new = *th_act;

    /*Set the parent theme and the style apply callback for the new theme*/
    lv_theme_set_parent(&th_new, th_act);
    lv_theme_set_apply_cb(&th_new, new_theme_apply_cb);

    /*Assign the new theme to the current display*/
    lv_disp_set_theme(NULL, &th_new);
}
```

