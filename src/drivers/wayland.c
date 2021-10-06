// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim: ft=cpp:expandtab:ts=4:sw=4:softtabstop=4:
// SPDX-FileCopyrightText: Huawei Inc.
// SPDX-License-Identifier: MIT

#if defined(USE_WAYLAND) && USE_WAYLAND

#include <pthread.h>
#include <unistd.h>

#include <lv_drivers/wayland/wayland.h>


static void * tick_thread(void * data)
{
    (void) data;
    while(true) {
        usleep(5 * 1000);
        lv_tick_inc(5);
    }
}

void hal_init(void)
{
    wayland_init();

    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t color_buf[WAYLAND_HOR_RES * WAYLAND_VER_RES];
    lv_disp_draw_buf_init(&draw_buf, color_buf, NULL,
                          WAYLAND_HOR_RES * WAYLAND_VER_RES
                         );

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res = WAYLAND_HOR_RES;
    disp_drv.ver_res = WAYLAND_VER_RES;
    disp_drv.flush_cb = wayland_flush;
    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);

    lv_theme_t * th = lv_theme_default_init(disp,
                                            lv_palette_main(LV_PALETTE_BLUE),
                                            lv_palette_main(LV_PALETTE_RED),
                                            LV_THEME_DEFAULT_DARK,
                                            LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, th);

    lv_group_t * g = lv_group_create();
    lv_group_set_default(g);

    static lv_indev_drv_t mouse_drv;
    lv_indev_drv_init(&mouse_drv);
    mouse_drv.type = LV_INDEV_TYPE_POINTER;
    mouse_drv.read_cb = wayland_pointer_read;
    lv_indev_t * mouse_indev = lv_indev_drv_register(&mouse_drv);

    static lv_indev_drv_t touch_drv;
    lv_indev_drv_init(&touch_drv);
    touch_drv.type = LV_INDEV_TYPE_POINTER;
    touch_drv.read_cb = wayland_pointer_read;
    lv_indev_t * touch_indev = lv_indev_drv_register(&touch_drv);

    static lv_indev_drv_t keyboard_drv;
    lv_indev_drv_init(&keyboard_drv);
    keyboard_drv.type = LV_INDEV_TYPE_KEYPAD;
    keyboard_drv.read_cb = wayland_keyboard_read;

    lv_indev_t * keyboard_indev = lv_indev_drv_register(&keyboard_drv);
    lv_indev_set_group(keyboard_indev, g);

    static pthread_t hal_thread;
    pthread_create(&hal_thread, NULL, tick_thread, NULL);
}



#endif
