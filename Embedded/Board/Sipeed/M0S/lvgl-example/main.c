/**
 * @file main.c
 * @brief
 *
 * Copyright (c) 2021 Bouffalolab team
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 *
 */
#include "board.h"
#include "bflb_gpio.h"
#include "bflb_l1c.h"
#include "bflb_mtimer.h"

#include "lv_conf.h"
#include "lvgl.h"

#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include "demos/lv_demos.h"

#include "lcd.h"

#include "cst816.h"

/* lvgl log cb */
void lv_log_print_g_cb(const char *buf)
{
    printf("[LVGL] %s", buf);
}

void my_port_indev_init(void)
{
    static lv_indev_drv_t indev_drv;

    cst816_init();

    /* Basic initialization */
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = cst826_read;

    lv_indev_drv_register(&indev_drv);
}


int main(void)
{
    board_init();


    printf("LCD Backlight ON\r\n");
    struct bflb_device_s *gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, GPIO_PIN_11, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_set(gpio, GPIO_PIN_11);

    printf("lvgl case\r\n");

    /* lvgl init */
    lv_log_register_print_cb(lv_log_print_g_cb);
    lv_init();
    lv_port_disp_init();
    // lv_port_indev_init();
    my_port_indev_init();

    // lv_demo_benchmark();
    // lv_demo_stress();
    lv_demo_widgets();

    lv_task_handler();

    printf("lvgl success\r\n");

    while (1) {
        lv_task_handler();
        bflb_mtimer_delay_ms(1);
    }
}
