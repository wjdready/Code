#ifndef CST816_H
#define CST816_H

#include <stdint.h>
#include "lvgl.h"

#define CST816_DEFAULT_ADDR 0x15
#define CONFIG_LV_DISPLAY_ORIENTATION 0

int cst816_init(void);
void cst826_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
#endif
