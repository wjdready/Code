#ifndef __UW_DEMO_H__
#define __UW_DEMO_H__

#include "lvgl.h"

lv_obj_t * uw_demo_create(lv_obj_t * parent);

void uw_demo_set_text(lv_obj_t *obj, const char *text);
void uw_demo_set_src(lv_obj_t *obj, const void *src);

#endif