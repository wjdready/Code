
#include <stdio.h>
#include <unistd.h>
// #define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/sdl/sdl.h"

#include "uw_demo.h"
#include "uw_selectbox.h"

static void hal_init(void);


static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_DRAW_MAIN) 
    {
        // lv_obj_t * obj = lv_event_get_target(e);

        // lv_area_t area;
        // /* 获取对象的坐标, 也可以直接通过 obj->coords.y1 访问 */
        // lv_obj_get_coords(obj, &area);
        // lv_coord_t x_ofs = area.x1 - lv_obj_get_scroll_x(obj);
        // lv_coord_t y_ofs = area.y1 - lv_obj_get_scroll_y(obj);
        // // lv_coord_t h = lv_obj_get_height(obj);

        // /* 从对象中提取线条属性 */
        // lv_draw_line_dsc_t line_dsc;
        // lv_draw_line_dsc_init(&line_dsc);
        // lv_obj_init_draw_line_dsc(obj, LV_PART_MAIN, &line_dsc);

        // /* 获取绘制上下文 */
        // lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);

        // /* 绘制两条对角线 */
        // lv_point_t p1, p2;

        // p1.x = area.x1;
        // p1.y = area.y1;
        // p2.x = area.x2;
        // p2.y = area.y2;
        // lv_draw_line(draw_ctx, &line_dsc, &p1, &p2);

        // p1.x = area.x1;
        // p1.y = area.y2;
        // p2.x = area.x2;
        // p2.y = area.y1;
        // lv_draw_line(draw_ctx, &line_dsc, &p1, &p2);
    }
}

int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  hal_init();

