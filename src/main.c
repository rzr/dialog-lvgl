// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim: ft=cpp:expandtab:ts=4:sw=4:softtabstop=4:
// SPDX-FileCopyrightText: Huawei Inc.
// SPDX-License-Identifier: MIT

#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>

#include <lvgl/lvgl.h>
#include <lv_drivers/display/monitor.h>
#include <lv_drivers/indev/mouse.h>
#include <lv_drivers/indev/keyboard.h>

#include "msgbox.h"
#include "yesno.h"


char * usage(int argc, char ** argv)
{
    printf("Usage: %s, <COMMAND> [OPTIONS]\n\
\n\
Description:\n\
 Basic UI utility to be used in scripts.\n\
 Inspired by ncurses' dialog, implemented using LVGL\n\
\n\
Commands: \n\
 --msgbox \"message\" 0 0\n\
 --yesno  \"message\" 0 0\n\
\n\
", argv[0]);
}


void default_ui_init(int argc, char ** argv)
{
    char * text = "Basic UI utility to be used in scripts.\n\
Inspired by ncurses dialog, implemented using LVGL\n\
For help run again with '--help' command option";
    lv_obj_t * label1 = lv_label_create(lv_scr_act());
    lv_label_set_text(label1, text);
    usage(argc, argv);
}


void ui_init(int argc, char ** argv)
{
    lv_obj_set_flex_flow(lv_scr_act(), LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(lv_scr_act(),
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);
    if((argc >= 1) && (argv[1])) {
        if(strcmp(argv[1], "--help") == 0) {
            usage(argc, argv);
        }
        else if(strcmp(argv[1], "--msgbox") == 0) {
            msgbox_ui_init(argc, argv);
        }
        else if(strcmp(argv[1], "--yesno") == 0) {
            yesno_ui_init(argc, argv);
        }
        else {
            default_ui_init(argc, argv);
        }
    }
    else {
        default_ui_init(argc, argv);
    }
}


static void hal_init(void)
{
    monitor_init();
    mouse_init();

    lv_disp_t * disp = lv_disp_get_default();
    lv_theme_t * th = lv_theme_default_init(disp,
                                            lv_palette_main(LV_PALETTE_BLUE),
                                            lv_palette_main(LV_PALETTE_RED),
                                            LV_THEME_DEFAULT_DARK,
                                            LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, th);

    lv_group_t * g = lv_group_create();
    lv_group_set_default(g);

    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1);
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;
    indev_drv_1.read_cb = mouse_read;
    lv_indev_t * mouse_indev = lv_indev_drv_register(&indev_drv_1);

    keyboard_init();
    static lv_indev_drv_t indev_drv_2;
    lv_indev_drv_init(&indev_drv_2);
    indev_drv_2.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv_2.read_cb = keyboard_read;
    lv_indev_t * kb_indev = lv_indev_drv_register(&indev_drv_2);
    lv_indev_set_group(kb_indev, g);
}


int main(int argc, char ** argv)
{
    lv_init();
    hal_init();
    ui_init(argc, argv);

    while(1) {
        lv_timer_handler();
        usleep(5 * 1000);
    }
    return 0;
}
