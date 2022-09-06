// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim: ft=cpp:expandtab:ts=4:sw=4:softtabstop=4:
// SPDX-FileCopyrightText: Huawei Inc.
// SPDX-License-Identifier: MIT

#if defined(USE_WAYLAND) && USE_WAYLAND

#include <pthread.h>
#include <unistd.h>

#include <lv_drivers/wayland/wayland.h>

#ifndef WAYLAND_HOR_RES
#define WAYLAND_HOR_RES      480
#endif

#ifndef WAYLAND_VER_RES
#define WAYLAND_VER_RES      320
#endif

static void * tick_thread(void * data)
{
    (void) data;
    while(true) {
        usleep(5 * 1000);
        lv_tick_inc(5);
    }
}


static lv_wayland_display_close_f_t close_cb()
{
}


void hal_init(void)
{
    lv_wayland_init();
    lv_disp_t *disp = lv_wayland_create_window(WAYLAND_HOR_RES, WAYLAND_VER_RES, "Wayland", close_cb);

    lv_theme_t * th = lv_theme_default_init(disp,
                                            lv_palette_main(LV_PALETTE_BLUE),
                                            lv_palette_main(LV_PALETTE_RED),
                                            LV_THEME_DEFAULT_DARK,
                                            LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, th);

    lv_group_t * g = lv_group_create();
    lv_group_set_default(g);
    static pthread_t hal_thread;
    pthread_create(&hal_thread, NULL, tick_thread, NULL);
}

#endif
