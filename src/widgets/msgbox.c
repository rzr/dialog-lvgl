// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim: ft=cpp:expandtab:ts=4:sw=4:softtabstop=4:
// SPDX-FileCopyrightText: Huawei Inc.
// SPDX-License-Identifier: MIT

#include <stdlib.h>
#include <lvgl/lvgl.h>
#include "msgbox.h"

static void msgbox_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        exit(0);
    }
}

void msgbox_ui_init(int argc, char ** argv)
{
    lv_obj_t * label1 = lv_label_create(lv_scr_act());
    char * text = (argc > 2) ? argv[2] : "";
    lv_label_set_text(label1, text);

    lv_obj_t * btn = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn, msgbox_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "OK");
}
