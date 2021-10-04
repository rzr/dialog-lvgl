// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim: ft=cpp:expandtab:ts=4:sw=4:softtabstop=4:
// SPDX-FileCopyrightText: Huawei Inc.
// SPDX-License-Identifier: MIT

#if defined(USE_ILI9341) && USE_ILI9341

#include <lvgl/lvgl.h>
#include <lvdrivers/display/ILI9341.h>

// TODO: test on lowend device and remove if not needed
#undef ILI9341_HOR_RES
#define ILI9341_HOR_RES (320>>1)
#undef ILI9341_VER_RES
#define ILI9341_VER_RES (240>>1)

void hal_init(void)
{
    ili9341_init();

    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t color_buf[ILI9341_HOR_RES * ILI9341_VER_RES];
    lv_disp_draw_buf_init(&draw_buf, color_buf, NULL,
                          ILI9341_HOR_RES * ILI9341_VER_RES
                         );

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res = LV_HOR_RES;
    disp_drv.ver_res = LV_VER_RES;
    disp_drv.flush_cb = ili9341_flush;
    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);

    lv_theme_t * th = lv_theme_default_init(disp,
                                            lv_palette_main(LV_PALETTE_BLUE),
                                            lv_palette_main(LV_PALETTE_RED),
                                            LV_THEME_DEFAULT_DARK,
                                            LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, th);

    lv_group_t * g = lv_group_create();
    lv_group_set_default(g);
}

#endif
