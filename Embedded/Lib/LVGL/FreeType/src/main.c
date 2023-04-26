
#include <stdio.h>
#include <unistd.h>
// #define SDL_MAIN_HANDLED
#include "lv_drivers/sdl/sdl.h"
#include "lvgl/demos/lv_demos.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/lvgl.h"
#include <SDL2/SDL.h>

#include "lv_freetype.h"
#include <ft2build.h>
#include <freetype/freetype.h>

static void hal_init(void);

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

    /*Init freetype library
     *Cache max 64 faces and 1 size*/
    lv_freetype_init(128, 20, 0);

    /*Create a font*/
    static lv_ft_info_t info;
    info.name = "./MSYH.TTC";
    info.weight = 32;
    info.style = FT_FONT_STYLE_NORMAL;
    lv_ft_font_init(&info);

    static lv_ft_info_t info2;
    info2.name = "./MSYH.TTC";
    info2.weight = 64;
    info2.style = FT_FONT_STYLE_NORMAL;
    lv_ft_font_init(&info2);

    /*Create a label with the new style*/
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label, info.font, 0);
    lv_label_set_text(label, "Hello world 中文");
    lv_obj_enable_style_refresh(true);

    int cnt = 0;
    int size = 32;

    FT_Library library;
    FT_Face face;
    FT_Error error = FT_Init_FreeType( &library );

    // error = FT_Set_Char_Size(face,    /* handle to face object */
    //                          0,       /* char_width in 1/64th of points */
    //                          16 * 64, /* char_height in 1/64th of points */
    //                          300,     /* horizontal device resolution */
    //                          300);    /* vertical device resolution */

    // error = FT_Set_Pixel_Sizes(face, /* handle to face object */
    //                            0,    /* pixel_width */
    //                            16);  /* pixel_height */

    // glyph_index = FT_Get_Char_Index(face, charcode);

    // error = FT_Load_Glyph(face,        /* handle to face object */
    //                       glyph_index, /* glyph index */
    //                       load_flags); /* load flags, see below */

    // error = FT_Render_Glyph(face->glyph,  /* glyph slot */
    //                         render_mode); /* render mode */

    // error = FT_Set_Transform(face,    /* target face object */
    //                          &matrix, /* pointer to 2x2 matrix */
    //                          &delta); /* pointer to 2d vector */

    // draw_bitmap( &slot->bitmap, pen_x + slot->bitmap_left, pen_y - slot->bitmap_top );

    error = FT_New_Face( library, "./MSYH.TTC", 0, &face );

    while (1)
    {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_timer_handler();

        if (cnt++ >= 100)
        {
            // size += 1;
            lv_ft_font_destroy(&info);
            info.name = "./MSYH.TTC";
            info.weight += 1;
            info.style = FT_FONT_STYLE_NORMAL;
            lv_ft_font_init(&info);
            
            lv_obj_del(label);
            label = lv_label_create(lv_scr_act());
            lv_obj_set_style_text_font(label, info2.font, 0);

            char buf[64];
            snprintf(buf, 64, "Hello 中文 %d", info.weight);
            lv_label_set_text(label, buf);
            cnt = 0;
            
        }

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

    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

    lv_theme_t *th = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                           LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, th);

    lv_group_t *g = lv_group_create();
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
    lv_indev_t *enc_indev = lv_indev_drv_register(&indev_drv_3);
    lv_indev_set_group(enc_indev, g);

    /*Set a cursor for the mouse*/
    // LV_IMG_DECLARE(mouse_cursor_icon);                  /*Declare the image file.*/
    // lv_obj_t *cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
    // lv_img_set_src(cursor_obj, &mouse_cursor_icon);     /*Set the image source*/
    // lv_indev_set_cursor(mouse_indev, cursor_obj);       /*Connect the image  object to the driver*/
}