//  lv_example_switch_1();
//  lv_example_calendar_1();
//  lv_example_btnmatrix_2();
//  lv_example_checkbox_1();
//  lv_example_colorwheel_1();
//  lv_example_chart_6();
//  lv_example_table_2();
//  lv_example_scroll_2();
//  lv_example_textarea_1();
//  lv_example_msgbox_1();
//  lv_example_dropdown_2();
//  lv_example_btn_1();
//  lv_example_scroll_1();
//  lv_example_tabview_1();
//  lv_example_tabview_1();
//  lv_example_flex_3();
//  lv_example_label_1();

    // lv_demo_widgets();

    // lv_example_dropdown_1();
    // lv_example_style_15();
    // lv_example_flex_1();
    // lv_example_canvas_1();

    // lv_obj_t *obj = uw_demo_create(lv_scr_act());
    // lv_obj_set_style_bg_color(obj, lv_color_make(255, 0, 0), 0);
    // lv_obj_set_style_bg_color(obj, lv_color_make(0, 0, 255), LV_STATE_PRESSED);
    // lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);
    // lv_obj_set_style_border_width(obj, 1, 0);
    // lv_obj_set_style_border_color(obj, lv_color_make(255, 0, 0), 0);
    // lv_obj_set_style_border_opa(obj, LV_OPA_100, 0);
    // lv_obj_set_style_radius(obj, 10, 0);

    // lv_obj_t *label = lv_label_create(obj);
    // lv_obj_set_width(label, LV_PCT(100));
    // lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);

    // lv_label_set_text(label, "G\no\nod\na asd\n asd\n as\nd \nas\naa\na\nq\nz\nsd\nsdads\na");
    // lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    // lv_obj_set_size(obj, 300, 200);
    // uw_demo_set_text(obj, "Hello \nWorld\n, I \nam \nd\ne\nm\no!\n");

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, 240, 40);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0 ,0);
    lv_obj_set_style_bg_color(cont, lv_color_make(41, 46, 67), 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_100, 0);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_all(cont, 5, 0);

    lv_obj_t *obj = uw_selectbox_create(cont);

    lv_obj_set_style_text_align(obj, LV_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_all(obj, 10, 0);

    uw_selectbox_set_options(obj, "Setcoo\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12");
    lv_obj_set_style_bg_color(obj, lv_color_make(41, 46, 67), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);
    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(obj, lv_color_make(255, 255, 255), 0);
    LV_IMG_DECLARE(img_caret_down)
    uw_selectbox_set_symbol(obj, LV_SYMBOL_RIGHT);
    lv_obj_set_style_transform_angle(obj, 1800, LV_PART_INDICATOR | LV_STATE_CHECKED);

    lv_obj_set_style_text_font(uw_selectbox_get_list(obj), &lv_font_montserrat_24, 0);
    lv_obj_set_style_bg_color(uw_selectbox_get_list(obj), lv_color_make(41, 46, 67), LV_PART_INDICATOR);
    lv_obj_set_style_text_color(uw_selectbox_get_list(obj), lv_color_make(255, 255, 255), LV_PART_INDICATOR);
    lv_obj_set_style_text_font(uw_selectbox_get_list(obj), &lv_font_montserrat_16, LV_PART_INDICATOR);
    lv_obj_set_style_height(uw_selectbox_get_list(obj), 45, LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(uw_selectbox_get_list(obj), LV_OPA_100, LV_PART_INDICATOR);
    lv_obj_set_style_radius(uw_selectbox_get_list(obj), 10, LV_PART_INDICATOR);

    lv_obj_set_style_pad_all(uw_selectbox_get_list(obj), 10, LV_PART_SELECTED);
    lv_obj_set_style_bg_opa(uw_selectbox_get_list(obj), LV_OPA_100, LV_PART_SELECTED);
    lv_obj_set_style_bg_color(uw_selectbox_get_list(obj), lv_color_make(34, 88, 236), LV_PART_SELECTED);
    lv_obj_set_style_text_color(uw_selectbox_get_list(obj), lv_color_make(255, 255, 255), LV_PART_SELECTED);

    lv_obj_set_style_border_width(uw_selectbox_get_list(obj), 0, LV_PART_CUSTOM_FIRST);
    lv_obj_set_style_radius(uw_selectbox_get_list(obj), 0, LV_PART_CUSTOM_FIRST);
    lv_obj_set_style_bg_color(uw_selectbox_get_list(obj), lv_color_make(24, 28, 53), LV_PART_CUSTOM_FIRST);
    lv_obj_set_style_bg_opa(uw_selectbox_get_list(obj), LV_OPA_100, LV_PART_CUSTOM_FIRST);
    lv_obj_set_style_pad_all(uw_selectbox_get_list(obj), 0, LV_PART_CUSTOM_FIRST);
    lv_obj_align(uw_selectbox_get_list(obj), LV_ALIGN_BOTTOM_MID, 0, LV_PART_CUSTOM_FIRST);
    lv_obj_set_style_text_color(uw_selectbox_get_list(obj), lv_color_make(255, 255, 255), LV_PART_CUSTOM_FIRST);

    lv_obj_align(obj, LV_ALIGN_RIGHT_MID, 0, 0);

    obj = uw_selectbox_create(lv_scr_act());
    uw_selectbox_set_options(obj, "1\n2\n3");

    for (int i = 4; i < 100; i++)
    {
        uw_selectbox_add_option(obj, "haode", i);
    }
    
    lv_obj_set_style_bg_color(obj, lv_color_make(255, 0, 0), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);

    lv_obj_set_style_bg_color(uw_selectbox_get_list(obj), lv_color_make(0, 0, 255), LV_PART_SELECTED);
    lv_obj_set_style_bg_opa(uw_selectbox_get_list(obj), LV_OPA_100, LV_PART_SELECTED);
    // lv_obj_set_style_text_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN);

    lv_obj_align(obj, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_set_style_text_color(obj, lv_color_make(0, 0, 0), 0);
    lv_obj_set_style_text_opa(obj, LV_OPA_COVER, 0);

    /* 更改图片颜色 */
    lv_obj_set_style_img_recolor(obj, lv_color_make(0, 0, 255), 0);
    lv_obj_set_style_img_recolor_opa(obj, LV_OPA_COVER, 0);

    lv_obj_set_size(obj, 100, 25);
    
    void ui_init(void);
    ui_init();

  while(1) {
      /* Periodically call the lv_task handler.
       * It could be done in a timer interrupt or an OS task too.*/
      lv_timer_handler();
      usleep(5 * 1000);
  }

  return 0;
}

static void hal_init(void)
{
  /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
  sdl_init();

  /*Create a display buffer*/
  static lv_disp_draw_buf_t disp_buf1;
  static lv_color_t buf1_1[SDL_HOR_RES * 100];
  lv_disp_draw_buf_init(&disp_buf1, buf1_1, NULL, SDL_HOR_RES * 100);

  /*Create a display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv); /*Basic initialization*/
  disp_drv.draw_buf = &disp_buf1;
  disp_drv.flush_cb = sdl_display_flush;
  disp_drv.hor_res = SDL_HOR_RES;
  disp_drv.ver_res = SDL_VER_RES;

  lv_disp_t * disp = lv_disp_drv_register(&disp_drv);

  lv_theme_t * th = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
  lv_disp_set_theme(disp, th);

  lv_group_t * g = lv_group_create();
  lv_group_set_default(g);

  /* Add the mouse as input device
   * Use the 'mouse' driver which reads the PC's mouse*/
  static lv_indev_drv_t indev_drv_1;
  lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
  indev_drv_1.type = LV_INDEV_TYPE_POINTER;

  /*This function will be called periodically (by the library) to get the mouse position and state*/
  indev_drv_1.read_cb = sdl_mouse_read;
  lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

  static lv_indev_drv_t indev_drv_2;
  lv_indev_drv_init(&indev_drv_2); /*Basic initialization*/
  indev_drv_2.type = LV_INDEV_TYPE_KEYPAD;
  indev_drv_2.read_cb = sdl_keyboard_read;
  lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_2);
  lv_indev_set_group(kb_indev, g);

  static lv_indev_drv_t indev_drv_3;
  lv_indev_drv_init(&indev_drv_3); /*Basic initialization*/
  indev_drv_3.type = LV_INDEV_TYPE_ENCODER;
  indev_drv_3.read_cb = sdl_mousewheel_read;
  lv_indev_t * enc_indev = lv_indev_drv_register(&indev_drv_3);
  lv_indev_set_group(enc_indev, g);

  /*Set a cursor for the mouse*/
//   LV_IMG_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
//   lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
//   lv_img_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
//   lv_indev_set_cursor(mouse_indev, cursor_obj);             /*Connect the image  object to the driver*/
}
