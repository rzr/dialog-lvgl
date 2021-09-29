// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim: ft=cpp:expandtab:ts=4:sw=4:softtabstop=4:
// SPDX-FileCopyrightText: Huawei Inc.
// SPDX-License-Identifier: MIT

#if defined(USE_SDL) && USE_SDL

#include <SDL2/SDL.h>
#include <lvgl/lv_drivers/sdl/sdl.h>


static int tick_thread(void * data)
{
    (void) data;
    while(true) {
        SDL_Delay(5);
        lv_tick_inc(5);
    }
}


void hal_init(void)
{
    sdl_init();

    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t color_buf[SDL_HOR_RES * SDL_VER_RES];
    lv_disp_draw_buf_init(&draw_buf, color_buf, NULL,
                          SDL_HOR_RES * SDL_VER_RES
                         );

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res = SDL_HOR_RES;
    disp_drv.ver_res = SDL_VER_RES;
    disp_drv.flush_cb = sdl_display_flush;
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
    mouse_drv.read_cb = sdl_mouse_read;
    lv_indev_t * mouse_indev = lv_indev_drv_register(&mouse_drv);

    static lv_indev_drv_t keyboard_drv;
    lv_indev_drv_init(&keyboard_drv);
    keyboard_drv.type = LV_INDEV_TYPE_KEYPAD;
    keyboard_drv.read_cb = sdl_keyboard_read;

    SDL_CreateThread(tick_thread, "tick", NULL);
}

#endif
